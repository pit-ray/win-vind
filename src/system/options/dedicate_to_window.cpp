#include "dedicate_to_window.hpp"

#include <windows.h>
#include <iostream>

#include "virtual_key_fwd.hpp"
#include "msg_logger.hpp"
#include "key_absorber.hpp"
#include "mouse_eventer.hpp"
#include "change_mode.hpp"
#include "edi_change_mode.hpp"

Dedicate2Window::Dedicate2Window() = default ;
Dedicate2Window::~Dedicate2Window() noexcept = default ;
Dedicate2Window::Dedicate2Window(Dedicate2Window&&) noexcept = default ;
Dedicate2Window& Dedicate2Window::operator=(Dedicate2Window&&) noexcept = default ;

const std::string Dedicate2Window::name() const noexcept
{
    return "dedicate_to_window" ;
}

std::unique_ptr<DynamicOption> Dedicate2Window::create()
{
    return std::move(std::make_unique<Dedicate2Window>()) ;
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
    static HWND hwnd = NULL ;
    static auto is_other_selected = true ;
    auto selected_hwnd = GetForegroundWindow() ;

    if(!MouseEventer::is_releasing_occured(VKC_MOUSE_LEFT)) {
        return true ;
    }

    using KeyAbsorber::is_pressed ;
    if(is_pressed(VKC_LALT) || is_pressed(VKC_RALT) ||
       is_pressed(VKC_LSHIFT) || is_pressed(VKC_RSHIFT) ||
       is_pressed(VKC_LCTRL) || is_pressed(VKC_RCTRL)) {
        if(hwnd != selected_hwnd) {
            hwnd = selected_hwnd ;
            is_other_selected = true ;
        }
        else {
            hwnd = NULL ;
            Change2Insert::sprocess(true) ;
        }
    }
    if(!hwnd) return true ;

    if(hwnd != selected_hwnd) { //other window
        if(!Change2Insert::sprocess(!is_other_selected)) {
            return false ;
        }
        is_other_selected = true ;
    }
    else { //dedicated window
        if(!Change2EdiNormal::sprocess(is_other_selected)) {
            return false ;
        }
        is_other_selected = false ;
    }
    return true ;
}