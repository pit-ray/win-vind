#include "dedicate_to_window_ctrl.hpp"

#include <iostream>

#include "opt/dedicate_to_window.hpp"
#include "util/def.hpp"

namespace vind
{
    //Enable
    const std::string EnableTargetingOfDedicate2Window::sname() noexcept {
        return "enable_targeting_of_dedicate_to_window" ;
    }
    void EnableTargetingOfDedicate2Window::sprocess(
            bool first_call,
            unsigned int UNUSED(repeat_num),
            KeycodeLogger* const UNUSED(parent_vkclgr),
            const CharLogger* const UNUSED(parent_charlgr)) {
        if(!first_call) return ;
        Dedicate2Window::enable_targeting() ;
    }

    //Disable
    const std::string DisableTargetingOfDedicate2Window::sname() noexcept {
        return "disable_targeting_of_dedicate_to_window" ;
    }
    void DisableTargetingOfDedicate2Window::sprocess(
            bool first_call,
            unsigned int UNUSED(repeat_num),
            KeycodeLogger* const UNUSED(parent_vkclgr),
            const CharLogger* const UNUSED(parent_charlgr)) {
        if(!first_call) return ;
        Dedicate2Window::disable_targeting() ;
    }
}