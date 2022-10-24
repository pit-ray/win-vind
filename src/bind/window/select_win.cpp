#include "select_win.hpp"

#include <map>

#include "winutil.hpp"

#include "bind/mouse/jump_actwin.hpp"
#include "core/ntypelogger.hpp"
#include "util/box2d.hpp"
#include "util/rect.hpp"
#include "util/screen_metrics.hpp"
#include "util/winwrap.hpp"

namespace
{
    using namespace vind ;

    std::unordered_map<HWND, util::Box2D> g_rects ;
    BOOL CALLBACK EnumWindowsProcForNearest(HWND hwnd, LPARAM lparam) {
        auto self_hwnd = reinterpret_cast<HWND>(lparam) ;
        if(self_hwnd == hwnd) {
            return TRUE ;
        }

        if(!bind::is_visible_hwnd(hwnd)) {
            return TRUE ;
        }

        util::Box2D rect ;
        if(!GetWindowRect(hwnd, &(rect.data()))) {
            return TRUE ;
        }

        if(!bind::is_window_mode(hwnd, rect.data())) {
            return TRUE ;
        }

        util::MonitorInfo minfo ;
        util::get_monitor_metrics(hwnd, minfo) ;

        if(rect.is_out_of(minfo.work_rect)) {
            return TRUE ;
        }

        g_rects[hwnd] = std::move(rect) ;
        return TRUE ;
    }

    template <typename T1, typename T2>
    inline void select_nearest_window(
            T1&& is_if_target,
            T2&& calc_distance) {
        auto fg_hwnd = util::get_foreground_window() ;

        g_rects.clear() ;
        if(!EnumWindows(EnumWindowsProcForNearest,
                    reinterpret_cast<LPARAM>(fg_hwnd))) {

            throw RUNTIME_EXCEPT("Could not enumerate all top-level windows.") ;
        }

        auto fg_rect = util::get_window_rect(fg_hwnd) ;

        std::map<LONG, HWND> distance_order_hwnd ;
        for(const auto& [enu_hwnd, enu_rect] : g_rects) {
            if(is_if_target(fg_rect, enu_rect)) {
                auto distance = calc_distance(fg_rect, enu_rect) ;
                distance_order_hwnd[distance] = enu_hwnd ;
            }
        }

        if(!distance_order_hwnd.empty()) {
            auto nearest_hwnd = distance_order_hwnd.begin()->second ;
            util::set_foreground_window(nearest_hwnd) ;
            Sleep(50) ;
            bind::JumpToActiveWindow::sprocess(1, "") ;
        }
    }
}


namespace vind
{
    namespace bind
    {
        //SelectLeftWindow
        SelectLeftWindow::SelectLeftWindow()
        : BindedFuncVoid("select_left_window")
        {}
        void SelectLeftWindow::sprocess(
                std::uint16_t UNUSED(count),
                const std::string& UNUSED(args)) {
            auto is_if_target = [] (const auto& rect, const auto& erect) {
                return rect.center_x() >= erect.center_x() ;
            } ;

            auto calc_distance = [] (const auto& rect, const auto& erect) {
                return util::l2_distance_nosq(
                        erect.center_x(), erect.center_y(),
                        rect.left(), rect.center_y()) / 100 ;
            } ;

            select_nearest_window(is_if_target, calc_distance) ;
        }

        //SelectRightWindow
        SelectRightWindow::SelectRightWindow()
        : BindedFuncVoid("select_right_window")
        {}
        void SelectRightWindow::sprocess(
                std::uint16_t UNUSED(count),
                const std::string& UNUSED(args)) {
            auto is_if_target = [] (const auto& rect, const auto& erect) {
                return rect.center_x() <= erect.center_x() ;
            } ;

            auto calc_distance = [] (const auto& rect, const auto& erect) {
                return util::l2_distance_nosq(
                        erect.center_x(), erect.center_y(),
                        rect.right(), rect.center_y()) / 100 ;
            } ;

            select_nearest_window(is_if_target, calc_distance) ;
        }

        //SelectUpperWindow
        SelectUpperWindow::SelectUpperWindow()
        : BindedFuncVoid("select_upper_window")
        {}
        void SelectUpperWindow::sprocess(
                std::uint16_t UNUSED(count),
                const std::string& UNUSED(args)) {
            auto is_if_target = [] (const auto& rect, const auto& erect) {
                return rect.center_y() >= erect.center_y() ;
            } ;

            auto calc_distance = [] (const auto& rect, const auto& erect) {
                return util::l2_distance_nosq(
                        erect.center_x(), erect.center_y(),
                        rect.center_x(), rect.top()) / 100 ;
            } ;

            select_nearest_window(is_if_target, calc_distance) ;
        }

        //SelectLowerWindow
        SelectLowerWindow::SelectLowerWindow()
        : BindedFuncVoid("select_lower_window")
        {}
        void SelectLowerWindow::sprocess(
                std::uint16_t UNUSED(count),
                const std::string& UNUSED(args)) {
            auto is_if_target = [] (const auto& rect, const auto& erect) {
                return rect.center_y() <= erect.center_y() ;
            } ;

            auto calc_distance = [] (const auto& rect, const auto& erect) {
                return util::l2_distance_nosq(
                        erect.center_x(), erect.center_y(),
                        rect.center_x(), rect.bottom()) / 100 ;
            } ;

            select_nearest_window(is_if_target, calc_distance) ;
        }
    }
}
