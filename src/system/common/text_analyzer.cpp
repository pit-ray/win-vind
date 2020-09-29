#include "text_analyzer.hpp"

#include <windows.h>

#include <iostream>
#include <memory>
#include <vector>

#include "keybrd_eventer.hpp"
#include "mouse_eventer.hpp"
#include "msg_logger.hpp"
#include "smart_clipboard.hpp"
#include "utility.hpp"

using namespace std ;

namespace TextAnalyzer{
    const SelRes get_selected_text(std::function<bool()> clip_func, const bool backup) {
        const auto hwnd = GetForegroundWindow() ;
        if(!hwnd) {
            WIN_ERROR_PRINT("not exist active window") ;
            return SelRes(false) ;
        }

        SmartClipboard scb(hwnd) ;
        if(!scb.open()) {
            return SelRes(false) ;
        }

        if(backup) {
            if(!scb.backup()) {
                return SelRes(false) ;
            }
        }

        //initialize clipboard
        if(!scb.set("")) {
            return SelRes(false) ;
        }

        if(!scb.close()) {
            return SelRes(false) ;
        }

        //By copy or cut functions, sends text to analyze to clipboard.
        if(!clip_func()) {
            return SelRes(false) ;
        }

        //It needs to start reading text in a clipboard,
        //insofar as having already arrived the copied text
        //from the editor to a clipboard,
        //so wait for a little.
        Sleep(100) ;

        if(!scb.open()) {
            return SelRes(false) ;
        }

        SelRes out(true, "", false) ;
        if(!scb.get_as_str(out.str, out.having_EOL)) {
            return SelRes(false) ;
        }

        if(backup) {
            if(!scb.restore_backup()) {
                return SelRes(false) ;
            }
        }

        if(!scb.close()) {
            return SelRes(false) ;
        }

        return out ;
    }
}
