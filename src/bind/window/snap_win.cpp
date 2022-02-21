#include "snap_win.hpp"

#include <functional>

#include "winutil.hpp"

#include "core/ntypelogger.hpp"
#include "util/box2d.hpp"
#include "util/def.hpp"
#include "util/screen_metrics.hpp"
#include "util/winwrap.hpp"

namespace
{
    using namespace vind ;
    inline void snap_foreground_window(
            const std::function<util::Box2D(const util::Box2D&)>& calc_half_size,
            const std::function<POINT(const util::Box2D&)>& next_monitor_pos) {

        auto hwnd = util::get_foreground_window() ;

        util::MonitorInfo minfo ;
        util::get_monitor_metrics(hwnd, minfo) ;

        auto half_rect = calc_half_size(minfo.work_rect) ;

        auto cur_rect = util::get_window_rect(hwnd) ;
        if(cur_rect == half_rect) {
            util::get_monitor_metrics(next_monitor_pos(minfo.rect), minfo) ;

            half_rect = calc_half_size(minfo.work_rect) ;
        }

        bind::resize_window(hwnd, half_rect) ;
    }
}

namespace vind
{
    namespace bind
    {
        //SnapCurrentWindow2Left
        SnapCurrentWindow2Left::SnapCurrentWindow2Left()
        : BindedFuncVoid("snap_current_window_to_left")
        {}

        void SnapCurrentWindow2Left::sprocess() {
            auto calc_half_size = [] (const util::Box2D& mrect) {
                return util::Box2D{
                    mrect.left(),
                    mrect.top(),
                    mrect.center_x(),
                    mrect.bottom()
                } ;
            } ;

            auto next_monitor_pos = [] (const util::Box2D& rect) {
                return POINT{rect.left() - 100, rect.center_y()} ;
            } ;

            snap_foreground_window(calc_half_size, next_monitor_pos) ;
        }
        void SnapCurrentWindow2Left::sprocess(core::NTypeLogger& parent_lgr) {
            if(!parent_lgr.is_long_pressing()) {
                sprocess() ;
            }
        }
        void SnapCurrentWindow2Left::sprocess(const core::CharLogger& UNUSED(parent_lgr)) {
            sprocess() ;
        }


        //SnapCurrentWindow2Right
        SnapCurrentWindow2Right::SnapCurrentWindow2Right()
        : BindedFuncVoid("snap_current_window_to_right")
        {}
        void SnapCurrentWindow2Right::sprocess() {
            auto calc_half_size = [] (const util::Box2D& mrect) {
                return util::Box2D {
                    mrect.center_x(),
                    mrect.top(),
                    mrect.right(),
                    mrect.bottom()
                } ;
            } ;

            auto next_monitor_pos = [] (const util::Box2D& rect) {
                return POINT{rect.right() + 100, rect.center_y()} ;
            } ;

            snap_foreground_window(calc_half_size, next_monitor_pos) ;
        }
        void SnapCurrentWindow2Right::sprocess(core::NTypeLogger& parent_lgr) {
            if(!parent_lgr.is_long_pressing()) {
                sprocess() ;
            }
        }
        void SnapCurrentWindow2Right::sprocess(const core::CharLogger& UNUSED(parent_lgr)) {
            sprocess() ;
        }


        //SnapCurrentWindow2Top
        SnapCurrentWindow2Top::SnapCurrentWindow2Top()
        : BindedFuncVoid("snap_current_window_to_top")
        {}
        void SnapCurrentWindow2Top::sprocess() {
            auto calc_half_size = [] (const util::Box2D& mrect) {
                return util::Box2D {
                    mrect.left(),
                    mrect.top(),
                    mrect.right(),
                    mrect.center_y()
                } ;
            } ;

            auto next_monitor_pos = [] (const util::Box2D& rect) {
                return POINT{rect.center_x(), rect.top() - 100} ;
            } ;

            snap_foreground_window(calc_half_size, next_monitor_pos) ;
        }
        void SnapCurrentWindow2Top::sprocess(core::NTypeLogger& parent_lgr) {
            if(!parent_lgr.is_long_pressing()) {
                sprocess() ;
            }
        }
        void SnapCurrentWindow2Top::sprocess(const core::CharLogger& UNUSED(parent_lgr)) {
            sprocess() ;
        }


        //SnapCurrentWindow2Bottom
        SnapCurrentWindow2Bottom::SnapCurrentWindow2Bottom()
        : BindedFuncVoid("snap_current_window_to_bottom")
        {}
        void SnapCurrentWindow2Bottom::sprocess() {
            auto calc_half_size = [] (const util::Box2D& mrect) {
                return util::Box2D {
                    mrect.left(),
                    mrect.center_y(),
                    mrect.right(),
                    mrect.bottom()
                } ;
            } ;

            auto next_monitor_pos = [] (const util::Box2D& rect) {
                return POINT{rect.center_x(), rect.bottom() + 100} ;
            } ;

            snap_foreground_window(calc_half_size, next_monitor_pos) ;
        }
        void SnapCurrentWindow2Bottom::sprocess(core::NTypeLogger& parent_lgr) {
            if(!parent_lgr.is_long_pressing()) {
                sprocess() ;
            }
        }
        void SnapCurrentWindow2Bottom::sprocess(const core::CharLogger& UNUSED(parent_lgr)) {
            sprocess() ;
        }
    }
}
