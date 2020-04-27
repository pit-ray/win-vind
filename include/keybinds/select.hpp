#ifndef _SELECT_HPP
#define _SELECT_HPP
#include "binded_function_with_creator.hpp"

struct SelectAll : public BindedFunctionWithCreator<SelectAll>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;

#endif