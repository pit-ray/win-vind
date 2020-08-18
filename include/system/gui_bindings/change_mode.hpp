#ifndef _CHANGE_MODE_HPP
#define _CHANGE_MODE_HPP
#include "binded_function_with_creator.hpp"

struct Change2Normal : public BindedFunctionWithCreator<Change2Normal>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;


struct Change2Insert : public BindedFunctionWithCreator<Change2Insert>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;


struct Change2Visual : public BindedFunctionWithCreator<Change2Visual>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;


struct Change2Editor : public BindedFunctionWithCreator<Change2Editor>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;


struct Change2Command : public BindedFunctionWithCreator<Change2Command>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;

#endif