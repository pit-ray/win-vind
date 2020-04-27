#ifndef _SWITCH_TASKVIEW_HPP
#define _SWITCH_TASKVIEW_HPP

#include "binded_function_with_creator.hpp"

struct SwitchTaskview2Left : public BindedFunctionWithCreator<SwitchTaskview2Left>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;


struct SwitchTaskview2Right : public BindedFunctionWithCreator<SwitchTaskview2Right>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;
#endif