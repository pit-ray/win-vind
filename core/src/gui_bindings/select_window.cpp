#include "select_window.hpp"

#include <windows.h>

#include <map>
#include <unordered_map>

#include "jump_cursor.hpp"
#include "key_absorber.hpp"
#include "key_binder.hpp"
#include "vkc_logger.hpp"
#include "keybrd_eventer.hpp"
#include "edi_move_caret.hpp"
#include "screen_metrics.hpp"
#include "utility.hpp"
#include "win_vind.hpp"
#include "window_ctrl.hpp"

//SwitchWindow
const std::string SwitchWindow::sname() noexcept
{
    return "switch_window" ;
}

void SwitchWindow::sprocess(
        const bool first_call,
        const unsigned int UNUSED(repeat_num),
        VKCLogger* const UNUSED(parent_vkclgr),
        const CharLogger* const UNUSED(parent_charlgr))
{
    using namespace KeybrdEventer ;
    if(!first_call) return ;

    KeyAbsorber::InstantKeyAbsorber ika ;

    SmartKey alt(VKC_LALT) ;
    alt.press() ;
    KeyAbsorber::release_virtually(VKC_LALT) ;

    pushup(VKC_TAB) ;

    auto preserve_pushup = [] (const auto vkc) {
        using namespace KeybrdEventer ;
        if(!KeyAbsorber::is_pressed(vkc)) {
            pushup(vkc) ;
            return ;
        }

        release_keystate(vkc) ;
        pushup(vkc) ;
        press_keystate(vkc) ;
    } ;

    VKCLogger logger{} ;
    while(win_vind::update_background()) {
        if(KeyAbsorber::is_pressed(VKC_ESC)) {
            break ;
        }
        if(KeyAbsorber::is_pressed(VKC_ENTER)) {
            break ;
        }

        logger.update() ;
        if(!logger.is_changed()) {
            logger.remove_from_back(1) ;
            continue ;
        }
        if(KeyBinder::is_invalid_log(logger.latest(),
                    KeyBinder::InvalidPolicy::UnbindedSystemKey)) {

            logger.remove_from_back(1) ;
            continue ;
        }

        auto matched_func = KeyBinder::find_func(
                &logger, nullptr, false,
                ModeManager::Mode::EdiNormal) ;

        if(!matched_func) {
            logger.clear() ;
            continue ;
        }

        if(matched_func->is_callable()) {
            const auto name = matched_func->name() ;
            logger.clear() ;
            if(name == EdiMoveCaretLeft::sname()) {
                preserve_pushup(VKC_LEFT) ;
                continue ;
            }
            if(name == EdiMoveCaretRight::sname()) {
                preserve_pushup(VKC_RIGHT) ;
                continue ;
            }
        }
    }

    KeyAbsorber::release_virtually(VKC_ESC) ;
    KeyAbsorber::release_virtually(VKC_ENTER) ;

    alt.release() ;

    //jump cursor to a selected window after releasing alt and tab.
    Sleep(50) ; //send select-message to OS(wait)
    Jump2ActiveWindow::sprocess(true, 1, nullptr, nullptr) ;
}

namespace SelectWindow
{
    static std::unordered_map<HWND, RECT> g_rects ;
    static BOOL CALLBACK EnumWindowsProcForNearest(HWND hwnd, LPARAM lparam) {
        auto self_hwnd = reinterpret_cast<HWND>(lparam) ;
        if(self_hwnd == hwnd) {
            return TRUE ;
        }

        if(!WindowCtrl::is_visible_hwnd(hwnd)) {
            return TRUE ;
        }

        RECT rect ;
        if(!GetWindowRect(hwnd, &rect)) {
            return TRUE ;
        }

        if(!WindowCtrl::is_window_mode(hwnd, rect)) {
            return TRUE ;
        }

        ScreenMetrics::MonitorInfo minfo ;
        ScreenMetrics::get_monitor_metrics(hwnd, minfo) ;

        if(ScreenMetrics::is_out_of_range(rect, minfo.work_rect)) {
            return TRUE ;
        }

        g_rects[hwnd] = std::move(rect) ;
        return TRUE ;
    }

    template <typename T1, typename T2>
    inline static void select_nearest_window(T1&& is_if_target, T2&& calc_distance) {
        auto fg_hwnd = GetForegroundWindow() ;
        if(fg_hwnd == NULL) {
            throw RUNTIME_EXCEPT("There is not a foreground window.") ;
        }

        SelectWindow::g_rects.clear() ;
        if(!EnumWindows(SelectWindow::EnumWindowsProcForNearest,
                    reinterpret_cast<LPARAM>(fg_hwnd))) {

            throw RUNTIME_EXCEPT("Could not enumerate all top-level windows.") ;
        }

        RECT fg_rect ;
        if(!GetWindowRect(fg_hwnd, &fg_rect)) {
            throw RUNTIME_EXCEPT("Could not get a rectangle of a foreground window.") ;
        }

        std::map<LONG, HWND> distance_order_hwnd ;
        for(const auto& enumed_rect : SelectWindow::g_rects) {
            auto& enu_hwnd = enumed_rect.first ;
            auto& enu_rect = enumed_rect.second ;

            auto cx  = ScreenMetrics::center_x(fg_rect) ;
            auto cy  = ScreenMetrics::center_y(fg_rect) ;
            auto ecx = ScreenMetrics::center_x(enu_rect) ;
            auto ecy = ScreenMetrics::center_y(enu_rect) ;

            if(is_if_target(fg_rect, enu_rect, cx, cy, ecx, ecy)) {
                const auto distance = calc_distance(fg_rect, enu_rect, cx, cy, ecx, ecy) ;
                distance_order_hwnd[distance] = enu_hwnd ;
            }
        }

        if(!distance_order_hwnd.empty()) {
            auto nearest_hwnd = distance_order_hwnd.begin()->second ;
            if(!SetForegroundWindow(nearest_hwnd)) {
                throw RUNTIME_EXCEPT("Could not set a foreground window.") ;
            }
            Sleep(50) ;
            Jump2ActiveWindow::sprocess(true, 1, nullptr, nullptr) ;
        }
    }
}


//SelectLeftWindow
const std::string SelectLeftWindow::sname() noexcept
{
    return "select_left_window" ;
}
void SelectLeftWindow::sprocess(
        const bool first_call,
        const unsigned int UNUSED(repeat_num),
        VKCLogger* const UNUSED(parent_vkclgr),
        const CharLogger* const UNUSED(parent_charlgr))
{
    if(!first_call) return ;

    auto is_if_target = [] (
            const auto& UNUSED(rect),
            const auto& UNUSED(erect),
            auto cx, auto UNUSED(cy),
            auto ecx, auto UNUSED(ecy)) {
        return cx >= ecx ;
    } ;

    auto calc_distance = [] (
            const auto& rect,
            const auto& UNUSED(erect),
            auto UNUSED(cx), auto cy,
            auto ecx, auto ecy) {

        return ScreenMetrics::l2_distance_nosq(ecx, ecy, rect.left, cy) / 100 ;
    } ;

    SelectWindow::select_nearest_window(is_if_target, calc_distance) ;
}

//SelectRightWindow
const std::string SelectRightWindow::sname() noexcept
{
    return "select_right_window" ;
}
void SelectRightWindow::sprocess(
        const bool first_call,
        const unsigned int UNUSED(repeat_num),
        VKCLogger* const UNUSED(parent_vkclgr),
        const CharLogger* const UNUSED(parent_charlgr))
{
    if(!first_call) return ;

    auto is_if_target = [] (
            const auto& UNUSED(rect),
            const auto& UNUSED(erect),
            auto cx, auto UNUSED(cy),
            auto ecx, auto UNUSED(ecy)) {
        return cx <= ecx ;
    } ;

    auto calc_distance = [] (
            const auto& rect,
            const auto& UNUSED(erect),
            auto UNUSED(cx), auto cy,
            auto ecx, auto ecy) {

        return ScreenMetrics::l2_distance_nosq(ecx, ecy, rect.right, cy) / 100 ;
    } ;

    SelectWindow::select_nearest_window(is_if_target, calc_distance) ;
}

//SelectUpperWindow
const std::string SelectUpperWindow::sname() noexcept
{
    return "select_upper_window" ;
}
void SelectUpperWindow::sprocess(
        const bool first_call,
        const unsigned int UNUSED(repeat_num),
        VKCLogger* const UNUSED(parent_vkclgr),
        const CharLogger* const UNUSED(parent_charlgr))
{
    if(!first_call) return ;

    auto is_if_target = [] (
            const auto& UNUSED(rect),
            const auto& UNUSED(erect),
            auto UNUSED(cx), auto cy,
            auto UNUSED(ecx), auto ecy) {
        return cy >= ecy ;
    } ;

    auto calc_distance = [] (
            const auto& rect,
            const auto& UNUSED(erect),
            auto cx, auto UNUSED(cy),
            auto ecx, auto ecy) {

        return ScreenMetrics::l2_distance_nosq(ecx, ecy, cx, rect.top) / 100 ;
    } ;

    SelectWindow::select_nearest_window(is_if_target, calc_distance) ;
}

//SelectLowerWindow
const std::string SelectLowerWindow::sname() noexcept
{
    return "select_lower_window" ;
}
void SelectLowerWindow::sprocess(
        const bool first_call,
        const unsigned int UNUSED(repeat_num),
        VKCLogger* const UNUSED(parent_vkclgr),
        const CharLogger* const UNUSED(parent_charlgr))
{
    if(!first_call) return ;

    auto is_if_target = [] (
            const auto& UNUSED(rect),
            const auto& UNUSED(erect),
            auto UNUSED(cx), auto cy,
            auto UNUSED(ecx), auto ecy) {
        return cy <= ecy ;
    } ;

    auto calc_distance = [] (
            const auto& rect,
            const auto& UNUSED(erect),
            auto cx, auto UNUSED(cy),
            auto ecx, auto ecy) {

        return ScreenMetrics::l2_distance_nosq(ecx, ecy, cx, rect.bottom) / 100 ;
    } ;

    SelectWindow::select_nearest_window(is_if_target, calc_distance) ;
}
