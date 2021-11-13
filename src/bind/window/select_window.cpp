#include "select_window.hpp"

#include <map>

#include "window_utility.hpp"

#include "bind/mouse/jump_actwin.hpp"
#include "core/ntype_logger.hpp"
#include "util/box_2d.hpp"
#include "util/rect.hpp"
#include "util/screen_metrics.hpp"

namespace
{
    using namespace vind ;

    std::unordered_map<HWND, util::Box2D> g_rects ;
    BOOL CALLBACK EnumWindowsProcForNearest(HWND hwnd, LPARAM lparam) {
        auto self_hwnd = reinterpret_cast<HWND>(lparam) ;
        if(self_hwnd == hwnd) {
            return TRUE ;
        }

        if(!windowutil::is_visible_hwnd(hwnd)) {
            return TRUE ;
        }

        util::Box2D rect ;
        if(!GetWindowRect(hwnd, &(rect.data()))) {
            return TRUE ;
        }

        if(!windowutil::is_window_mode(hwnd, rect.data())) {
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
        auto fg_hwnd = GetForegroundWindow() ;
        if(fg_hwnd == NULL) {
            throw RUNTIME_EXCEPT("There is not a foreground window.") ;
        }

        g_rects.clear() ;
        if(!EnumWindows(EnumWindowsProcForNearest,
                    reinterpret_cast<LPARAM>(fg_hwnd))) {

            throw RUNTIME_EXCEPT("Could not enumerate all top-level windows.") ;
        }

        util::Box2D fg_rect ;
        if(!GetWindowRect(fg_hwnd, &(fg_rect.data()))) {
            throw RUNTIME_EXCEPT("Could not get a rectangle of a foreground window.") ;
        }

        std::map<LONG, HWND> distance_order_hwnd ;
        for(const auto& [enu_hwnd, enu_rect] : g_rects) {
            if(is_if_target(fg_rect, enu_rect)) {
                auto distance = calc_distance(fg_rect, enu_rect) ;
                distance_order_hwnd[distance] = enu_hwnd ;
            }
        }

        if(!distance_order_hwnd.empty()) {
            auto nearest_hwnd = distance_order_hwnd.begin()->second ;
            if(!SetForegroundWindow(nearest_hwnd)) {
                throw RUNTIME_EXCEPT("Could not set a foreground window.") ;
            }
            Sleep(50) ;
            JumpToActiveWindow::sprocess() ;
        }
    }
}


namespace vind
{
    //SelectLeftWindow
    SelectLeftWindow::SelectLeftWindow()
    : BindedFuncCreator("select_left_window")
    {}
    void SelectLeftWindow::sprocess() {
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
    void SelectLeftWindow::sprocess(core::NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess() ;
        }
    }
    void SelectLeftWindow::sprocess(const core::CharLogger& UNUSED(parent_lgr)) {
        sprocess() ;
    }


    //SelectRightWindow
    SelectRightWindow::SelectRightWindow()
    : BindedFuncCreator("select_right_window")
    {}
    void SelectRightWindow::sprocess() {
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
    void SelectRightWindow::sprocess(core::NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess() ;
        }
    }
    void SelectRightWindow::sprocess(const core::CharLogger& UNUSED(parent_lgr)) {
        sprocess() ;
    }


    //SelectUpperWindow
    SelectUpperWindow::SelectUpperWindow()
    : BindedFuncCreator("select_upper_window")
    {}
    void SelectUpperWindow::sprocess() {
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
    void SelectUpperWindow::sprocess(core::NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess() ;
        }
    }
    void SelectUpperWindow::sprocess(const core::CharLogger& UNUSED(parent_lgr)) {
        sprocess() ;
    }


    //SelectLowerWindow
    SelectLowerWindow::SelectLowerWindow()
    : BindedFuncCreator("select_lower_window")
    {}
    void SelectLowerWindow::sprocess() {
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
    void SelectLowerWindow::sprocess(core::NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess() ;
        }
    }
    void SelectLowerWindow::sprocess(const core::CharLogger& UNUSED(parent_lgr)) {
        sprocess() ;
    }
}
