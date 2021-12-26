#include "opt/dedicate_to_window.hpp"

#include <windows.h>

#include "bind/emu/edi_change_mode.hpp"
#include "bind/mode/change_mode.hpp"
#include "core/errlogger.hpp"
#include "core/inputgate.hpp"
#include "core/keycodedef.hpp"
#include "core/settable.hpp"
#include "opt/vcmdline.hpp"
#include "util/mouse.hpp"


namespace vind
{
    namespace opt
    {
        HWND Dedicate2Window::target_hwnd_ = NULL ;
        HWND Dedicate2Window::past_hwnd_   = NULL ;

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
                target_hwnd_ = GetForegroundWindow() ;
                past_hwnd_   = NULL ;
                opt::VCmdLine::print(GeneralMessage("-- TARGET ON --")) ;
            }
        }

        void Dedicate2Window::disable_targeting() {
            auto& settable = core::SetTable::get_instance() ;
            if(settable.get("dedicate_to_window").get<bool>()) {
                target_hwnd_ = NULL ;
                past_hwnd_   = NULL ;
                opt::VCmdLine::print(GeneralMessage("-- TARGET OFF --")) ;
            }
        }

        void Dedicate2Window::do_process() const {
            if(!target_hwnd_)  return ;

            auto foreground_hwnd = GetForegroundWindow() ;

            //is selected window changed?
            if(past_hwnd_ == foreground_hwnd) {
                return ;
            }

            if(target_hwnd_ == foreground_hwnd) { //other -> target
                bind::ToEdiNormal::sprocess(true) ;
            }
            else if(past_hwnd_ == target_hwnd_) { //target -> other
                bind::ToInsert::sprocess(true) ;
            }
            past_hwnd_ = foreground_hwnd ;
        }
    }
}
