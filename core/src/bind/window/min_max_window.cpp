#include "bind/window/min_max_window.hpp"

#include "bind/safe_repeater.hpp"
#include "io/keybrd.hpp"
#include "key/char_logger.hpp"
#include "key/ntype_logger.hpp"
#include "util/def.hpp"
#include "util/rect.hpp"

namespace vind
{
    //MinimizeCurrentWindow
    MinimizeCurrentWindow::MinimizeCurrentWindow()
    : BindedFuncCreator("minimize_current_window")
    {}
    void MinimizeCurrentWindow::sprocess(unsigned int repeat_num) {
        repeater::safe_for(repeat_num, [] {
            keybrd::pushup(KEYCODE_LWIN, KEYCODE_DOWN) ;
        }) ;
    }
    void MinimizeCurrentWindow::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(parent_lgr.get_head_num()) ;
        }
    }
    void MinimizeCurrentWindow::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess() ;
    }


    //MaximizeCurrentWindow
    MaximizeCurrentWindow::MaximizeCurrentWindow()
    : BindedFuncCreator("maximize_current_window")
    {}
    void MaximizeCurrentWindow::sprocess(unsigned int repeat_num) {
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
            if(util::is_equel(before_rect, after_rect)) {
                keybrd::pushup(KEYCODE_LWIN, KEYCODE_DOWN) ;
                keybrd::pushup(KEYCODE_LWIN, KEYCODE_UP) ;
            }
        }
        else {
            repeater::safe_for(repeat_num, [] {
                keybrd::pushup(KEYCODE_LWIN, KEYCODE_UP) ;
            }) ;
        }
    }
    void MaximizeCurrentWindow::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(parent_lgr.get_head_num()) ;
        }
    }
    void MaximizeCurrentWindow::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess() ;
    }
}
