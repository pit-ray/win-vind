#ifndef _SCREEN_METRICS_HPP
#define _SCREEN_METRICS_HPP

#define _WIN32_WINNT_WIN10 0x0A00 //Windows 10

//for DPI support
#if !defined(WINVER)

#define WINVER        _WIN32_WINNT_WIN10
#define _WIN32_WINNT  _WIN32_WINNT_WIN10

#elif WINVER < _WIN32_WINNT_WIN10

#define WINVER        _WIN32_WINNT_WIN10
#define _WIN32_WINNT  _WIN32_WINNT_WIN10

#endif

#include <windows.h>
#include <sstream>
#include <string>
#include "utility.hpp"

namespace ScreenMetrics {

    inline void copy(RECT& lhs, const RECT& rhs) noexcept {
        lhs.left   = rhs.left ;
        lhs.top    = rhs.top ;
        lhs.right  = rhs.right ;
        lhs.bottom = rhs.bottom ;
    }

    inline auto width(const RECT& rect) noexcept {
        return rect.right - rect.left ;
    }

    inline auto height(const RECT& rect) noexcept {
        return rect.bottom - rect.top ;
    }

    inline auto is_out_of_range(const RECT& target, const RECT& range) noexcept {
        return (target.left <= range.left   &&  target.right <= range.left) || \
               (target.top  <= range.top    && target.bottom <= range.top) || \
               (target.left >= range.right  && target.right  >= range.right) || \
               (target.top  >= range.bottom && target.bottom >= range.bottom) ;
    }

    inline auto is_fully_in_range(const RECT& target, const RECT& range) noexcept {
        return target.left >= range.left && target.right <= range.right && \
                              target.top >= range.top && target.bottom <= range.bottom ;
    }

    inline auto is_in_range(const RECT& target, const RECT& range) noexcept {
        return target.left >= range.left || target.right <= range.right || \
                              target.top >= range.top || target.bottom <= range.bottom ;
    }

    inline auto is_equel(const RECT& rhs, const RECT& lhs) noexcept {
        return lhs.left   == rhs.left && \
               lhs.top    == rhs.top && \
               lhs.right  == rhs.right && \
               lhs.bottom == rhs.bottom ;
    }

    inline void get_conbined_metrics(RECT* const rect) {
        if(!SetProcessDPIAware()) {
            throw RUNTIME_EXCEPT("Your system is not supported DPI.") ;
        }

        WINDOWINFO winfo ;
        winfo.cbSize = sizeof(WINDOWINFO) ;
        if(!GetWindowInfo(GetDesktopWindow(), &winfo)) {
            throw RUNTIME_EXCEPT("Could not get window infomation.") ;
        }
        copy(*rect, winfo.rcWindow) ;
    }

    inline void get_primary_metrics(RECT* const rect) {
        MONITORINFO minfo ;
        minfo.cbSize = sizeof(MONITORINFO) ;
        const auto hmonitor = MonitorFromWindow(GetDesktopWindow(), MONITOR_DEFAULTTOPRIMARY) ;
        if(!GetMonitorInfo(hmonitor, &minfo)) {
            throw RUNTIME_EXCEPT("Could not get primary monitor infomation.") ;
        }
        copy(*rect, minfo.rcMonitor) ;
    }

    inline void get_monitor_metrics(HWND hwnd, RECT* const rect, RECT* const work_rect=NULL, HMONITOR* monitor=NULL) {
        MONITORINFO minfo ;
        minfo.cbSize = sizeof(MONITORINFO) ;
        const auto hmonitor = MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST) ;
        if(!GetMonitorInfo(hmonitor, &minfo)) {
            throw RUNTIME_EXCEPT("Could not get monitor infomation.") ;
        }

        copy(*rect, minfo.rcMonitor) ;

        if(work_rect != NULL) {
            copy(*work_rect, minfo.rcWork) ;
        }
        if(monitor != NULL) {
            *monitor = hmonitor ;
        }
    }

    namespace Debug {
        inline static std::string info(const RECT& rect) {
            std::stringstream ss ;
            ss << "(" << rect.left << ", " << rect.top << ")\t" ;
            ss << "(" << rect.right << ", " << rect.bottom << ")\t" ;
            ss << "w: " << width(rect) << ", h:" << height(rect) ;
            return ss.str() ;
        }
    }
}

#endif
