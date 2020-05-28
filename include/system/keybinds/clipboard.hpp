#ifndef _CLIPBOARD_HPP
#define _CLIPBOARD_HPP

#include "binded_function_with_creator.hpp"

struct CBCopy : public BindedFunctionWithCreator<CBCopy>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;


struct CBPaste : public BindedFunctionWithCreator<CBPaste>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;


struct CBCut : public BindedFunctionWithCreator<CBCut>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;


struct CBDelete : public BindedFunctionWithCreator<CBDelete>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;


struct CBBackSpace : public BindedFunctionWithCreator<CBBackSpace>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;
#endif