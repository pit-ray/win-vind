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

bool Dedicate2Window::do_process() const
{
    auto is_selected = [] {
        if(!MouseEventer::is_releasing_occured(VKC_MOUSE_LEFT)) {
            return false ;
        }

        using KeyAbsorber::is_pressed ;
        return is_pressed(VKC_LALT) ||
               is_pressed(VKC_RALT) ||
               is_pressed(VKC_LSHIFT) ||
               is_pressed(VKC_RSHIFT) ||
               is_pressed(VKC_LCTRL) ||
               is_pressed(VKC_RCTRL) ;
    } ;

    static HWND target_hwnd = NULL ;
    static HWND past_hwnd = NULL ;
    auto selected_hwnd = GetForegroundWindow() ;

    if(!target_hwnd) {
        if(!is_selected()) return true ;
        //turn on
        target_hwnd = past_hwnd = selected_hwnd ;
        if(!Change2Editor::sprocess(true)) {
            return false ;
        }
        VirtualCmdLine::msgout("-- TARGET ON --") ;
        return true ;
    }

    //-- targeting now --
    if(is_selected()) { //turn off
        target_hwnd = NULL ;
        past_hwnd = NULL ;
        if(!Change2Insert::sprocess(true)) {
            return false ;
        }
        VirtualCmdLine::msgout("-- TARGET OFF --") ;
        return true ;
    }

    //is selected window changed?
    if(past_hwnd == selected_hwnd) {
        return true ;
    }

    if(target_hwnd == selected_hwnd) { //other -> target
        if(!Change2EdiNormal::sprocess(true)) {
            return false ;
        }
    }
    else if(past_hwnd == target_hwnd) { //target -> other
        if(!Change2Insert::sprocess(true)) {
            return false ;
        }
    }

    past_hwnd = selected_hwnd ;
    return true ;
}