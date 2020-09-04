#ifndef _SWITCH_TASKVIEW_HPP
#define _SWITCH_TASKVIEW_HPP

#include "key_binding_with_creator.hpp"

struct SwitchVDesktop2Left : public KeyBindingWithCreator<SwitchVDesktop2Left>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;


struct SwitchVDesktop2Right : public KeyBindingWithCreator<SwitchVDesktop2Right>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;
#endif