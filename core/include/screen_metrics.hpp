#ifndef _SCREEN_METRICS_HPP
#define _SCREEN_METRICS_HPP

#include <windows.h>
#include <sstream>
#include <string>
#include <cmath>
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

    inline auto center_x(const RECT& rect) noexcept {
        return rect.left + width(rect) / 2 ;
    }

    inline auto center_y(const RECT& rect) noexcept {
        return rect.top + height(rect) / 2 ;
    }

    inline auto l1_distance(const RECT& lhs, const RECT& rhs) noexcept {
        return std::abs(center_x(rhs) - center_x(lhs)) \
            + std::abs(center_y(rhs) - center_y(lhs)) ;
    }

    inline auto l2_distance_nosq(const RECT& lhs, const RECT& rhs) noexcept {
        auto delta_x = center_x(rhs) - center_x(lhs) ;
        auto delta_y = center_y(rhs) - center_y(lhs) ;
        return delta_x*delta_x + delta_y*delta_y ;
    }
    inline auto l2_distance_nosq(LONG x1, LONG y1, LONG x2, LONG y2) noexcept {
        auto delta_x = x2 - x1 ;
        auto delta_y = y2 - y1 ;
        return delta_x*delta_x + delta_y*delta_y ;
    }

    inline auto l2_distance(const RECT& lhs, const RECT& rhs) noexcept {
        return std::sqrt(l2_distance_nosq(rhs, lhs)) ;
    }
    inline auto l2_distance(LONG x1, LONG y1, LONG x2, LONG y2) noexcept {
        return std::sqrt(l2_distance_nosq(x1, y1, x2, y2)) ;
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

    inline auto is_equel(const RECT& lhs, const RECT& rhs) noexcept {
        return lhs.left   == rhs.left && \
               lhs.top    == rhs.top && \
               lhs.right  == rhs.right && \
               lhs.bottom == rhs.bottom ;
    }

    inline auto is_bigger_tran(const RECT& lhs, const RECT& rhs) noexcept {
        return lhs.left <= rhs.left && lhs.top <= rhs.top &&
               lhs.right >= rhs.right && lhs.bottom >= rhs.bottom ;
    }

    inline void get_conbined_metrics(RECT* const rect) {
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
        const auto hmonitor = MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST) ;
        MONITORINFO minfo ;
        minfo.cbSize = sizeof(MONITORINFO) ;
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
