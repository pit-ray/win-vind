#ifndef _CLIPBOARD_HPP
#define _CLIPBOARD_HPP

#include "key_binding_with_creator.hpp"

struct CBCopy : public KeyBindingWithCreator<CBCopy>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;


struct CBPaste : public KeyBindingWithCreator<CBPaste>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;


struct CBCut : public KeyBindingWithCreator<CBCut>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;


struct CBDelete : public KeyBindingWithCreator<CBDelete>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;


struct CBBackSpace : public KeyBindingWithCreator<CBBackSpace>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;
#endif