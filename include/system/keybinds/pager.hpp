#ifndef _PAGER_HPP
#define _PAGER_HPP

#include "binded_function_with_creator.hpp"

struct Move2NextPage : public BindedFunctionWithCreator<Move2NextPage>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;

struct Move2PrevPage : public BindedFunctionWithCreator<Move2PrevPage>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;
#endif