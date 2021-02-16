#include "select_window.hpp"

#include <windows.h>

#include <map>
#include <unordered_map>

#include "jump_cursor.hpp"
#include "key_absorber.hpp"
#include "key_binder.hpp"
#include "key_logger.hpp"
#include "keybrd_eventer.hpp"
#include "move_cursor.hpp"
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
        KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr))
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

    KeyLogger logger{} ;
    while(win_vind::update_background()) {
        if(KeyAbsorber::is_pressed(VKC_ESC)) {
            break ;
        }
        if(KeyAbsorber::is_pressed(VKC_ENTER)) {
            break ;
        }

        if(!KyLgr::log_as_vkc(logger)) {
            Utility::remove_from_back(logger, 1) ;
            continue ;
        }
        if(KeyBinder::is_invalid_log(logger.back(),
                    KeyBinder::InvalidPolicy::UnbindedSystemKey)) {

            Utility::remove_from_back(logger, 1) ;
            continue ;
        }

        auto matched_func = KeyBinder::find_func(
                logger, nullptr, false,
                ModeManager::Mode::Normal) ;

        if(!matched_func) {
            logger.clear() ;
            continue ;
        }

        if(matched_func->is_callable()) {
            const auto name = matched_func->name() ;
            logger.clear() ;
            if(name == MoveLeft::sname()) {
                preserve_pushup(VKC_LEFT) ;
                continue ;
            }
            if(name == MoveRight::sname()) {
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

        if(!WindowCtrl::is_valid_hwnd(hwnd)) {
            return TRUE ;
        }

        RECT rect ;
        if(!GetWindowRect(hwnd, &rect)) {
            return TRUE ;
        }

        if(!WindowCtrl::is_valid_rect(hwnd, rect)) {
            return TRUE ;
        }

        RECT monitor_rect, monitor_rect_work ;
        ScreenMetrics::get_monitor_metrics(hwnd, &monitor_rect, &monitor_rect_work) ;

        if(ScreenMetrics::is_out_of_range(rect, monitor_rect_work)) {
            return TRUE ;
        }

        g_rects[hwnd] = std::move(rect) ;
        return TRUE ;
    }

    template <typename T1, typename T2>
    inline static void select_nearest_window(T1&& is_if_target, T2&& calc_distance) {
        auto hwnd = GetForegroundWindow() ;
        if(hwnd == NULL) {
            throw RUNTIME_EXCEPT("There is not a foreground window.") ;
        }

        SelectWindow::g_rects.clear() ;
        if(!EnumWindows(SelectWindow::EnumWindowsProcForNearest,
                    reinterpret_cast<LPARAM>(hwnd))) {

            throw RUNTIME_EXCEPT("Could not enumerate all top-level windows.") ;
        }

        RECT rect ;
        if(!GetWindowRect(hwnd, &rect)) {
            throw RUNTIME_EXCEPT("Could not get a rectangle of a foreground window.") ;
        }

        std::map<LONG, HWND> nearest ;
        for(const auto& hr : SelectWindow::g_rects) {
            auto& ehwnd  = hr.first ;
            auto& erect = hr.second ;

            auto cx  = ScreenMetrics::center_x(rect) ;
            auto cy  = ScreenMetrics::center_y(rect) ;
            auto ecx = ScreenMetrics::center_x(erect) ;
            auto ecy = ScreenMetrics::center_y(erect) ;

            if(is_if_target(rect, erect, cx, cy, ecx, ecy)) {
                nearest[calc_distance(rect, erect, cx, cy, ecx, ecy)] = ehwnd ;
            }
        }

        if(!nearest.empty()) {
            auto target_hwnd = nearest.begin()->second ;
            if(!SetForegroundWindow(target_hwnd)) {
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
        KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr))
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
            const auto& erect,
            auto UNUSED(cx), auto cy,
            auto UNUSED(ecx), auto ecy) {

        return ScreenMetrics::l2_distance_nosq(erect.right, ecy, rect.left, cy) / 100 ;
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
        KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr))
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
            const auto& erect,
            auto UNUSED(cx), auto cy,
            auto UNUSED(ecx), auto ecy) {

        return ScreenMetrics::l2_distance_nosq(erect.left, ecy, rect.right, cy) / 100 ;
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
        KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr))
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
            const auto& erect,
            auto cx, auto UNUSED(cy),
            auto ecx, auto UNUSED(ecy)) {

        return ScreenMetrics::l2_distance_nosq(ecx, erect.bottom, cx, rect.top) / 100 ;
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
        KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr))
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
            const auto& erect,
            auto cx, auto UNUSED(cy),
            auto ecx, auto UNUSED(ecy)) {

        return ScreenMetrics::l2_distance_nosq(ecx, erect.top, cx, rect.bottom) / 100 ;
    } ;

    SelectWindow::select_nearest_window(is_if_target, calc_distance) ;
}
