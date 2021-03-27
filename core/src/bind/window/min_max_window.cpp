#include "min_max_window.hpp"

#include "io/keybrd.hpp"
#include "io/screen_metrics.hpp"
#include "util/def.hpp"

namespace vind
{
    //MinimizeCurrentWindow
    const std::string MinimizeCurrentWindow::sname() noexcept {
        return "minimize_current_window" ;
    }

    void MinimizeCurrentWindow::sprocess(
            bool first_call,
            unsigned int repeat_num,
            KeycodeLogger* const UNUSED(parent_vkclgr),
            const CharLogger* const UNUSED(parent_charlgr)) {
        if(!first_call) return ;

        for(unsigned int i = 0 ; i < repeat_num ; i ++) {
            keybrd::pushup(KEYCODE_LWIN, KEYCODE_DOWN) ;
        }
    }

    //MaximizeCurrentWindow
    const std::string MaximizeCurrentWindow::sname() noexcept {
        return "maximize_current_window" ;
    }

    void MaximizeCurrentWindow::sprocess(
            bool first_call,
            unsigned int repeat_num,
            KeycodeLogger* const UNUSED(parent_vkclgr),
            const CharLogger* const UNUSED(parent_charlgr)) {
        if(!first_call) return ;

        if(repeat_num == 1) {
            auto hwnd = GetForegroundWindow() ;
            if(hwnd == NULL) {
                throw RUNTIME_EXCEPT("There is no foreground window.") ;
            }

            RECT before_rect ;
            if(!GetWindowRect(hwnd, &before_rect)) {
                throw RUNTIME_EXCEPT("Could not get a rectangle of a foreground window.") ;
            }

            keybrd::pushup(KEYCODE_LWIN, KEYCODE_UP) ;
            Sleep(50) ;

            RECT after_rect ;
            if(!GetWindowRect(hwnd, &after_rect)) {
                throw RUNTIME_EXCEPT("Could not get a rectangle of a foreground window.") ;
            }

            //If not changed, regard it as a full screen and deal with it.
            if(screenmetrics::is_equel(before_rect, after_rect)) {
                keybrd::pushup(KEYCODE_LWIN, KEYCODE_DOWN) ;
                keybrd::pushup(KEYCODE_LWIN, KEYCODE_UP) ;
            }
        }
        else {
            for(unsigned int i = 0 ; i < repeat_num ; i ++) {
                keybrd::pushup(KEYCODE_LWIN, KEYCODE_UP) ;
            }
        }
    }
}
