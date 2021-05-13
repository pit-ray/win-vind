#include "bind/window/select_window.hpp"

#include <map>

#include "bind/mouse/jump_actwin.hpp"
#include "bind/window/window_utility.hpp"
#include "io/screen_metrics.hpp"
#include "key/ntype_logger.hpp"

namespace
{
    using namespace vind ;

    std::unordered_map<HWND, RECT> g_rects ;
    BOOL CALLBACK EnumWindowsProcForNearest(HWND hwnd, LPARAM lparam) {
        auto self_hwnd = reinterpret_cast<HWND>(lparam) ;
        if(self_hwnd == hwnd) {
            return TRUE ;
        }

        if(!windowutil::is_visible_hwnd(hwnd)) {
            return TRUE ;
        }

        RECT rect ;
        if(!GetWindowRect(hwnd, &rect)) {
            return TRUE ;
        }

        if(!windowutil::is_window_mode(hwnd, rect)) {
            return TRUE ;
        }

        screenmetrics::MonitorInfo minfo ;
        screenmetrics::get_monitor_metrics(hwnd, minfo) ;

        if(screenmetrics::is_out_of_range(rect, minfo.work_rect)) {
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

        RECT fg_rect ;
        if(!GetWindowRect(fg_hwnd, &fg_rect)) {
            throw RUNTIME_EXCEPT("Could not get a rectangle of a foreground window.") ;
        }

        std::map<LONG, HWND> distance_order_hwnd ;
        for(const auto& enumed_rect : g_rects) {
            auto& enu_hwnd = enumed_rect.first ;
            auto& enu_rect = enumed_rect.second ;

            auto cx  = screenmetrics::center_x(fg_rect) ;
            auto cy  = screenmetrics::center_y(fg_rect) ;
            auto ecx = screenmetrics::center_x(enu_rect) ;
            auto ecy = screenmetrics::center_y(enu_rect) ;

            if(is_if_target(fg_rect, enu_rect, cx, cy, ecx, ecy)) {
                auto distance = calc_distance(fg_rect, enu_rect, cx, cy, ecx, ecy) ;
                distance_order_hwnd[distance] = enu_hwnd ;
            }
        }

        if(!distance_order_hwnd.empty()) {
            auto nearest_hwnd = distance_order_hwnd.begin()->second ;
            if(!SetForegroundWindow(nearest_hwnd)) {
                throw RUNTIME_EXCEPT("Could not set a foreground window.") ;
            }
            Sleep(50) ;
            Jump2ActiveWindow::sprocess() ;
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

            return screenmetrics::l2_distance_nosq(ecx, ecy, rect.left, cy) / 100 ;
        } ;

        select_nearest_window(is_if_target, calc_distance) ;
    }
    void SelectLeftWindow::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess() ;
        }
    }
    void SelectLeftWindow::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess() ;
    }


    //SelectRightWindow
    SelectRightWindow::SelectRightWindow()
    : BindedFuncCreator("select_right_window")
    {}
    void SelectRightWindow::sprocess() {
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

            return screenmetrics::l2_distance_nosq(ecx, ecy, rect.right, cy) / 100 ;
        } ;

        select_nearest_window(is_if_target, calc_distance) ;
    }
    void SelectRightWindow::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess() ;
        }
    }
    void SelectRightWindow::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess() ;
    }


    //SelectUpperWindow
    SelectUpperWindow::SelectUpperWindow()
    : BindedFuncCreator("select_upper_window")
    {}
    void SelectUpperWindow::sprocess() {
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

            return screenmetrics::l2_distance_nosq(ecx, ecy, cx, rect.top) / 100 ;
        } ;

        select_nearest_window(is_if_target, calc_distance) ;
    }
    void SelectUpperWindow::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess() ;
        }
    }
    void SelectUpperWindow::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess() ;
    }


    //SelectLowerWindow
    SelectLowerWindow::SelectLowerWindow()
    : BindedFuncCreator("select_lower_window")
    {}
    void SelectLowerWindow::sprocess() {
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

            return screenmetrics::l2_distance_nosq(ecx, ecy, cx, rect.bottom) / 100 ;
        } ;

        select_nearest_window(is_if_target, calc_distance) ;
    }
    void SelectLowerWindow::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess() ;
        }
    }
    void SelectLowerWindow::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess() ;
    }
}
