#include "jump_actwin.hpp"

#include <stdio.h>
#include <windows.h>

#include "core/ntype_logger.hpp"
#include "util/def.hpp"

namespace vind
{
    namespace bind
    {
        //JumpToActiveWindow
        JumpToActiveWindow::JumpToActiveWindow()
        : BindedFuncVoid("jump_cursor_to_active_window")
        {}
        void JumpToActiveWindow::sprocess() {
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
        void JumpToActiveWindow::sprocess(core::NTypeLogger& parent_lgr) {
            if(!parent_lgr.is_long_pressing()) {
                sprocess() ;
            }
        }
        void JumpToActiveWindow::sprocess(const core::CharLogger& UNUSED(parent_lgr)) {
            sprocess() ;
        }
    }
}
