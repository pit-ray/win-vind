#ifndef _CLIPBOARD_HPP
#define _CLIPBOARD_HPP

#include "binded_func_with_creator.hpp"

struct CBCopy : public BindedFuncWithCreator<CBCopy>
{
    static void sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger) ;
    static const std::string sname() noexcept ;
} ;


struct CBPaste : public BindedFuncWithCreator<CBPaste>
{
    static void sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger) ;
    static const std::string sname() noexcept ;
} ;


struct CBCut : public BindedFuncWithCreator<CBCut>
{
    static void sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger) ;
    static const std::string sname() noexcept ;
} ;


struct CBDelete : public BindedFuncWithCreator<CBDelete>
{
    static void sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger) ;
    static const std::string sname() noexcept ;
} ;


struct CBBackSpace : public BindedFuncWithCreator<CBBackSpace>
{
    static void sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger) ;
    static const std::string sname() noexcept ;
} ;
#endif
