#ifndef _JUMP_KEYBRD_HPP
#define _JUMP_KEYBRD_HPP

#include "binded_func_with_creator.hpp"

namespace vind
{
    struct Jump2Any : public BindedFuncWithCreator<Jump2Any> {
        static void sprocess(
                const bool first_call,
                const unsigned int repeat_num,
                VKCLogger* const parent_vkclgr,
                const CharLogger* const parent_charlgr) ;
        static const std::string sname() noexcept ;

        static void load_config() ;
    } ;
}

#endif
