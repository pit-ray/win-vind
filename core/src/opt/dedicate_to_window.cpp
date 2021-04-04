#include "opt/dedicate_to_window.hpp"

#include <windows.h>
#include <iostream>

#include "coreio/i_params.hpp"
#include "coreio/err_logger.hpp"
#include "bind/emu/edi_change_mode.hpp"
#include "bind/mode/change_mode.hpp"
#include "io/mouse.hpp"
#include "key/key_absorber.hpp"
#include "key/keycode_def.hpp"
#include "opt/virtual_cmd_line.hpp"

namespace
{
    HWND target_hwnd = NULL ;
    HWND past_hwnd   = NULL ;
}

namespace vind
{
    const std::string Dedicate2Window::sname() noexcept {
        return "dedicate_to_window" ;
    }

    void Dedicate2Window::do_enable() const {
    }

    void Dedicate2Window::do_disable() const {
    }

    void Dedicate2Window::enable_targeting() {
        if(iparams::get_b(sname())) {
            target_hwnd = GetForegroundWindow() ;
            past_hwnd   = NULL ;
            VirtualCmdLine::msgout("-- TARGET ON --") ;
        }
    }

    void Dedicate2Window::disable_targeting() {
        if(iparams::get_b(sname())) {
            target_hwnd = NULL ;
            past_hwnd   = NULL ;
            VirtualCmdLine::msgout("-- TARGET OFF --") ;
        }
    }

    void Dedicate2Window::do_process() const {
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
}
