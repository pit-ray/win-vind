#include "bind/mouse/jump_actwin.hpp"

#include <stdio.h>
#include <windows.h>

#include "key/ntype_logger.hpp"
#include "util/def.hpp"

namespace vind
{
    //Jump2ActiveWindow
    Jump2ActiveWindow::Jump2ActiveWindow()
    : BindedFuncCreator("jump_to_active_window")
    {}
    void Jump2ActiveWindow::sprocess() {
        auto hwnd = GetForegroundWindow() ;
        if(!hwnd) {
            throw RUNTIME_EXCEPT("GetForegoundWindow return nullptr") ;
        }

        RECT rect ;
        if(!GetWindowRect(hwnd, &rect)) {
            throw RUNTIME_EXCEPT("cannot get window rect") ;
        }

        auto&& xpos = static_cast<int>(rect.left + (rect.right - rect.left) / 2) ;
        auto&& ypos = static_cast<int>(rect.top + (rect.bottom - rect.top) / 2) ;

        SetCursorPos(xpos, ypos) ;
    }
    void Jump2ActiveWindow::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess() ;
        }
    }
    void Jump2ActiveWindow::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess() ;
    }
}
