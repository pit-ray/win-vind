#ifndef MY_BINDING_HPP
#define MY_BINDING_HPP
#include "binded_func_with_creator.hpp"

struct MyBinding : public BindedFuncWithCreator<MyBinding>
{
    static void sprocess(
            const bool first_call,
            const unsigned int repeat_num,
            KeyLogger* parent_vkclgr,
            const KeyLogger* const parent_charlgr) ;
    static const std::string sname() noexcept ;
} ;

#endif
