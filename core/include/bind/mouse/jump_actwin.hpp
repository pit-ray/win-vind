#ifndef _JUMP_ACTWIN_HPP
#define _JUMP_ACTWIN_HPP

#include "bind/binded_func_with_creator.hpp"

namespace vind
{
    struct Jump2ActiveWindow : public BindedFuncWithCreator<Jump2ActiveWindow> {
        static void sprocess(
                bool first_call,
                unsigned int repeat_num,
                KeycodeLogger* const parent_keycodelgr,
                const CharLogger* const parent_charlgr) ;
        static const std::string sname() noexcept ;
    } ;
}

#endif
