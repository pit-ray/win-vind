#include "opt/dedicate_to_window.hpp"

#if defined(DEBUG)
#include <iostream>
#endif

#include <windows.h>

#include "bind/emu/edi_change_mode.hpp"
#include "bind/mode/change_mode.hpp"
#include "err_logger.hpp"
#include "g_params.hpp"
#include "io/mouse.hpp"
#include "key/key_absorber.hpp"
#include "key/keycode_def.hpp"
#include "opt/vcmdline.hpp"

namespace
{
    HWND target_hwnd = NULL ;
    HWND past_hwnd   = NULL ;
}

namespace vind
{
    Dedicate2Window::Dedicate2Window()
    : OptionCreator("dedicate_to_window")
    {}

    void Dedicate2Window::do_enable() const {
    }

    void Dedicate2Window::do_disable() const {
    }

    void Dedicate2Window::enable_targeting() {
        if(gparams::get_b("dedicate_to_window")) {
            target_hwnd = GetForegroundWindow() ;
            past_hwnd   = NULL ;
            VCmdLine::print(GeneralMessage("-- TARGET ON --")) ;
        }
    }

    void Dedicate2Window::disable_targeting() {
        if(gparams::get_b("dedicate_to_window")) {
            target_hwnd = NULL ;
            past_hwnd   = NULL ;
            VCmdLine::print(GeneralMessage("-- TARGET OFF --")) ;
        }
    }

    void Dedicate2Window::do_process() const {
        if(!target_hwnd)  return ;

        auto foreground_hwnd = GetForegroundWindow() ;

        //is selected window changed?
        if(past_hwnd == foreground_hwnd) {
            return ;
        }

        if(target_hwnd == foreground_hwnd) { //other -> target
            ToEdiNormal::sprocess(true) ;
        }
        else if(past_hwnd == target_hwnd) { //target -> other
            ToInsert::sprocess(true) ;
        }
        past_hwnd = foreground_hwnd ;
    }
}
