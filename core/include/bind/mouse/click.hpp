#ifndef _CLICK_HPP
#define _CLICK_HPP
#include "bind/binded_func_with_creator.hpp"

namespace vind
{
    struct ClickLeft : public BindedFuncWithCreator<ClickLeft> {
        static void sprocess(
                const bool first_call,
                const unsigned int repeat_num,
                VKCLogger* const parent_vkclgr,
                const CharLogger* const parent_charlgr) ;
        static const std::string sname() noexcept ;
    } ;


    struct ClickRight : public BindedFuncWithCreator<ClickRight> {
        static void sprocess(
                const bool first_call,
                const unsigned int repeat_num,
                VKCLogger* const parent_vkclgr,
                const CharLogger* const parent_charlgr) ;
        static const std::string sname() noexcept ;
    } ;
}

#endif
