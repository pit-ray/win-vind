#include "bind/window/rotate_window.hpp"

#include <cmath>
#include <functional>
#include <map>
#include <memory>
#include <sstream>
#include <unordered_map>
#include <utility>

#include "io/screen_metrics.hpp"
#include "bind/mouse/jump_actwin.hpp"
#include "util/def.hpp"
#include "bind/window/window_utility.hpp"

namespace {
    using namespace vind ;

    using ordered_hwnd_t = std::map<float, HWND> ;
    using ordered_rect_t = std::map<float, std::unique_ptr<RECT>> ;

    struct RotEnumArgs {
        ordered_hwnd_t angle_hwnds{} ;
        ordered_rect_t angle_rects{} ;
        HMONITOR hmonitor = NULL ;
    } ;

    BOOL CALLBACK EnumWindowsProcForRotation(HWND hwnd, LPARAM lparam) {
        if(!windowutil::is_visible_hwnd(hwnd)) {
            return TRUE ;
        }

        auto prect = std::make_unique<RECT>() ;
        if(!GetWindowRect(hwnd, prect.get())) {
            return TRUE ; //continue
        }

        if(!windowutil::is_window_mode(hwnd, *prect)) {
            return TRUE ; //continue
        }

        auto p_args = reinterpret_cast<RotEnumArgs*>(lparam) ;

        screenmetrics::MonitorInfo minfo ;
        screenmetrics::get_monitor_metrics(hwnd, minfo) ;

        //search only in the same monitor as a foreground window.
        if(minfo.hmonitor != p_args->hmonitor) {
            return TRUE ;
        }

        //Is existed in work area?
        if(screenmetrics::is_out_of_range(*prect, minfo.work_rect)) {
            return TRUE ;
        }

        const auto x = screenmetrics::center_x(*prect) - screenmetrics::center_x(minfo.work_rect) ;
        const auto y = screenmetrics::center_y(minfo.work_rect) - screenmetrics::center_y(*prect) ;

        if(x == 0 && y == 0) {
            p_args->angle_hwnds[0.0f] = hwnd ;
            p_args->angle_rects[0.0f] = std::move(prect) ;
        }
        else {
            auto angle = static_cast<float>(std::atan2(y, x)) ;
            p_args->angle_hwnds[angle] = hwnd ;
            p_args->angle_rects[angle] = std::move(prect) ;
        }

        return TRUE ;
    }

    inline void rotate_windows_core(
            const std::function<void(ordered_hwnd_t&)>& sort_func) {

        windowutil::ForegroundInfo fginfo ;

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

            windowutil::resize(
                    hwnd,
                    prect->left, prect->top,
                    screenmetrics::width(*prect),
                    screenmetrics::height(*prect)) ;
        }

        if(!SetForegroundWindow(fginfo.hwnd)) {
            std::stringstream ss ;
            ss << "Could not set " << fginfo.hwnd << " as a foreground window." ;
            throw RUNTIME_EXCEPT(ss.str()) ;
        }
    }
}

namespace vind
{
    //RotateWindow
    const std::string RotateWindows::sname() noexcept {
        return "rotate_windows" ;
    }

    void RotateWindows::sprocess(
            bool first_call,
            unsigned int repeat_num,
            KeycodeLogger* const UNUSED(parent_keycodelgr),
            const CharLogger* const UNUSED(parent_charlgr)) {
        if(!first_call) return ;

        auto sort = [repeat_num] (ordered_hwnd_t& angle_hwnds) {
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

        rotate_windows_core(sort) ;
    }

    //RotateWindowsInReverse
    const std::string RotateWindowsInReverse::sname() noexcept {
        return "rotate_windows_in_reverse" ;
    }

    void RotateWindowsInReverse::sprocess(
            bool first_call,
            unsigned int repeat_num,
            KeycodeLogger* const UNUSED(parent_keycodelgr),
            const CharLogger* const UNUSED(parent_charlgr)) {
        if(!first_call) return ;

        auto sort = [repeat_num] (ordered_hwnd_t& angle_hwnds) {
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

        rotate_windows_core(sort) ;
    }
}
