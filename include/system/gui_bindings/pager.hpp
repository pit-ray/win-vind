#ifndef _PAGER_HPP
#define _PAGER_HPP

#include "key_binding_with_creator.hpp"

struct Move2NextPage : public KeyBindingWithCreator<Move2NextPage>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;

struct Move2PrevPage : public KeyBindingWithCreator<Move2PrevPage>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;
#endif