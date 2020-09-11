#ifndef _CHANGE_MODE_HPP
#define _CHANGE_MODE_HPP
#include "key_binding_with_creator.hpp"

struct Change2Normal : public KeyBindingWithCreator<Change2Normal>
{
    static bool sprocess(const bool first_call, const bool vclmodeout = true) ;
    static const std::string sname() noexcept ;
} ;


struct Change2Insert : public KeyBindingWithCreator<Change2Insert>
{
    static bool sprocess(const bool first_call, const bool vclmodeout = true) ;
    static const std::string sname() noexcept ;
} ;


struct Change2Visual : public KeyBindingWithCreator<Change2Visual>
{
    static bool sprocess(const bool first_call, const bool vclmodeout = true) ;
    static const std::string sname() noexcept ;
} ;


struct Change2Editor : public KeyBindingWithCreator<Change2Editor>
{
    static bool sprocess(const bool first_call, const bool vclmodeout = true) ;
    static const std::string sname() noexcept ;
} ;


struct Change2Command : public KeyBindingWithCreator<Change2Command>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;

#endif
