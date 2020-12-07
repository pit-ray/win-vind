#ifndef _SELECT_HPP
#define _SELECT_HPP
#include "binded_func_with_creator.hpp"

struct SelectAll : public BindedFuncWithCreator<SelectAll>
{
    static void sprocess(
            const bool first_call,
            const unsigned int repeat_num,
            KeyLogger* parent_vkclgr,
            const KeyLogger* const parent_charlgr) ;
    static const std::string sname() noexcept ;
} ;

#endif
