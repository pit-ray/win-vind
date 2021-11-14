#include "window_utility.hpp"

#include <dwmapi.h>

#include "bind/mouse/jump_actwin.hpp"
#include "util/def.hpp"
#include "util/keybrd.hpp"
#include "util/screen_metrics.hpp"
#include "util/winwrap.hpp"

namespace vind
{
    namespace bind
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

        bool is_window_mode(HWND hwnd, const RECT& rect) {
            if(hwnd == NULL) {
                throw RUNTIME_EXCEPT("There is not a foreground window.") ;
            }

            //Is having title bar ?
            if(GetWindowTextLengthW(hwnd) == 0) {
                return false ;
            }

            if(util::width(rect) == 0 || util::height(rect) == 0) {
                return false ;
            }

            //is full screen window ??
            RECT client_rect ;
            if(!GetClientRect(hwnd, &client_rect)) {
                return false ;
            }
            if(util::is_equal(rect, client_rect)) {
                return false ;
            }

            return true ;
        }

        void resize_window(
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

            util::Box2D rect ;
            if(!GetWindowRect(hwnd, &(rect.data()))) {
                throw RUNTIME_EXCEPT("Could not get a rectangle of a window.") ;
            }

            if(rect.width() != width || rect.height() != height) {
                //If a window is Chromium browser (e.g. GoogleChrome or Microsoft Edge) and when it is full screen,
                //could not resize its size, so cancel full screen.

                util::MonitorInfo minfo ;
                util::get_monitor_metrics(hwnd, minfo) ;

                //Whether it is a full screen ?
                if(!rect.is_over(minfo.work_rect)) {
                    return ;
                }

                util::set_foreground_window(hwnd) ;

                //minimize it once
                util::pushup(KEYCODE_LWIN, KEYCODE_DOWN) ;
                Sleep(50) ; //50ms

                if(!MoveWindow(hwnd, left, top, width, height, TRUE)) {
                    throw RUNTIME_EXCEPT("Could not change window size in twice.") ;
                }
            }

            JumpToActiveWindow::sprocess() ;
        }

        void batch_resize(const std::unordered_map<HWND, util::Box2D>& rects) {
            //Resize each windows
            for(const auto& [hwnd, rect] : rects) {
                resize_window(hwnd, rect) ;
            }
        }
        void batch_resize(const std::unordered_map<HWND, RECT>& rects) {
            //Resize each windows
            for(const auto& [hwnd, rect] : rects) {
                resize_window(hwnd, rect) ;
            }
        }
    }
}
