#include "dedicate_to_window_ctrl.hpp"

#include <iostream>

#include "utility.hpp"
#include "dedicate_to_window.hpp"

//Enable
const std::string EnableTargetingOfDedicate2Window::sname() noexcept
{
    return "enable_targeting_of_dedicate_to_window" ;
}

bool EnableTargetingOfDedicate2Window::common_process()
{
    return Dedicate2Window::enable_targeting() ;
}

bool EnableTargetingOfDedicate2Window::sprocess(const bool first_call)
{
    if(!first_call) return true ;
    return common_process() ;
}

bool EnableTargetingOfDedicate2Window::sprocess(const std::string UNUSED(cmd))
{
    return common_process() ;
}


//Disable
const std::string DisableTargetingOfDedicate2Window::sname() noexcept
{
    return "disable_targeting_of_dedicate_to_window" ;
}

bool DisableTargetingOfDedicate2Window::common_process()
{
    return Dedicate2Window::disable_targeting() ;
}

bool DisableTargetingOfDedicate2Window::sprocess(const bool first_call)
{
    if(!first_call) return true ;
    return common_process() ;
}

bool DisableTargetingOfDedicate2Window::sprocess(const std::string UNUSED(cmd))
{
    return common_process() ;
}
