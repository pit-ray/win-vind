#include "bind/emu/text_util.hpp"

#include "bind/emu/simple_text_register.hpp"
#include "bind/emu/smart_clipboard.hpp"
#include "io/keybrd.hpp"
#include "key/keycode_def.hpp"
#include "util/def.hpp"

namespace vind
{
    namespace txutil {
        //Some editors have a visible EOL mark in a line.
        //This function select text from current position to EOL except for the visible EOL mark.
        //If the line has only null characters, it does not select.
        //  <EOL mark exists> [select] NONE    [clipboard] null characters with EOL.    (neighborhoods of LSB are 0x00)
        //  <plain text>      [select] NONE    [clipboard] null characters without EOL. (neighborhoods of LSB are 0x?0)
        bool select_line_until_EOL(const textanalyze::SelRes* const exres) {
            using namespace keybrd ;
            if(exres != nullptr) {
                pushup(KEYCODE_LSHIFT, KEYCODE_END) ;
                if(exres->having_EOL) {
                    pushup(KEYCODE_LSHIFT, KEYCODE_LEFT) ;
                    if(exres->str.empty()) {
                        return false ; //not selected (true text is only null text)
                    }
                }
                return true ; //selected
            }

            auto res = textanalyze::get_selected_text([] {
                    pushup(KEYCODE_LSHIFT, KEYCODE_END) ;
                    pushup(KEYCODE_LCTRL, KEYCODE_C) ;
            }) ;
            if(res.having_EOL) {
                pushup(KEYCODE_LSHIFT, KEYCODE_LEFT) ;
                if(res.str.empty()) {
                    return false ;
                }
            }
            return true ;
        }

        void clear_clipboard_with_null() {
            auto hwnd = GetForegroundWindow() ;
            if(!hwnd) {
                throw RUNTIME_EXCEPT("not exist active window") ;
            }
            SmartClipboard scb(hwnd) ;
            scb.open() ;
            scb.set("") ;
            scb.close() ;
        }
    }
}
