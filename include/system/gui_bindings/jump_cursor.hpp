#ifndef _JUMP_CURSOR_HPP
#define _JUMP_CURSOR_HPP

#include "key_binding_with_creator.hpp"

namespace JumpCursorUtility
{
    void load_config() noexcept ;
}

struct Jump2Left : public KeyBindingWithCreator<Jump2Left>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;

struct Jump2Right : public KeyBindingWithCreator<Jump2Right>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;

struct Jump2Top : public KeyBindingWithCreator<Jump2Top>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;

struct Jump2Bottom : public KeyBindingWithCreator<Jump2Bottom>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;

struct Jump2XCenter : public KeyBindingWithCreator<Jump2XCenter>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;

struct Jump2YCenter : public KeyBindingWithCreator<Jump2YCenter>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;

struct Jump2Any : public KeyBindingWithCreator<Jump2Any>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;

struct Jump2ActiveWindow : public KeyBindingWithCreator<Jump2ActiveWindow>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;
#endif