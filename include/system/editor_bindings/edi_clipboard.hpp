#ifndef _EDI_CLIPBOARD_HPP
#define _EDI_CLIPBOARD_HPP

#include "key_binding_with_creator.hpp"


//Copy
struct EdiCopyHighlightText : public KeyBindingWithCreator<EdiCopyHighlightText>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;

struct EdiNCopyLine : public KeyBindingWithCreator<EdiNCopyLine>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;


//Paste
struct EdiNPasteAfter : public KeyBindingWithCreator<EdiNPasteAfter>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;

struct EdiNPasteBefore : public KeyBindingWithCreator<EdiNPasteBefore>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;


//Delete
struct EdiDeleteHighlightText : public KeyBindingWithCreator<EdiDeleteHighlightText>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;

struct EdiNDeleteLine : public KeyBindingWithCreator<EdiNDeleteLine>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;


struct EdiNDeleteLineUntilEOL : public KeyBindingWithCreator<EdiNDeleteLineUntilEOL>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;

struct EdiNDeleteAfter : public KeyBindingWithCreator<EdiNDeleteAfter>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;

struct EdiNDeleteBefore : public KeyBindingWithCreator<EdiNDeleteBefore>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;

#endif