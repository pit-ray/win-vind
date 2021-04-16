#include "bind/window/snap_window.hpp"

#include <functional>

#include "io/screen_metrics.hpp"
#include "util/def.hpp"
#include "bind/window/window_utility.hpp"
#include "bind/base/ntype_logger.hpp"

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
    SnapCurrentWindow2Left::SnapCurrentWindow2Left()
    : BindedFuncCreator("snap_current_window_to_left")
    {}

    void SnapCurrentWindow2Left::sprocess() {
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
    void SnapCurrentWindow2Left::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess() ;
        }
    }
    void SnapCurrentWindow2Left::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess() ;
    }


    //SnapCurrentWindow2Right
    SnapCurrentWindow2Right::SnapCurrentWindow2Right()
    : BindedFuncCreator("snap_current_window_to_right")
    {}
    void SnapCurrentWindow2Right::sprocess() {
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
    void SnapCurrentWindow2Right::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess() ;
        }
    }
    void SnapCurrentWindow2Right::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess() ;
    }


    //SnapCurrentWindow2Top
    SnapCurrentWindow2Top::SnapCurrentWindow2Top()
    : BindedFuncCreator("snap_current_window_to_top")
    {}
    void SnapCurrentWindow2Top::sprocess() {
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
    void SnapCurrentWindow2Top::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess() ;
        }
    }
    void SnapCurrentWindow2Top::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess() ;
    }


    //SnapCurrentWindow2Bottom
    SnapCurrentWindow2Bottom::SnapCurrentWindow2Bottom()
    : BindedFuncCreator("snap_current_window_to_bottom")
    {}
    void SnapCurrentWindow2Bottom::sprocess() {
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
    void SnapCurrentWindow2Bottom::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess() ;
        }
    }
    void SnapCurrentWindow2Bottom::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess() ;
    }
}
