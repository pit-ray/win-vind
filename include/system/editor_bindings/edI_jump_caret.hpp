#ifndef _EDI_JUMP_CARET_HPP
#define _EDI_JUMP_CARET_HPP

#include "binded_function_with_creator.hpp"

struct EdiJumpCaret2BOL : public BindedFunctionWithCreator<EdiJumpCaret2BOL>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;

struct EdiJumpCaret2EOL : public BindedFunctionWithCreator<EdiJumpCaret2EOL>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;

struct EdiNJumpCaret2Line_DfBOF : public BindedFunctionWithCreator<EdiNJumpCaret2Line_DfBOF>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;

struct EdiNJumpCaret2Line_DfEOF : public BindedFunctionWithCreator<EdiNJumpCaret2Line_DfEOF>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;

#endif