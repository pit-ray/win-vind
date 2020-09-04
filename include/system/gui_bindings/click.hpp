#ifndef _CLICK_HPP
#define _CLICK_HPP
#include "key_binding_with_creator.hpp"

struct ClickLeft : public KeyBindingWithCreator<ClickLeft>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;


struct ClickRight : public KeyBindingWithCreator<ClickRight>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;

#endif