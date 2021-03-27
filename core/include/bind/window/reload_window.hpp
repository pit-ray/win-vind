#ifndef _RELOAD_WINDOW_HPP
#define _RELOAD_WINDOW_HPP

#include "bind/binded_func_with_creator.hpp"

namespace vind
{
    struct ReloadCurrentWindow : public BindedFuncWithCreator<ReloadCurrentWindow> {
        static void sprocess(
                bool first_call,
                unsigned int repeat_num,
                KeycodeLogger* const parent_vkclgr,
                const CharLogger* const parent_charlgr) ;
        static const std::string sname() noexcept ;
    } ;
}

#endif