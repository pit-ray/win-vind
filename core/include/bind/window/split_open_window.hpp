#ifndef _SPLIT_OPEN_WINDOW_HPP
#define _SPLIT_OPEN_WINDOW_HPP

#include "bind/binded_func_with_creator.hpp"

namespace vind
{
    struct OpenNewCurWinWithHorizontalSplit : public BindedFuncWithCreator<OpenNewCurWinWithHorizontalSplit> {
        static void sprocess(
                const bool first_call,
                const unsigned int repeat_num,
                KeycodeLogger* const parent_vkclgr,
                const CharLogger* const parent_charlgr) ;
        static const std::string sname() noexcept ;
    } ;

    struct OpenNewCurWinWithVerticalSplit : public BindedFuncWithCreator<OpenNewCurWinWithVerticalSplit> {
        static void sprocess(
                const bool first_call,
                const unsigned int repeat_num,
                KeycodeLogger* const parent_vkclgr,
                const CharLogger* const parent_charlgr) ;
        static const std::string sname() noexcept ;
    } ;
}

#endif
