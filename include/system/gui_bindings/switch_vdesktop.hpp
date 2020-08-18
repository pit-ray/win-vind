#ifndef _SWITCH_TASKVIEW_HPP
#define _SWITCH_TASKVIEW_HPP

#include "binded_function_with_creator.hpp"

struct SwitchVDesktop2Left : public BindedFunctionWithCreator<SwitchVDesktop2Left>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;


struct SwitchVDesktop2Right : public BindedFunctionWithCreator<SwitchVDesktop2Right>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;
#endif