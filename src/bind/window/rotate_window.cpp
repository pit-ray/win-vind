#include "rotate_window.hpp"

#include <cmath>
#include <functional>
#include <map>
#include <memory>
#include <sstream>
#include <unordered_map>
#include <utility>

#include "window_utility.hpp"

#include "bind/mouse/jump_actwin.hpp"
#include "bind/safe_repeater.hpp"
#include "core/char_logger.hpp"
#include "core/ntype_logger.hpp"
#include "util/def.hpp"
#include "util/screen_metrics.hpp"

namespace {
    using namespace vind ;

    using AngleOrderedHWND = std::map<float, HWND> ;
    using AngleOrderedRect = std::map<float, util::Box2D> ;

    struct RotEnumArgs {
        AngleOrderedHWND angle_hwnds{} ;
        AngleOrderedRect angle_rects{} ;
        HMONITOR hmonitor = NULL ;
    } ;

    BOOL CALLBACK EnumWindowsProcForRotation(HWND hwnd, LPARAM lparam) {
        if(!windowutil::is_visible_hwnd(hwnd)) {
            return TRUE ;
        }

        util::Box2D rect{} ;
        if(!GetWindowRect(hwnd, &(rect.data()))) {
            return TRUE ; //continue
        }

        if(!windowutil::is_window_mode(hwnd, rect.data())) {
            return TRUE ; //continue
        }

        auto p_args = reinterpret_cast<RotEnumArgs*>(lparam) ;

        util::MonitorInfo minfo ;
        util::get_monitor_metrics(hwnd, minfo) ;

        //search only in the same monitor as a foreground window.
        if(minfo.hmonitor != p_args->hmonitor) {
            return TRUE ;
        }

        //Is existed in work area?
        if(rect.is_out_of(minfo.work_rect)) {
            return TRUE ;
        }

        auto x = rect.center_x() - minfo.work_rect.center_x() ;
        auto y = minfo.work_rect.center_y() - rect.center_y() ;

        if(x == 0 && y == 0) {
            p_args->angle_hwnds[0.0f] = hwnd ;
            p_args->angle_rects[0.0f] = std::move(rect) ;
        }
        else {
            auto angle = static_cast<float>(std::atan2(y, x)) ;
            p_args->angle_hwnds[angle] = hwnd ;
            p_args->angle_rects[angle] = std::move(rect) ;
        }

        return TRUE ;
    }

    template <typename FuncType>
    inline void rotate_windows_core(FuncType&& sort_func) {

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

        for(auto& [angle, hwnd] : args.angle_hwnds) {
            windowutil::resize(hwnd, args.angle_rects[angle]) ;
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
    RotateWindows::RotateWindows()
    : BindedFuncCreator("rotate_windows")
    {}
    void RotateWindows::sprocess(unsigned int repeat_num) {
        rotate_windows_core([repeat_num] (AngleOrderedHWND& angle_hwnds) {
            repeater::safe_for(repeat_num, [&angle_hwnds] {
                auto itr     = angle_hwnds.rbegin() ;
                auto pre_itr = itr ;
                auto last_hwnd = itr->second ;
                itr ++ ;

                while(itr != angle_hwnds.rend()) {
                    pre_itr->second = itr->second ; //rotate-shift hwnd (counter-clockwise)
                    pre_itr = itr ;
                    itr ++ ;
                }
                pre_itr->second = last_hwnd ;
            }) ;
        }) ;
    }
    void RotateWindows::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(parent_lgr.get_head_num()) ;
        }
    }
    void RotateWindows::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess(1) ;
    }


    //RotateWindowsInReverse
    RotateWindowsInReverse::RotateWindowsInReverse()
    : BindedFuncCreator("rotate_windows_in_reverse")
    {}
    void RotateWindowsInReverse::sprocess(unsigned int repeat_num) {
        rotate_windows_core([repeat_num] (AngleOrderedHWND& angle_hwnds) {
            repeater::safe_for(repeat_num, [&angle_hwnds] {
                auto itr     = angle_hwnds.begin() ;
                auto pre_itr = itr ;
                auto last_hwnd = itr->second ;
                itr ++ ;

                while(itr != angle_hwnds.end()) {
                    pre_itr->second = itr->second ; //rotate-shift hwnd (clockwise)
                    pre_itr = itr ;
                    itr ++ ;
                }
                pre_itr->second = last_hwnd ;
            }) ;
        }) ;
    }
    void RotateWindowsInReverse::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(parent_lgr.get_head_num()) ;
        }
    }
    void RotateWindowsInReverse::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess(1) ;
    }
}
