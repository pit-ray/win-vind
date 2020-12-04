#ifndef _COMMAND_MODE_HPP
#define _COMMAND_MODE_HPP

#include "binded_func_with_creator.hpp"

struct CommandMode : public BindedFuncWithCreator<CommandMode>
{
    static const std::string sname() noexcept ;
    static void sprocess(
            const bool first_call,
            const unsigned int repeat_num,
            const KeyLogger* parent_vkclgr,
            const KeyLogger* const parent_charlgr) ;
} ;

#endif
