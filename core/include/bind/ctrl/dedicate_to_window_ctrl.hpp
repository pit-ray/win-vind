#ifndef _DEDICATE_TO_WINDOW_CTRL_HPP
#define _DEDICATE_TO_WINDOW_CTRL_HPP

#include "bind/binded_func_with_creator.hpp"

namespace vind
{
    struct EnableTargetingOfDedicate2Window : public BindedFuncWithCreator<EnableTargetingOfDedicate2Window> {
        static void sprocess(
                const bool first_call,
                const unsigned int repeat_num,
                VKCLogger* const parent_vkclgr,
                const CharLogger* const parent_charlgr) ;
        static const std::string sname() noexcept ;
    } ;

    struct DisableTargetingOfDedicate2Window : public BindedFuncWithCreator<DisableTargetingOfDedicate2Window> {
        static void sprocess(
                const bool first_call,
                const unsigned int repeat_num,
                VKCLogger* const parent_vkclgr,
                const CharLogger* const parent_charlgr) ;
        static const std::string sname() noexcept ;
    } ;
}

#endif
