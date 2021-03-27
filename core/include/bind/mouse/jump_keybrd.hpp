#ifndef _JUMP_KEYBRD_HPP
#define _JUMP_KEYBRD_HPP

#include "bind/binded_func_with_creator.hpp"

namespace vind
{
    struct Jump2Any : public BindedFuncWithCreator<Jump2Any> {
        static void sprocess(
                bool first_call,
                unsigned int repeat_num,
                KeycodeLogger* const parent_vkclgr,
                const CharLogger* const parent_charlgr) ;
        static const std::string sname() noexcept ;

        static void load_config() ;
    } ;
}

#endif