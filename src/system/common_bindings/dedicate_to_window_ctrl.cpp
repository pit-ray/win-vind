#include "dedicate_to_window_ctrl.hpp"

#include <iostream>

#include "utility.hpp"
#include "dedicate_to_window.hpp"

//Enable
const std::string EnableTargetingOfDedicate2Window::sname() noexcept
{
    return "enable_targeting_of_dedicate_to_window" ;
}
void EnableTargetingOfDedicate2Window::sprocess(
        const bool first_call,
        const unsigned int UNUSED(repeat_num),
        KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr))
{
    if(!first_call) return ;
    Dedicate2Window::enable_targeting() ;
}


//Disable
const std::string DisableTargetingOfDedicate2Window::sname() noexcept
{
    return "disable_targeting_of_dedicate_to_window" ;
}
void DisableTargetingOfDedicate2Window::sprocess(
        const bool first_call,
        const unsigned int UNUSED(repeat_num),
        KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr))
{
    if(!first_call) return ;
    Dedicate2Window::disable_targeting() ;
}
