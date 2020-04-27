#ifndef _CLICK_HPP
#define _CLICK_HPP
#include "binded_function_with_creator.hpp"

struct ClickLeft : public BindedFunctionWithCreator<ClickLeft>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;


struct ClickRight : public BindedFunctionWithCreator<ClickRight>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;

#endif