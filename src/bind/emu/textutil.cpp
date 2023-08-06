#include "textutil.hpp"

#include <windows.h>

#include "core/inputgate.hpp"
#include "smartclipboard.hpp"
#include "util/winwrap.hpp"


namespace vind
{
    namespace bind
    {
        SelectedTextResult get_selected_text(
                std::function<void()> clip_func,
                bool backup) {
            auto hwnd = util::get_foreground_window() ;
            if(!hwnd) {
                throw std::runtime_error("There is no foreground window.") ;
            }

            SmartClipboard scb(hwnd) ;
            scb.open() ;

            if(backup) {
                scb.backup() ;
            }

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

            if(backup) {
                scb.restore_backup() ;
            }

            scb.close() ;
            return out ;
        }
    }
}
