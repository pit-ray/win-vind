#include "textanalyze.hpp"

#include <windows.h>

#include <memory>
#include <vector>

#include "core/errlogger.hpp"
#include "smartclipboard.hpp"
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
            Sleep(300) ;

            scb.open() ; //<open>-------------------------

            SelectedTextResult out{} ;
            out.having_EOL = scb.get_as_str(out.str) ;

            if(backup) scb.restore_backup() ;

            scb.close() ; //<close>----------------------
            return out ;
        }
    }
}
