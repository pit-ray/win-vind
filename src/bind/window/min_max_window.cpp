#include "min_max_window.hpp"

#include "bind/safe_repeater.hpp"
#include "core/char_logger.hpp"
#include "core/ntype_logger.hpp"
#include "util/def.hpp"
#include "util/keybrd.hpp"
#include "util/rect.hpp"

namespace vind
{
    namespace bind
    {
        //MinimizeCurrentWindow
        MinimizeCurrentWindow::MinimizeCurrentWindow()
        : BindedFuncVoid("minimize_current_window")
        {}
        void MinimizeCurrentWindow::sprocess(unsigned int repeat_num) {
            safe_for(repeat_num, [] {
                util::pushup(KEYCODE_LWIN, KEYCODE_DOWN) ;
            }) ;
        }
        void MinimizeCurrentWindow::sprocess(core::NTypeLogger& parent_lgr) {
            if(!parent_lgr.is_long_pressing()) {
                sprocess(parent_lgr.get_head_num()) ;
            }
        }
        void MinimizeCurrentWindow::sprocess(const core::CharLogger& UNUSED(parent_lgr)) {
            sprocess() ;
        }


        //MaximizeCurrentWindow
        MaximizeCurrentWindow::MaximizeCurrentWindow()
        : BindedFuncVoid("maximize_current_window")
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

                util::pushup(KEYCODE_LWIN, KEYCODE_UP) ;
                Sleep(50) ;

                RECT after_rect ;
                if(!GetWindowRect(hwnd, &after_rect)) {
                    throw RUNTIME_EXCEPT("Could not get a rectangle of a foreground window.") ;
                }

                //If not changed, regard it as a full screen and deal with it.
                if(util::is_equal(before_rect, after_rect)) {
                    util::pushup(KEYCODE_LWIN, KEYCODE_DOWN) ;
                    util::pushup(KEYCODE_LWIN, KEYCODE_UP) ;
                }
            }
            else {
                safe_for(repeat_num, [] {
                    util::pushup(KEYCODE_LWIN, KEYCODE_UP) ;
                }) ;
            }
        }
        void MaximizeCurrentWindow::sprocess(core::NTypeLogger& parent_lgr) {
            if(!parent_lgr.is_long_pressing()) {
                sprocess(parent_lgr.get_head_num()) ;
            }
        }
        void MaximizeCurrentWindow::sprocess(const core::CharLogger& UNUSED(parent_lgr)) {
            sprocess() ;
        }
    }
}
