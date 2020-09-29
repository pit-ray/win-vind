#ifndef _COMMAND_MODE_HPP
#define _COMMAND_MODE_HPP

#include "key_binding_with_creator.hpp"
#include "command.hpp"

struct CommandMode : public KeyBindingWithCreator<CommandMode>
{
    static const std::string sname() noexcept ;
    static bool sprocess(const bool first_call) ;
    static void load_config() noexcept ;
} ;

#endif
