#ifndef _EDI_JUMP_CARET_HPP
#define _EDI_JUMP_CARET_HPP

#include "key_binding_with_creator.hpp"

struct EdiJumpCaret2BOL : public KeyBindingWithCreator<EdiJumpCaret2BOL>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;

struct EdiJumpCaret2EOL : public KeyBindingWithCreator<EdiJumpCaret2EOL>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;

struct EdiNJumpCaret2Line_DfBOF : public KeyBindingWithCreator<EdiNJumpCaret2Line_DfBOF>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;

struct EdiNJumpCaret2Line_DfEOF : public KeyBindingWithCreator<EdiNJumpCaret2Line_DfEOF>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;

#endif