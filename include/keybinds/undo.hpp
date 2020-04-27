#ifndef _UNDO_HPP
#define _UNDO_HPP

#include "binded_function_with_creator.hpp"

struct SCRedo : public BindedFunctionWithCreator<SCRedo>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;


struct SCUndo : public BindedFunctionWithCreator<SCUndo>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;
#endif