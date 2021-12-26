#include "dedicate_to_window_ctrl.hpp"

#include "core/charlogger.hpp"
#include "core/ntypelogger.hpp"

#include "opt/dedicate_to_window.hpp"
#include "util/debug.hpp"
#include "util/def.hpp"


namespace vind
{
    namespace bind
    {
        //Enable
        EnableTargetingOfDedicate2Window::EnableTargetingOfDedicate2Window()
        : BindedFuncVoid("enable_targeting_of_dedicate_to_window")
        {}
        void EnableTargetingOfDedicate2Window::sprocess() {
            opt::Dedicate2Window::enable_targeting() ;
        }
        void EnableTargetingOfDedicate2Window::sprocess(core::NTypeLogger& parent_lgr) {
            if(!parent_lgr.is_long_pressing()) {
                sprocess() ;
            }
        }
        void EnableTargetingOfDedicate2Window::sprocess(const core::CharLogger& UNUSED(parent_lgr)) {
            sprocess() ;
        }

        //Disable
        DisableTargetingOfDedicate2Window::DisableTargetingOfDedicate2Window()
        : BindedFuncVoid("disable_targeting_of_dedicate_to_window")
        {}
        void DisableTargetingOfDedicate2Window::sprocess() {
            opt::Dedicate2Window::disable_targeting() ;
        }
        void DisableTargetingOfDedicate2Window::sprocess(core::NTypeLogger& parent_lgr) {
            if(!parent_lgr.is_long_pressing()) {
                sprocess() ;
            }
        }
        void DisableTargetingOfDedicate2Window::sprocess(const core::CharLogger& UNUSED(parent_lgr)) {
            sprocess() ;
        }
    }
}
