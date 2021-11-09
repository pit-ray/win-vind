#include "dedicate_to_window_ctrl.hpp"

#include "core/char_logger.hpp"
#include "core/ntype_logger.hpp"

#include "opt/dedicate_to_window.hpp"
#include "util/def.hpp"

#if defined(DEBUG)
#include <iostream>
#endif


namespace vind
{
    //Enable
    EnableTargetingOfDedicate2Window::EnableTargetingOfDedicate2Window()
    : BindedFuncCreator("enable_targeting_of_dedicate_to_window")
    {}
    void EnableTargetingOfDedicate2Window::sprocess() {
        opt::Dedicate2Window::enable_targeting() ;
    }
    void EnableTargetingOfDedicate2Window::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess() ;
        }
    }
    void EnableTargetingOfDedicate2Window::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess() ;
    }

    //Disable
    DisableTargetingOfDedicate2Window::DisableTargetingOfDedicate2Window()
    : BindedFuncCreator("disable_targeting_of_dedicate_to_window")
    {}
    void DisableTargetingOfDedicate2Window::sprocess() {
        opt::Dedicate2Window::disable_targeting() ;
    }
    void DisableTargetingOfDedicate2Window::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess() ;
        }
    }
    void DisableTargetingOfDedicate2Window::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess() ;
    }
}
