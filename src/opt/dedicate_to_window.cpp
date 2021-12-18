#include "opt/dedicate_to_window.hpp"

#if defined(DEBUG)
#include <iostream>
#endif

#include <windows.h>

#include "bind/emu/edi_change_mode.hpp"
#include "bind/mode/change_mode.hpp"
#include "core/err_logger.hpp"
#include "core/inputgate.hpp"
#include "core/keycode_def.hpp"
#include "core/settable.hpp"
#include "opt/vcmdline.hpp"
#include "util/mouse.hpp"

namespace
{
    HWND target_hwnd = NULL ;
    HWND past_hwnd   = NULL ;
}

namespace vind
{
    namespace opt
    {
        Dedicate2Window::Dedicate2Window()
        : OptionCreator("dedicate_to_window")
        {}

        void Dedicate2Window::do_enable() const {
        }

        void Dedicate2Window::do_disable() const {
        }

        void Dedicate2Window::enable_targeting() {
            auto& settable = core::SetTable::get_instance() ;
            if(settable.get("dedicate_to_window").get<bool>()) {
                target_hwnd = GetForegroundWindow() ;
                past_hwnd   = NULL ;
                opt::VCmdLine::print(GeneralMessage("-- TARGET ON --")) ;
            }
        }

        void Dedicate2Window::disable_targeting() {
            auto& settable = core::SetTable::get_instance() ;
            if(settable.get("dedicate_to_window").get<bool>()) {
                target_hwnd = NULL ;
                past_hwnd   = NULL ;
                opt::VCmdLine::print(GeneralMessage("-- TARGET OFF --")) ;
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
                bind::ToEdiNormal::sprocess(true) ;
            }
            else if(past_hwnd == target_hwnd) { //target -> other
                bind::ToInsert::sprocess(true) ;
            }
            past_hwnd = foreground_hwnd ;
        }
    }
}
