#include "bind/window/window_utility.hpp"

#include <dwmapi.h>

#include "bind/mouse/jump_actwin.hpp"
#include "io/keybrd.hpp"
#include "io/screen_metrics.hpp"
#include "util/def.hpp"

namespace vind
{
    namespace windowutil
    {
        bool is_visible_hwnd(HWND hwnd) {
            if(hwnd == NULL) {
                throw RUNTIME_EXCEPT("There is not a foreground window.") ;
            }

            //is movable window ? -----------
            if(hwnd == GetDesktopWindow()) {
                return false ;
            }
            if(hwnd == GetShellWindow()) {
                return false ;
            }

            //Is visible ? ------------------
            if(!IsWindowEnabled(hwnd)) {
                return false ;
            }
            if(!IsWindowVisible(hwnd)) {
                return false ;
            }
            if(IsIconic(hwnd)) { //is minimized?
                return false ;
            }

            int n_cloaked ;
            if(DwmGetWindowAttribute(hwnd, DWMWA_CLOAKED, &n_cloaked, sizeof(int)) != S_OK) {
                return false ;
            }
            if(n_cloaked) {
                return false ;
            }
            return true ;
        }

        bool is_window_mode(HWND hwnd, RECT& rect) {
            if(hwnd == NULL) {
                throw RUNTIME_EXCEPT("There is not a foreground window.") ;
            }

            //Is having title bar ?
            if(GetWindowTextLengthW(hwnd) == 0) {
                return false ;
            }

            if(screenmetrics::width(rect) == 0) {
                return false ;
            }

            if(screenmetrics::height(rect) == 0) {
                return false ;
            }

            //is full screen window ??
            RECT client_rect ;
            if(!GetClientRect(hwnd, &client_rect)) {
                return false ;
            }
            if(screenmetrics::is_equel(rect, client_rect)) {
                return false ;
            }

            return true ;
        }

        void resize(
                HWND hwnd,
                LONG left,
                LONG top,
                LONG width,
                LONG height) {

            if(hwnd == NULL) {
                throw RUNTIME_EXCEPT("There is not a foreground window.") ;
            }

            if(!MoveWindow(hwnd, left, top, width, height, TRUE)) {
                throw RUNTIME_EXCEPT("Could not change window size") ;
            }

            RECT rect ;
            if(!GetWindowRect(hwnd, &rect)) {
                throw RUNTIME_EXCEPT("Could not get a rectangle of a window.") ;
            }

            if(screenmetrics::width(rect) != width || screenmetrics::height(rect) != height) {
                //If a window is Chromium browser (e.g. GoogleChrome or Microsoft Edge) and when it is full screen,
                //could not resize its size, so cancel full screen.

                screenmetrics::MonitorInfo minfo ;
                screenmetrics::get_monitor_metrics(hwnd, minfo) ;

                //Whether it is a full screen ?
                if(!screenmetrics::is_bigger_than(rect, minfo.work_rect)) {
                    return ;
                }

                if(!SetForegroundWindow(hwnd)) {
                    throw RUNTIME_EXCEPT("Could not set a foreground window.") ;
                }

                //minimize it once
                keybrd::pushup(KEYCODE_LWIN, KEYCODE_DOWN) ;
                Sleep(50) ; //50ms

                if(!MoveWindow(hwnd, left, top, width, height, TRUE)) {
                    throw RUNTIME_EXCEPT("Could not change window size in twice.") ;
                }
            }

            Jump2ActiveWindow::sprocess() ;
        }

        void batch_resize(const std::unordered_map<HWND, RECT>& rects) {
            //Resize each windows
            for(const auto& hr : rects) {
                const auto hwnd = hr.first ;
                const auto rect = hr.second ;
                resize(hwnd, rect.left, rect.top,
                        screenmetrics::width(rect), screenmetrics::height(rect)) ;
            }
        }
    }
}
