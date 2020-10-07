#ifndef _JUMP_CURSOR_HPP
#define _JUMP_CURSOR_HPP

#include "binded_func_with_creator.hpp"

namespace JumpCursorUtility
{
    void load_config() noexcept ;
}

struct Jump2Left : public BindedFuncWithCreator<Jump2Left>
{
    static bool sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger) ;
    static const std::string sname() noexcept ;
} ;

struct Jump2Right : public BindedFuncWithCreator<Jump2Right>
{
    static bool sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger) ;
    static const std::string sname() noexcept ;
} ;

struct Jump2Top : public BindedFuncWithCreator<Jump2Top>
{
    static bool sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger) ;
    static const std::string sname() noexcept ;
} ;

struct Jump2Bottom : public BindedFuncWithCreator<Jump2Bottom>
{
    static bool sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger) ;
    static const std::string sname() noexcept ;
} ;

struct Jump2XCenter : public BindedFuncWithCreator<Jump2XCenter>
{
    static bool sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger) ;
    static const std::string sname() noexcept ;
} ;

struct Jump2YCenter : public BindedFuncWithCreator<Jump2YCenter>
{
    static bool sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger) ;
    static const std::string sname() noexcept ;
} ;

struct Jump2Any : public BindedFuncWithCreator<Jump2Any>
{
    static bool sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger) ;
    static const std::string sname() noexcept ;
} ;

struct Jump2ActiveWindow : public BindedFuncWithCreator<Jump2ActiveWindow>
{
    static bool sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger) ;
    static const std::string sname() noexcept ;
} ;
#endif
