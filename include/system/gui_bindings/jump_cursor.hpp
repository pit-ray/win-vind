#ifndef _JUMP_CURSOR_HPP
#define _JUMP_CURSOR_HPP

#include "binded_func_with_creator.hpp"

namespace JumpCursorUtility
{
    void load_config() ;
}

struct Jump2Left : public BindedFuncWithCreator<Jump2Left>
{
    static void sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger) ;
    static const std::string sname() noexcept ;
} ;

struct Jump2Right : public BindedFuncWithCreator<Jump2Right>
{
    static void sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger) ;
    static const std::string sname() noexcept ;
} ;

struct Jump2Top : public BindedFuncWithCreator<Jump2Top>
{
    static void sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger) ;
    static const std::string sname() noexcept ;
} ;

struct Jump2Bottom : public BindedFuncWithCreator<Jump2Bottom>
{
    static void sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger) ;
    static const std::string sname() noexcept ;
} ;

struct Jump2XCenter : public BindedFuncWithCreator<Jump2XCenter>
{
    static void sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger) ;
    static const std::string sname() noexcept ;
} ;

struct Jump2YCenter : public BindedFuncWithCreator<Jump2YCenter>
{
    static void sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger) ;
    static const std::string sname() noexcept ;
} ;

struct Jump2Any : public BindedFuncWithCreator<Jump2Any>
{
    static void sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger) ;
    static const std::string sname() noexcept ;
} ;

struct Jump2ActiveWindow : public BindedFuncWithCreator<Jump2ActiveWindow>
{
    static void sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger) ;
    static const std::string sname() noexcept ;
} ;
#endif
