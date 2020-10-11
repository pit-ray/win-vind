#ifndef _SCREEN_METRICS_HPP
#define _SCREEN_METRICS_HPP

//for DPI support
#define _WIN32_WINNT_WIN10 0x0A00 //Windows 10
#define WINVER          _WIN32_WINNT_WIN10
#define _WIN32_WINNT    _WIN32_WINNT_WIN10

#include <windows.h>
#include <winuser.h>
#include "msg_logger.hpp"
#include "utility.hpp"

class ScreenMetrics {
private:
    LONG w ;
    LONG h ;
    LONG pw ;
    LONG ph ;

public:
    explicit ScreenMetrics() : w(0), h(0), pw(0), ph() {
        calibrate() ;
    }

    void calibrate() {
        if(!SetProcessDPIAware()) {
            throw RUNTIME_EXCEPT("SetProcessDPIAware") ;
        }

        MONITORINFO minfo ;
        minfo.cbSize = sizeof(MONITORINFO) ;
        const auto hmonitor = MonitorFromWindow(GetDesktopWindow(), MONITOR_DEFAULTTOPRIMARY) ;
        GetMonitorInfo(hmonitor, &minfo) ;
        pw = minfo.rcMonitor.right - minfo.rcMonitor.left ;
        ph = minfo.rcMonitor.bottom - minfo.rcMonitor.top ;

        WINDOWINFO winfo ;
        winfo.cbSize = sizeof(WINDOWINFO) ;
        GetWindowInfo(GetDesktopWindow(), &winfo) ;
        w = winfo.rcWindow.right - winfo.rcWindow.left ;
        h = winfo.rcWindow.bottom - winfo.rcWindow.top ;
    }
    const auto width() const noexcept {
        return w ;
    }
    const auto height() const noexcept {
        return h ;
    }
    const auto primary_width() const noexcept {
        return pw ;
    }
    const auto primary_height() const noexcept {
        return ph ;
    }

    virtual ~ScreenMetrics() noexcept                       = default ;
    ScreenMetrics(ScreenMetrics&&) noexcept                 = default ;
    ScreenMetrics& operator=(ScreenMetrics&&) noexcept      = default ;
    ScreenMetrics(const ScreenMetrics&) noexcept            = default ;
    ScreenMetrics& operator=(const ScreenMetrics&) noexcept = default ;
} ;

#endif
