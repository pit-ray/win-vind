#ifndef _JUMP_ACTWIN_HPP
#define _JUMP_ACTWIN_HPP

#include "bind/binded_func_with_creator.hpp"

namespace vind
{
    struct Jump2ActiveWindow : public BindedFuncWithCreator<Jump2ActiveWindow> {
        static void sprocess(
                const bool first_call,
                const unsigned int repeat_num,
                VKCLogger* const parent_vkclgr,
                const CharLogger* const parent_charlgr) ;
        static const std::string sname() noexcept ;
    } ;
}

#endif
