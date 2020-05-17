/*



                NOT IMPLEMENTED





*/
#include "text_analyzer.hpp"
#include "msg_logger.hpp"

#include <windows.h>
#include <richedit.h>
#include <vector>

#include <iostream>

//test
#include "mouse_eventer.hpp"
#include "keybrd_eventer.hpp"

#define UNICODE_PAGE (1200)

using namespace std ;

namespace TextAnalyzer{

    static vector<vector<DWORD32>> textmap ;

    inline static void disp_error_msg() noexcept {
        Logger::error_stream << "[Error] windows.h: " \
        << GetLastError() << " (text_analyzer.cpp)\n" ;
    }

    bool is_update() noexcept {
        POINT pos{0, 0} ;
        if(!GetCursorPos(&pos)) {
            disp_error_msg() ;
            return false ;
        }

        const auto hwnd = WindowFromPoint(pos) ;

        TCHAR buf[1000] ;
        //GetWindowText(hwnd, buf, 1000) ;
        SendMessage(hwnd, WM_GETTEXT, 1000, reinterpret_cast<LPARAM>(buf)) ;

        cout << buf << endl ;

        return true ;
    }
    /*
    in order to get text of edit control,
    use CTRL + A and Clipboard control.
    in addition, create text map from all text data,
    it is avaiable to move cursor, jump, search word, etc...
    */
}