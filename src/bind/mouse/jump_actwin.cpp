#include "jump_actwin.hpp"

#include <stdio.h>
#include <windows.h>

#include "core/ntypelogger.hpp"
#include "util/def.hpp"
#include "util/winwrap.hpp"

namespace vind
{
    namespace bind
    {
        //JumpToActiveWindow
        JumpToActiveWindow::JumpToActiveWindow()
        : BindedFuncVoid("jump_cursor_to_active_window")
        {}
        void JumpToActiveWindow::sprocess() {
            auto hwnd = util::get_foreground_window() ;
            auto rect = util::get_window_rect(hwnd) ;
            SetCursorPos(rect.center_x(), rect.center_y()) ;
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
