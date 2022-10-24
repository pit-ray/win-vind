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
        void JumpToActiveWindow::sprocess(
                std::uint16_t UNUSED(count),
                const std::string& UNUSED(args)) {
            auto hwnd = util::get_foreground_window() ;
            auto rect = util::get_window_rect(hwnd) ;
            util::set_cursor_pos(rect.center()) ;
        }
    }
}
