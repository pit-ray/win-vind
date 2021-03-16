#include "rotate_window.hpp"

#include <cmath>
#include <functional>
#include <map>
#include <memory>
#include <sstream>
#include <unordered_map>
#include <utility>

#include "jump_actwin.hpp"
#include "screen_metrics.hpp"
#include "utility.hpp"
#include "window_utility.hpp"

namespace RotateWindowCommon {
    using ordered_hwnd_t = std::map<float, HWND> ;
    using ordered_rect_t = std::map<float, std::unique_ptr<RECT>> ;

    struct RotEnumArgs {
        ordered_hwnd_t angle_hwnds{} ;
        ordered_rect_t angle_rects{} ;
        HMONITOR hmonitor = NULL ;
    } ;

    static BOOL CALLBACK EnumWindowsProcForRotation(HWND hwnd, LPARAM lparam) {
        if(!WindowUtility::is_visible_hwnd(hwnd)) {
            return TRUE ;
        }

        auto prect = std::make_unique<RECT>() ;
        if(!GetWindowRect(hwnd, prect.get())) {
            return TRUE ; //continue
        }

        if(!WindowUtility::is_window_mode(hwnd, *prect)) {
            return TRUE ; //continue
        }

        auto p_args = reinterpret_cast<RotEnumArgs*>(lparam) ;

        ScreenMetrics::MonitorInfo minfo ;
        ScreenMetrics::get_monitor_metrics(hwnd, minfo) ;

        //search only in the same monitor as a foreground window.
        if(minfo.hmonitor != p_args->hmonitor) {
            return TRUE ;
        }

        //Is existed in work area?
        if(ScreenMetrics::is_out_of_range(*prect, minfo.work_rect)) {
            return TRUE ;
        }

        const auto x = ScreenMetrics::center_x(*prect) - ScreenMetrics::center_x(minfo.work_rect) ;
        const auto y = ScreenMetrics::center_y(minfo.work_rect) - ScreenMetrics::center_y(*prect) ;

        if(x == 0 && y == 0) {
            p_args->angle_hwnds[0.0f] = hwnd ;
            p_args->angle_rects[0.0f] = std::move(prect) ;
        }
        else {
            auto angle = std::atan2(y, x) ;
            p_args->angle_hwnds[angle] = hwnd ;
            p_args->angle_rects[angle] = std::move(prect) ;
        }

        return TRUE ;
    }

    inline static void rotate_windows_core(
            const std::function<void(ordered_hwnd_t&)>& sort_func) {

        WindowUtility::ForegroundInfo fginfo ;

        RotEnumArgs args ;
        args.hmonitor = fginfo.hmonitor ;
        if(!EnumWindows(EnumWindowsProcForRotation, reinterpret_cast<LPARAM>(&args))) {
            throw RUNTIME_EXCEPT("Could not enumerate all top-level windows on the screen.") ;
        }

        if(args.angle_hwnds.empty()) {
            throw RUNTIME_EXCEPT("Cannot detect any windows.") ;
        }

        sort_func(args.angle_hwnds) ;

        for(const auto& aw : args.angle_hwnds) {
            auto& hwnd  = aw.second ;
            auto& prect = args.angle_rects[aw.first] ;

            WindowUtility::resize(
                    hwnd,
                    prect->left, prect->top,
                    ScreenMetrics::width(*prect),
                    ScreenMetrics::height(*prect)) ;
        }

        if(!SetForegroundWindow(fginfo.hwnd)) {
            std::stringstream ss ;
            ss << "Could not set " << fginfo.hwnd << " as a foreground window." ;
            throw RUNTIME_EXCEPT(ss.str()) ;
        }
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

    auto sort = [repeat_num] (RotateWindowCommon::ordered_hwnd_t& angle_hwnds) {
        for(unsigned int i = 0 ; i < repeat_num ; i ++) {
            auto itr     = angle_hwnds.rbegin() ;
            auto pre_itr = itr ;
            const auto last_hwnd = itr->second ;
            itr ++ ;

            while(itr != angle_hwnds.rend()) {
                pre_itr->second = itr->second ; //rotate-shift hwnd (counter-clockwise)
                pre_itr = itr ;
                itr ++ ;
            }
            pre_itr->second = last_hwnd ;
        }
    } ;

    RotateWindowCommon::rotate_windows_core(sort) ;
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
    if(!first_call) return ;

    auto sort = [repeat_num] (RotateWindowCommon::ordered_hwnd_t& angle_hwnds) {
        for(unsigned int i = 0 ; i < repeat_num ; i ++) {
            auto itr     = angle_hwnds.begin() ;
            auto pre_itr = itr ;
            const auto last_hwnd = itr->second ;
            itr ++ ;

            while(itr != angle_hwnds.end()) {
                pre_itr->second = itr->second ; //rotate-shift hwnd (clockwise)
                pre_itr = itr ;
                itr ++ ;
            }
            pre_itr->second = last_hwnd ;
        }
    } ;

    RotateWindowCommon::rotate_windows_core(sort) ;
}

