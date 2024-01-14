#include "move_win.hpp"

#include "core/settable.hpp"
#include "util/debug.hpp"
#include "util/screen_metrics.hpp"
#include "util/winwrap.hpp"

#include <algorithm>


namespace vind
{
    namespace bind
    {
        MoveWindowLeft::MoveWindowLeft()
        : BindedFuncVoid("move_window_left")
        {}

        void MoveWindowLeft::sprocess(
                std::uint16_t count,
                const std::string& UNUSED(args)) {
            auto hwnd = util::get_foreground_window() ;
            if(!hwnd) {
                return ;
            }

            auto delta = core::SetTable::get_instance().get("window_velocity").get<long>() ;
            delta *= static_cast<long>(count) ;

            auto rect = util::get_window_rect(hwnd) ;
            auto cb_rect = util::get_combined_metrics() ;

            auto left = (std::max)(rect.left() - delta, cb_rect.left()) ;
            auto top = rect.top() ;
            auto width = rect.width() ;
            auto height = rect.height() ;

            util::move_window(hwnd, left, top, width, height) ;
        }

        MoveWindowRight::MoveWindowRight()
        : BindedFuncVoid("move_window_right")
        {}

        void MoveWindowRight::sprocess(
                std::uint16_t count,
                const std::string& UNUSED(args)) {
            auto hwnd = util::get_foreground_window() ;
            if(!hwnd) {
                return ;
            }

            auto delta = core::SetTable::get_instance().get("window_velocity").get<long>() ;
            delta *= static_cast<long>(count) ;

            auto rect = util::get_window_rect(hwnd) ;
            auto width = rect.width() ;
            auto height = rect.height() ;

            auto cb_rect = util::get_combined_metrics() ;
            auto max_x = (std::max)(cb_rect.right() - width, static_cast<long>(0)) ;

            auto left = (std::min)(rect.left() + delta, max_x) ;
            auto top = rect.top() ;

            util::move_window(hwnd, left, top, width, height) ;
        }

        MoveWindowUp::MoveWindowUp()
        : BindedFuncVoid("move_window_up")
        {}

        void MoveWindowUp::sprocess(
                std::uint16_t count,
                const std::string& UNUSED(args)) {
            auto hwnd = util::get_foreground_window() ;
            if(!hwnd) {
                return ;
            }

            auto delta = core::SetTable::get_instance().get("window_velocity").get<long>() ;
            delta *= static_cast<long>(count) ;

            auto rect = util::get_window_rect(hwnd) ;
            auto cb_rect = util::get_combined_metrics() ;

            auto left = rect.left() ;
            auto top = (std::max)(rect.top() - delta, cb_rect.top()) ;
            auto width = rect.width() ;
            auto height = rect.height() ;

            util::move_window(hwnd, left, top, width, height) ;
        }

        MoveWindowDown::MoveWindowDown()
        : BindedFuncVoid("move_window_down")
        {}

        void MoveWindowDown::sprocess(
                std::uint16_t count,
                const std::string& UNUSED(args)) {
            auto hwnd = util::get_foreground_window() ;
            if(!hwnd) {
                return ;
            }

            auto delta = core::SetTable::get_instance().get("window_velocity").get<long>() ;
            delta *= static_cast<long>(count) ;

            auto rect = util::get_window_rect(hwnd) ;
            auto width = rect.width() ;
            auto height = rect.height() ;

            auto cb_rect = util::get_combined_metrics() ;
            auto max_y = (std::max)(cb_rect.bottom() - height, static_cast<long>(0)) ;

            auto left = rect.left() ;
            auto top = (std::min)(rect.top() + delta, max_y) ;

            util::move_window(hwnd, left, top, width, height) ;
        }
    }
}
