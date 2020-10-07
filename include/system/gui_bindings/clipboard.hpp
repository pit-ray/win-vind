#ifndef _CLIPBOARD_HPP
#define _CLIPBOARD_HPP

#include "binded_func_with_creator.hpp"

struct CBCopy : public BindedFuncWithCreator<CBCopy>
{
    static bool sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger) ;
    static const std::string sname() noexcept ;
} ;


struct CBPaste : public BindedFuncWithCreator<CBPaste>
{
    static bool sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger) ;
    static const std::string sname() noexcept ;
} ;


struct CBCut : public BindedFuncWithCreator<CBCut>
{
    static bool sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger) ;
    static const std::string sname() noexcept ;
} ;


struct CBDelete : public BindedFuncWithCreator<CBDelete>
{
    static bool sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger) ;
    static const std::string sname() noexcept ;
} ;


struct CBBackSpace : public BindedFuncWithCreator<CBBackSpace>
{
    static bool sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger) ;
    static const std::string sname() noexcept ;
} ;
#endif
