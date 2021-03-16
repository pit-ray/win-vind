#ifndef _MIN_MAX_WINDOW_HPP
#define _MIN_MAX_WINDOW_HPP

#include "binded_func_with_creator.hpp"

struct MinimizeCurrentWindow : public BindedFuncWithCreator<MinimizeCurrentWindow>
{
    static void sprocess(
            const bool first_call,
            const unsigned int repeat_num,
            VKCLogger* const parent_vkclgr,
            const CharLogger* const parent_charlgr) ;
    static const std::string sname() noexcept ;
} ;

struct MaximizeCurrentWindow : public BindedFuncWithCreator<MaximizeCurrentWindow>
{
    static void sprocess(
            const bool first_call,
            const unsigned int repeat_num,
            VKCLogger* const parent_vkclgr,
            const CharLogger* const parent_charlgr) ;
    static const std::string sname() noexcept ;
} ;

#endif
