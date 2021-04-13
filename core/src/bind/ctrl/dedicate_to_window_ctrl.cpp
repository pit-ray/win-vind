#include "bind/ctrl/dedicate_to_window_ctrl.hpp"

#include <iostream>

#include "bind/base/ntype_logger.hpp"
#include "bind/base/char_logger.hpp"

#include "opt/dedicate_to_window.hpp"
#include "util/def.hpp"

namespace vind
{
    //Enable
    const std::string EnableTargetingOfDedicate2Window::sname() noexcept {
        return "enable_targeting_of_dedicate_to_window" ;
    }
    void EnableTargetingOfDedicate2Window::sprocess() {
        Dedicate2Window::enable_targeting() ;
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
    const std::string DisableTargetingOfDedicate2Window::sname() noexcept {
        return "disable_targeting_of_dedicate_to_window" ;
    }
    void DisableTargetingOfDedicate2Window::sprocess() {
        Dedicate2Window::disable_targeting() ;
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
