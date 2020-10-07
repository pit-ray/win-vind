#include "dedicate_to_window.hpp"

#include <windows.h>
#include <iostream>

#include "virtual_key_fwd.hpp"
#include "msg_logger.hpp"
#include "key_absorber.hpp"
#include "mouse_eventer.hpp"
#include "change_mode.hpp"
#include "edi_change_mode.hpp"
#include "virtual_cmd_line.hpp"

namespace D2WUtility
{
    static const auto _NULL = static_cast<HWND>(0) ;
    static auto target_hwnd = _NULL ;
    static auto past_hwnd   = _NULL ;
}

const std::string Dedicate2Window::sname() noexcept
{
    return "dedicate_to_window" ;
}

bool Dedicate2Window::do_enable() const noexcept
{
    return true ;
}

bool Dedicate2Window::do_disable() const noexcept
{
    return true ;
}

bool Dedicate2Window::enable_targeting() noexcept
{
    using namespace D2WUtility ;
    target_hwnd = GetForegroundWindow() ;
    past_hwnd   = _NULL ;
    VirtualCmdLine::msgout("-- TARGET ON --") ;
    return true ;
}

bool Dedicate2Window::disable_targeting() noexcept
{
    using namespace D2WUtility ;
    target_hwnd = _NULL ;
    past_hwnd   = _NULL ;
    VirtualCmdLine::msgout("-- TARGET OFF --") ;
    return true ;
}

bool Dedicate2Window::do_process() const
{
    using namespace D2WUtility ;
    if(!target_hwnd)  return true ;

    const auto foreground_hwnd = GetForegroundWindow() ;

    //is selected window changed?
    if(past_hwnd == foreground_hwnd) {
        return true ;
    }

    if(target_hwnd == foreground_hwnd) { //other -> target
        if(!Change2Editor::sprocess(true, 1, nullptr, past_hwnd)) {
            return false ;
        }
    }
    else if(past_hwnd == target_hwnd) { //target -> other
        if(!Change2Insert::sprocess(true, 1, nullptr, past_hwnd)) {
            return false ;
        }
    }
    past_hwnd = foreground_hwnd ;
    return true ;
}
