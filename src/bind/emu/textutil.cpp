#include "textutil.hpp"

#include <windows.h>

#include <memory>
#include <vector>

#include "core/errlogger.hpp"
#include "core/inputgate.hpp"
#include "core/keycodedef.hpp"
#include "smartclipboard.hpp"
#include "textreg.hpp"
#include "util/debug.hpp"
#include "util/def.hpp"
#include "util/mouse.hpp"
#include "util/winwrap.hpp"


namespace vind
{
    namespace bind
    {
        SelectedTextResult get_selected_text(
                std::function<void()> clip_func,
                bool backup) {
            auto hwnd = util::get_foreground_window() ;

            SmartClipboard scb(hwnd) ;
            scb.open() ;

            if(backup) scb.backup() ;

            //initialize clipboard
            scb.set("") ;

            scb.close() ;

            //By copy or cut functions, sends text to analyze to clipboard.
            clip_func() ;

            //It needs to start reading text in a clipboard,
            //insofar as having already arrived the copied text
            //from the editor to a clipboard,
            //so wait for a little.
            Sleep(300) ;

            scb.open() ;

            SelectedTextResult out{} ;
            out.having_EOL = scb.get_as_str(out.str) ;

            if(backup) scb.restore_backup() ;

            scb.close() ;
            return out ;
        }

        bool select_line_until_EOL() {
            auto& igate = core::InputGate::get_instance() ;
            auto res = get_selected_text([&igate] {
                igate.pushup(KEYCODE_LSHIFT, KEYCODE_END) ;
                igate.pushup(KEYCODE_LCTRL, KEYCODE_C) ;
            }) ;

            if(res.having_EOL) {
                igate.pushup(KEYCODE_LSHIFT, KEYCODE_LEFT) ;
                if(res.str.empty()) {
                    return false ;
                }
            }
            return true ;
        }

        void clear_clipboard_with_null() {
            auto hwnd = util::get_foreground_window() ;
            SmartClipboard scb(hwnd) ;
            scb.open() ;
            scb.set("") ;
            scb.close() ;
        }
    }
}
