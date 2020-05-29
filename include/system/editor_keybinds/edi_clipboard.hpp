#ifndef _EDI_CLIPBOARD_HPP
#define _EDI_CLIPBOARD_HPP

#include "binded_function_with_creator.hpp"


//Copy
struct EdiCopyHighlightText : public BindedFunctionWithCreator<EdiCopyHighlightText>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;

struct EdiNCopyLine : public BindedFunctionWithCreator<EdiNCopyLine>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;


//Paste
struct EdiNPasteAfter : public BindedFunctionWithCreator<EdiNPasteAfter>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;

struct EdiNPasteBefore : public BindedFunctionWithCreator<EdiNPasteBefore>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;


//Delete
struct EdiDeleteHighlightText : public BindedFunctionWithCreator<EdiDeleteHighlightText>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;

struct EdiNDeleteLine : public BindedFunctionWithCreator<EdiNDeleteLine>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;


struct EdiNDeleteLineUntilEOL : public BindedFunctionWithCreator<EdiNDeleteLineUntilEOL>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;

struct EdiNDeleteAfter : public BindedFunctionWithCreator<EdiNDeleteAfter>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;

struct EdiNDeleteBefore : public BindedFunctionWithCreator<EdiNDeleteBefore>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;

#endif