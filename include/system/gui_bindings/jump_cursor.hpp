#ifndef _JUMP_CURSOR_HPP
#define _JUMP_CURSOR_HPP

#include "binded_function_with_creator.hpp"

struct Jump2Left : public BindedFunctionWithCreator<Jump2Left>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;

struct Jump2Right : public BindedFunctionWithCreator<Jump2Right>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;

struct Jump2Top : public BindedFunctionWithCreator<Jump2Top>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;

struct Jump2Bottom : public BindedFunctionWithCreator<Jump2Bottom>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;

struct Jump2XCenter : public BindedFunctionWithCreator<Jump2XCenter>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;

struct Jump2YCenter : public BindedFunctionWithCreator<Jump2YCenter>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;

struct Jump2Any : public BindedFunctionWithCreator<Jump2Any>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;

struct Jump2ActiveWindow : public BindedFunctionWithCreator<Jump2ActiveWindow>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;
#endif