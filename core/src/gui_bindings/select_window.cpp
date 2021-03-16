#include "select_window.hpp"

#include <map>

#include "jump_actwin.hpp"
#include "screen_metrics.hpp"
#include "window_utility.hpp"

namespace SelectWindowCommon
{
    static std::unordered_map<HWND, RECT> g_rects ;
    static BOOL CALLBACK EnumWindowsProcForNearest(HWND hwnd, LPARAM lparam) {
        auto self_hwnd = reinterpret_cast<HWND>(lparam) ;
        if(self_hwnd == hwnd) {
            return TRUE ;
        }

        if(!WindowUtility::is_visible_hwnd(hwnd)) {
            return TRUE ;
        }

        RECT rect ;
        if(!GetWindowRect(hwnd, &rect)) {
            return TRUE ;
        }

        if(!WindowUtility::is_window_mode(hwnd, rect)) {
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
    inline static void select_nearest_window(
            T1&& is_if_target,
            T2&& calc_distance) {
        auto fg_hwnd = GetForegroundWindow() ;
        if(fg_hwnd == NULL) {
            throw RUNTIME_EXCEPT("There is not a foreground window.") ;
        }

        g_rects.clear() ;
        if(!EnumWindows(EnumWindowsProcForNearest,
                    reinterpret_cast<LPARAM>(fg_hwnd))) {

            throw RUNTIME_EXCEPT("Could not enumerate all top-level windows.") ;
        }

        RECT fg_rect ;
        if(!GetWindowRect(fg_hwnd, &fg_rect)) {
            throw RUNTIME_EXCEPT("Could not get a rectangle of a foreground window.") ;
        }

        std::map<LONG, HWND> distance_order_hwnd ;
        for(const auto& enumed_rect : SelectWindowCommon::g_rects) {
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

    SelectWindowCommon::select_nearest_window(is_if_target, calc_distance) ;
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

    SelectWindowCommon::select_nearest_window(is_if_target, calc_distance) ;
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

    SelectWindowCommon::select_nearest_window(is_if_target, calc_distance) ;
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

    SelectWindowCommon::select_nearest_window(is_if_target, calc_distance) ;
}
