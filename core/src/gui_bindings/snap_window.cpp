#include "snap_window.hpp"

#include <functional>

#include "screen_metrics.hpp"
#include "utility.hpp"
#include "window_utility.hpp"

namespace ResizeWindowCommon {
    inline static void snap_foreground_window(
            const std::function<void(RECT&, const RECT&)>& calc_half_size,
            const std::function<POINT(const RECT&)>& next_monitor_pos) {

        auto hwnd = GetForegroundWindow() ;
        if(hwnd == NULL) {
            throw RUNTIME_EXCEPT("There is not the foreground window.") ;
        }

        ScreenMetrics::MonitorInfo minfo ;
        ScreenMetrics::get_monitor_metrics(hwnd, minfo) ;

        RECT half_rect ;
        calc_half_size(half_rect, minfo.work_rect) ;

        RECT cur_rect ;
        if(!GetWindowRect(hwnd, &cur_rect)) {
            throw RUNTIME_EXCEPT("Could not get a rectangle of a foreground window.") ;
        }

        if(ScreenMetrics::is_equel(cur_rect, half_rect)) {
            ScreenMetrics::get_monitor_metrics(next_monitor_pos(minfo.rect), minfo) ;

            calc_half_size(half_rect, minfo.work_rect) ;
        }

        WindowUtility::resize(
                hwnd,
                half_rect.left,
                half_rect.top,
                ScreenMetrics::width(half_rect),
                ScreenMetrics::height(half_rect)) ;
    }
}

//SnapCurrentWindow2Left
const std::string SnapCurrentWindow2Left::sname() noexcept
{
    return "snap_current_window_to_left" ;
}

void SnapCurrentWindow2Left::sprocess(
        const bool first_call,
        const unsigned int UNUSED(repeat_num),
        VKCLogger* const UNUSED(parent_vkclgr),
        const CharLogger* const UNUSED(parent_charlgr))
{
    if(!first_call) return ;

    auto calc_half_size = [] (RECT& rect, const RECT& mrect) {
        rect.left   = mrect.left ;
        rect.top    = mrect.top ;
        rect.right  = rect.left + ScreenMetrics::width(mrect) / 2 ;
        rect.bottom = mrect.bottom ;
    } ;

    auto next_monitor_pos = [] (const RECT& rect) {
        return POINT{rect.left - 100, ScreenMetrics::center_y(rect)} ;
    } ;

    ResizeWindowCommon::snap_foreground_window(calc_half_size, next_monitor_pos) ;
}


//SnapCurrentWindow2Right
const std::string SnapCurrentWindow2Right::sname() noexcept
{
    return "snap_current_window_to_right" ;
}

void SnapCurrentWindow2Right::sprocess(
        const bool first_call,
        const unsigned int UNUSED(repeat_num),
        VKCLogger* const UNUSED(parent_vkclgr),
        const CharLogger* const UNUSED(parent_charlgr))
{
    if(!first_call) return ;

    auto calc_half_size = [] (RECT& rect, const RECT& mrect) {
        rect.left   = mrect.left + ScreenMetrics::width(mrect) / 2 ;
        rect.top    = mrect.top ;
        rect.right  = mrect.right ;
        rect.bottom = mrect.bottom ;
    } ;

    auto next_monitor_pos = [] (const RECT& rect) {
        return POINT{rect.right + 100, ScreenMetrics::center_y(rect)} ;
    } ;

    ResizeWindowCommon::snap_foreground_window(calc_half_size, next_monitor_pos) ;
}


//SnapCurrentWindow2Top
const std::string SnapCurrentWindow2Top::sname() noexcept
{
    return "snap_current_window_to_top" ;
}

void SnapCurrentWindow2Top::sprocess(
        const bool first_call,
        const unsigned int UNUSED(repeat_num),
        VKCLogger* const UNUSED(parent_vkclgr),
        const CharLogger* const UNUSED(parent_charlgr))
{
    if(!first_call) return ;

    auto calc_half_size = [] (RECT& rect, const RECT& mrect) {
        rect.left   = mrect.left ;
        rect.top    = mrect.top ;
        rect.right  = mrect.right ;
        rect.bottom = rect.top + ScreenMetrics::height(mrect) / 2 ;
    } ;

    auto next_monitor_pos = [] (const RECT& rect) {
        return POINT{ScreenMetrics::center_x(rect), rect.top - 100} ;
    } ;

    ResizeWindowCommon::snap_foreground_window(calc_half_size, next_monitor_pos) ;
}

//SnapCurrentWindow2Bottom
const std::string SnapCurrentWindow2Bottom::sname() noexcept
{
    return "snap_current_window_to_bottom" ;
}

void SnapCurrentWindow2Bottom::sprocess(
        const bool first_call,
        const unsigned int UNUSED(repeat_num),
        VKCLogger* const UNUSED(parent_vkclgr),
        const CharLogger* const UNUSED(parent_charlgr))
{
    if(!first_call) return ;

    auto calc_half_size = [] (RECT& rect, const RECT& mrect) {
        rect.left   = mrect.left ;
        rect.top    = mrect.top + ScreenMetrics::height(mrect) / 2 ;
        rect.right  = mrect.right ;
        rect.bottom = mrect.bottom ;
    } ;

    auto next_monitor_pos = [] (const RECT& rect) {
        return POINT{ScreenMetrics::center_x(rect), rect.bottom + 100} ;
    } ;

    ResizeWindowCommon::snap_foreground_window(calc_half_size, next_monitor_pos) ;
}

