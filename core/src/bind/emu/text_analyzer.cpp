#include "bind/emu/text_analyzer.hpp"

#include <windows.h>

#include <memory>
#include <vector>

#include "bind/emu/smart_clipboard.hpp"
#include "err_logger.hpp"
#include "io/keybrd.hpp"
#include "io/mouse.hpp"
#include "util/def.hpp"

#if defined(DEBUG)
#include <iostream>
#endif

namespace vind
{
    namespace textanalyze{
        SelRes get_selected_text(
                std::function<void()> clip_func,
                bool backup) {
            auto hwnd = GetForegroundWindow() ;
            if(!hwnd) {
                throw RUNTIME_EXCEPT("not exist active window") ;
            }

            SmartClipboard scb(hwnd) ;
            scb.open() ; //<open>-------------------------

            if(backup) scb.backup() ;

            //initialize clipboard
            scb.set("") ;

            scb.close() ; //<close>-----------------------

            //By copy or cut functions, sends text to analyze to clipboard.
            clip_func() ;

            //It needs to start reading text in a clipboard,
            //insofar as having already arrived the copied text
            //from the editor to a clipboard,
            //so wait for a little.
            Sleep(100) ;

            scb.open() ; //<open>-------------------------

            SelRes out{} ;
            scb.get_as_str(out.str, out.having_EOL) ;

            if(backup) scb.restore_backup() ;

            scb.close() ; //<close>----------------------
            return out ;
        }
    }
}
