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

#include "msg_logger.hpp"
#include "utility.hpp"

class ScreenMetrics {
private:
    RECT virtual_size ;
    RECT primary_size ;
    RECT primary_client_size ;

public:
    explicit ScreenMetrics()
    : virtual_size(RECT{0, 0, 0, 0}),
      primary_size(RECT{0, 0, 0, 0}),
      primary_client_size(RECT{0, 0, 0, 0}) {

        calibrate() ;
    }

    void calibrate() {
        auto copy_rect = [] (auto& lhs, auto& rhs) {
            lhs.left   = rhs.left ;
            lhs.top    = rhs.top ;
            lhs.right  = rhs.right ;
            lhs.bottom = rhs.bottom ;
        } ;

        if(!SetProcessDPIAware()) {
            throw RUNTIME_EXCEPT("SetProcessDPIAware failed. Your system is not supported DPI on Windows10.") ;
        }

        MONITORINFO minfo ;
        minfo.cbSize = sizeof(MONITORINFO) ;
        const auto hmonitor = MonitorFromWindow(GetDesktopWindow(), MONITOR_DEFAULTTOPRIMARY) ;
        GetMonitorInfo(hmonitor, &minfo) ;
        copy_rect(primary_size, minfo.rcMonitor) ;
        copy_rect(primary_client_size, minfo.rcWork) ;

        WINDOWINFO winfo ;
        winfo.cbSize = sizeof(WINDOWINFO) ;
        GetWindowInfo(GetDesktopWindow(), &winfo) ;
        copy_rect(virtual_size, winfo.rcWindow) ;
    }
    auto width() const noexcept {
        return virtual_size.right - virtual_size.left ;
    }
    auto height() const noexcept {
        return virtual_size.bottom - virtual_size.top ;
    }
    auto left() const noexcept {
        return virtual_size.left ;
    }
    auto top() const noexcept {
        return virtual_size.top ;
    }
    auto right() const noexcept {
        return virtual_size.right ;
    }
    auto bottom() const noexcept {
        return virtual_size.bottom ;
    }

    auto primary_width() const noexcept {
        return primary_size.right - primary_size.left ;
    }
    auto primary_height() const noexcept {
        return primary_size.bottom - primary_size.top ;
    }
    auto primary_left() const noexcept {
        return primary_size.left ;
    }
    auto primary_top() const noexcept {
        return primary_size.top ;
    }
    auto primary_right() const noexcept {
        return primary_size.right ;
    }
    auto primary_bottom() const noexcept {
        return primary_size.bottom ;
    }

    auto primary_client_width() const noexcept {
        return primary_client_size.right - primary_client_size.left ;
    }
    auto primary_client_height() const noexcept {
        return primary_client_size.bottom - primary_client_size.top ;
    }
    auto primary_client_left() const noexcept {
        return primary_client_size.left ;
    }
    auto primary_client_top() const noexcept {
        return primary_client_size.top ;
    }
    auto primary_client_right() const noexcept {
        return primary_client_size.right ;
    }
    auto primary_client_bottom() const noexcept {
        return primary_client_size.bottom ;
    }

    virtual ~ScreenMetrics() noexcept                       = default ;
    ScreenMetrics(ScreenMetrics&&) noexcept                 = default ;
    ScreenMetrics& operator=(ScreenMetrics&&) noexcept      = default ;
    ScreenMetrics(const ScreenMetrics&) noexcept            = default ;
    ScreenMetrics& operator=(const ScreenMetrics&) noexcept = default ;
} ;

#endif
