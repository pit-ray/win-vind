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

    class SmartClipboard
    {
    private:
        HWND hwnd ;

    public:
        explicit SmartClipboard(HWND handle) : hwnd(handle) {}

        bool is_open() noexcept {
            if(!OpenClipboard(hwnd)) {
                return false ;
            }

            return true ;
        }

        ~SmartClipboard() {
            CloseClipboard() ;
        }

        SmartClipboard(SmartClipboard&&) = delete ;
        SmartClipboard& operator=(SmartClipboard&&) = delete ;

        SmartClipboard(const SmartClipboard&) = delete ;
        SmartClipboard& operator=(const SmartClipboard&) = delete ;
    } ;

    inline static bool _is_included_EOL_in_clipboard() {
        auto hwnd = GetForegroundWindow() ;
        if(!hwnd) {
            WIN_ERROR_STREAM << "hwnd loading failed (ECBUtility::GetForegroundWindow)\n" ;
            return false ;
        }

        SmartClipboard scb(hwnd) ;
        if(!scb.is_open()) {
            WIN_ERROR_STREAM << "cannot open clipboard (ECBUtility::OpenClipboard)\n" ;
            return false ;
        }

        if(!IsClipboardFormatAvailable(CF_TEXT)) {
            WIN_ERROR_STREAM << "not supported format (ECBUtility::IsClipboardFormatAvailable)\n" ;
            return false ;
        }

        auto handle = GetClipboardData(CF_TEXT) ;
        if(!handle) {
            WIN_ERROR_STREAM << "cannot get clipboard data (ECBUtility::GetClipboardData)\n" ;
            return false ;
        }

        auto deleter = [](void* ptr) {
            GlobalUnlock(ptr) ;
        } ;
        std::unique_ptr<void, decltype(deleter)> obj_dptr(GlobalLock(handle), deleter) ;
        if(!obj_dptr) {
            WIN_ERROR_STREAM << "cannot lock global clipboard data (ECBUtility::GlobalLock)\n" ;
            return false ;
        }

        std::string str(reinterpret_cast<char*>(obj_dptr.get())) ;
        if(!GlobalUnlock(handle)) {
            WIN_ERROR_STREAM << "cannot unlock global clipboard data (ECBUtility::GlobalUnLock)\n" ;
            return false ;
        }

        return str.back() == '\n' ;
    }
}