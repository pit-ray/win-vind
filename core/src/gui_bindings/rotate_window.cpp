#include "rotate_window.hpp"

#include <map>
#include <unordered_map>
#include <cmath>

#include "window_utility.hpp"
#include "screen_metrics.hpp"

namespace RotateWindowStatic {
    static std::map<float, HWND> g_angle_hwnds{} ;
    static std::unordered_map<HWND, RECT> g_hwnd_rects{} ;

    static BOOL CALLBACK EnumWindowsProcForRotation(HWND hwnd, LPARAM lparam) {
        if(!WindowUtility::is_visible_hwnd(hwnd)) {
            return TRUE ;
        }

        RECT rect ;
        if(!GetWindowRect(hwnd, &rect)) {
            return TRUE ; //continue
        }

        if(!WindowUtility::is_window_mode(hwnd, rect)) {
            return TRUE ; //continue
        }

        //Is existed in work area?
        ScreenMetrics::MonitorInfo minfo ;
        ScreenMetrics::get_monitor_metrics(hwnd, minfo) ;
        if(ScreenMetrics::is_out_of_range(rect, minfo.work_rect)) {
            return TRUE ;
        }

        const auto cx = ScreenMetrics::center_x(rect) ;
        const auto cy = ScreenMetrics::center_y(rect) ;

        if(cx == 0 && cy == 0) {
            g_angle_hwnds[0.0f] = hwnd ;
        }
        else {
            auto angle = std::atan2(
                    static_cast<float>(cx),
                    static_cast<float>(cy)) ;
            if(angle < 0) {
                angle = 3.14f + (-1)*angle ;
            }
            g_angle_hwnds[angle] = hwnd ;
        }

        g_hwnd_rects[hwnd] = std::move(rect) ;

        return TRUE ;
    }
}

//RotateWindow
const std::string RotateWindows::sname() noexcept
{
    return "rotate_windows" ;
}

void RotateWindows::sprocess(
        const bool first_call,
        const unsigned int repeat_num,
        VKCLogger* const UNUSED(parent_vkclgr),
        const CharLogger* const UNUSED(parent_charlgr))
{
    if(!first_call) return ;

    /*
    auto sort = [repeat_num] (ResizeWindow::ordered_hwnd_t& ordered_hwnd) {
        for(unsigned int i = 0 ; i < repeat_num ; i ++) {
            auto itr     = ordered_hwnd.rbegin() ;
            auto pre_itr = itr ;
            const auto head_hwnd = itr->second ;
            itr ++ ;
            while(itr != ordered_hwnd.rend()) {
                pre_itr->second = itr->second ;
                pre_itr = itr ;
                itr ++ ;
            }
            pre_itr->second = head_hwnd ;
        }
    } ;

    ResizeWindow::change_order_of_arranged_windows(sort) ;
    */
    using namespace RotateWindowStatic ;
    g_angle_hwnds.clear() ;
    g_hwnd_rects.clear() ;

    if(!EnumWindows(EnumWindowsProcForRotation, NULL)) {
        throw RUNTIME_EXCEPT("Could not enumerate all top-level windows on the screen.") ;
    }

    if(g_angle_hwnds.empty()) {
        throw RUNTIME_EXCEPT("Cannot detect any windows.") ;
    }

    for(unsigned int i = 0 ; i < repeat_num ; i ++) {
        auto itr     = g_angle_hwnds.rbegin() ;
        auto pre_itr = itr ;
        const auto last_hwnd = itr->second ;
        itr ++ ;

        std::cout << last_hwnd << ":\t" << pre_itr->first << "\t" << ScreenMetrics::Debug::info(g_hwnd_rects[last_hwnd]) << std::endl ;
        while(itr != g_angle_hwnds.rend()) {
            std::cout << itr->second << ":\t" << itr->first << "\t" << ScreenMetrics::Debug::info(g_hwnd_rects[itr->second]) << std::endl ;

            //itr->second = pre_itr->second ;
            pre_itr->second = itr->second ;
            pre_itr = itr ;
            itr ++ ;
        }
        pre_itr->second = last_hwnd ;
    }

    std::cout << "sorted-----\n" ;
    for(auto itr = g_angle_hwnds.rbegin() ; itr != g_angle_hwnds.rend() ; itr ++) {
        std::cout << itr->second << ":\t" << itr->first << "\t" << ScreenMetrics::Debug::info(g_hwnd_rects[itr->second]) << std::endl ;
    }

    std::unordered_map<HWND, RECT> rects ;
    for(const auto& aw : g_angle_hwnds) {
        rects[aw.second] = std::move(g_hwnd_rects[aw.second]) ;
    }
    WindowUtility::batch_resize(rects) ;
}

//RotateWindowsInReverse
const std::string RotateWindowsInReverse::sname() noexcept
{
    return "rotate_windows_in_reverse" ;
}

void RotateWindowsInReverse::sprocess(
        const bool first_call,
        const unsigned int repeat_num,
        VKCLogger* const UNUSED(parent_vkclgr),
        const CharLogger* const UNUSED(parent_charlgr))
{
    /*
    if(!first_call) return ;

    using namespace ResizeWindow::Arrange ;

    auto sort = [repeat_num] (ordered_hwnd_t& ordered_hwnd) {
        for(unsigned int i = 0 ; i < repeat_num ; i ++) {
            auto itr     = ordered_hwnd.begin() ;
            auto pre_itr = itr ;
            const auto head_hwnd = itr->second ;
            itr ++ ;
            while(itr != ordered_hwnd.end()) {
                pre_itr->second = itr->second ;
                pre_itr = itr ;
                itr ++ ;
            }
            pre_itr->second = head_hwnd ;
        }
    } ;

    change_order_of_arranged_windows(sort) ;
    */
}

