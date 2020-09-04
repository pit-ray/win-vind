#ifndef _SELECT_HPP
#define _SELECT_HPP
#include "key_binding_with_creator.hpp"

struct SelectAll : public KeyBindingWithCreator<SelectAll>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;

#endif