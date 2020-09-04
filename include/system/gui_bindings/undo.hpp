#ifndef _UNDO_HPP
#define _UNDO_HPP

#include "key_binding_with_creator.hpp"

struct SCRedo : public KeyBindingWithCreator<SCRedo>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;


struct SCUndo : public KeyBindingWithCreator<SCUndo>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;
#endif