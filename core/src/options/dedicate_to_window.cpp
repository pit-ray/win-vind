#include "dedicate_to_window.hpp"

#include <windows.h>
#include <iostream>

#include "change_mode.hpp"
#include "edi_change_mode.hpp"
#include "key_absorber.hpp"
#include "mouse_eventer.hpp"
#include "msg_logger.hpp"
#include "virtual_cmd_line.hpp"
#include "virtual_key_fwd.hpp"

namespace D2WUtility
{
    static HWND target_hwnd = NULL ;
    static HWND past_hwnd   = NULL ;
}

const std::string Dedicate2Window::sname() noexcept
{
    return "dedicate_to_window" ;
}

void Dedicate2Window::do_enable() const
{
}

void Dedicate2Window::do_disable() const
{
}

void Dedicate2Window::enable_targeting()
{
    using namespace D2WUtility ;
    target_hwnd = GetForegroundWindow() ;
    past_hwnd   = NULL ;
    VirtualCmdLine::msgout("-- TARGET ON --") ;
}

void Dedicate2Window::disable_targeting()
{
    using namespace D2WUtility ;
    target_hwnd = NULL ;
    past_hwnd   = NULL ;
    VirtualCmdLine::msgout("-- TARGET OFF --") ;
}

void Dedicate2Window::do_process() const
{
    using namespace D2WUtility ;
    if(!target_hwnd)  return ;

    const auto foreground_hwnd = GetForegroundWindow() ;

    //is selected window changed?
    if(past_hwnd == foreground_hwnd) {
        return ;
    }

    if(target_hwnd == foreground_hwnd) { //other -> target
        Change2EdiNormal::sprocess(true, 1, nullptr, nullptr, past_hwnd) ;
    }
    else if(past_hwnd == target_hwnd) { //target -> other
        Change2Insert::sprocess(true, 1, nullptr, nullptr, past_hwnd) ;
    }
    past_hwnd = foreground_hwnd ;
}
