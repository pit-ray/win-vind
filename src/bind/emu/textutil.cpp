#include "textutil.hpp"

#include "core/inputgate.hpp"
#include "core/keycodedef.hpp"
#include "smartclipboard.hpp"
#include "textreg.hpp"
#include "util/def.hpp"
#include "util/winwrap.hpp"


namespace vind
{
    namespace bind
    {
        //Some editors have a visible EOL mark in a line.
        //This function select text from current position to EOL except for the visible EOL mark.
        //If the line has only null characters, it does not select.
        //  <EOL mark exists> [select] NONE    [clipboard] null characters with EOL.    (neighborhoods of LSB are 0x00)
        //  <plain text>      [select] NONE    [clipboard] null characters without EOL. (neighborhoods of LSB are 0x?0)
        bool select_line_until_EOL(const SelectedTextResult* const exres) {
            auto& igate = core::InputGate::get_instance() ;

            if(exres != nullptr) {
                igate.pushup(KEYCODE_LSHIFT, KEYCODE_END) ;
                if(exres->having_EOL) {
                    igate.pushup(KEYCODE_LSHIFT, KEYCODE_LEFT) ;
                    if(exres->str.empty()) {
                        return false ; //not selected (true text is only null text)
                    }
                }
                return true ; //selected
            }

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
