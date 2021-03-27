#include "jump_actwin.hpp"

#include <windows.h>

#include "util/def.hpp"

namespace vind
{
    //Jump2ActiveWindow
    const std::string Jump2ActiveWindow::sname() noexcept {
        return "jump_to_active_window" ;
    }

    void Jump2ActiveWindow::sprocess(
            bool first_call,
            unsigned int UNUSED(repeat_num),
            KeycodeLogger* const UNUSED(parent_vkclgr),
            const CharLogger* const UNUSED(parent_charlgr)) {
        if(!first_call) return ;

        const auto hwnd = GetForegroundWindow() ;
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
}
