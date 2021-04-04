#include "snap_window.hpp"

#include <functional>

#include "io/screen_metrics.hpp"
#include "util/def.hpp"
#include "window_utility.hpp"

namespace
{
    inline void snap_foreground_window(
            const std::function<void(RECT&, const RECT&)>& calc_half_size,
            const std::function<POINT(const RECT&)>& next_monitor_pos) {

        using namespace vind ;

        auto hwnd = GetForegroundWindow() ;
        if(hwnd == NULL) {
            throw RUNTIME_EXCEPT("There is not the foreground window.") ;
        }

        screenmetrics::MonitorInfo minfo ;
        screenmetrics::get_monitor_metrics(hwnd, minfo) ;

        RECT half_rect ;
        calc_half_size(half_rect, minfo.work_rect) ;

        RECT cur_rect ;
        if(!GetWindowRect(hwnd, &cur_rect)) {
            throw RUNTIME_EXCEPT("Could not get a rectangle of a foreground window.") ;
        }

        if(screenmetrics::is_equel(cur_rect, half_rect)) {
            screenmetrics::get_monitor_metrics(next_monitor_pos(minfo.rect), minfo) ;

            calc_half_size(half_rect, minfo.work_rect) ;
        }

        windowutil::resize(
                hwnd,
                half_rect.left,
                half_rect.top,
                screenmetrics::width(half_rect),
                screenmetrics::height(half_rect)) ;
    }
}

namespace vind
{
    //SnapCurrentWindow2Left
    const std::string SnapCurrentWindow2Left::sname() noexcept {
        return "snap_current_window_to_left" ;
    }

    void SnapCurrentWindow2Left::sprocess(
            bool first_call,
            unsigned int UNUSED(repeat_num),
            KeycodeLogger* const UNUSED(parent_keycodelgr),
            const CharLogger* const UNUSED(parent_charlgr)) {
        if(!first_call) return ;

        auto calc_half_size = [] (RECT& rect, const RECT& mrect) {
            rect.left   = mrect.left ;
            rect.top    = mrect.top ;
            rect.right  = rect.left + screenmetrics::width(mrect) / 2 ;
            rect.bottom = mrect.bottom ;
        } ;

        auto next_monitor_pos = [] (const RECT& rect) {
            return POINT{rect.left - 100, screenmetrics::center_y(rect)} ;
        } ;

        snap_foreground_window(calc_half_size, next_monitor_pos) ;
    }


    //SnapCurrentWindow2Right
    const std::string SnapCurrentWindow2Right::sname() noexcept {
        return "snap_current_window_to_right" ;
    }

    void SnapCurrentWindow2Right::sprocess(
            bool first_call,
            unsigned int UNUSED(repeat_num),
            KeycodeLogger* const UNUSED(parent_keycodelgr),
            const CharLogger* const UNUSED(parent_charlgr)) {
        if(!first_call) return ;

        auto calc_half_size = [] (RECT& rect, const RECT& mrect) {
            rect.left   = mrect.left + screenmetrics::width(mrect) / 2 ;
            rect.top    = mrect.top ;
            rect.right  = mrect.right ;
            rect.bottom = mrect.bottom ;
        } ;

        auto next_monitor_pos = [] (const RECT& rect) {
            return POINT{rect.right + 100, screenmetrics::center_y(rect)} ;
        } ;

        snap_foreground_window(calc_half_size, next_monitor_pos) ;
    }


    //SnapCurrentWindow2Top
    const std::string SnapCurrentWindow2Top::sname() noexcept {
        return "snap_current_window_to_top" ;
    }

    void SnapCurrentWindow2Top::sprocess(
            bool first_call,
            unsigned int UNUSED(repeat_num),
            KeycodeLogger* const UNUSED(parent_keycodelgr),
            const CharLogger* const UNUSED(parent_charlgr)) {
        if(!first_call) return ;

        auto calc_half_size = [] (RECT& rect, const RECT& mrect) {
            rect.left   = mrect.left ;
            rect.top    = mrect.top ;
            rect.right  = mrect.right ;
            rect.bottom = rect.top + screenmetrics::height(mrect) / 2 ;
        } ;

        auto next_monitor_pos = [] (const RECT& rect) {
            return POINT{screenmetrics::center_x(rect), rect.top - 100} ;
        } ;

        snap_foreground_window(calc_half_size, next_monitor_pos) ;
    }

    //SnapCurrentWindow2Bottom
    const std::string SnapCurrentWindow2Bottom::sname() noexcept {
        return "snap_current_window_to_bottom" ;
    }

    void SnapCurrentWindow2Bottom::sprocess(
            bool first_call,
            unsigned int UNUSED(repeat_num),
            KeycodeLogger* const UNUSED(parent_keycodelgr),
            const CharLogger* const UNUSED(parent_charlgr)) {
        if(!first_call) return ;

        auto calc_half_size = [] (RECT& rect, const RECT& mrect) {
            rect.left   = mrect.left ;
            rect.top    = mrect.top + screenmetrics::height(mrect) / 2 ;
            rect.right  = mrect.right ;
            rect.bottom = mrect.bottom ;
        } ;

        auto next_monitor_pos = [] (const RECT& rect) {
            return POINT{screenmetrics::center_x(rect), rect.bottom + 100} ;
        } ;

        snap_foreground_window(calc_half_size, next_monitor_pos) ;
    }
}
