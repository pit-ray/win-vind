#ifndef _EXTERNAL_APP_HPP
#define _EXTERNAL_APP_HPP

#include "binded_func_with_creator.hpp"

namespace ExAppUtility
{
    void load_config() noexcept ;
}

struct StartShell : public BindedFuncWithCreator<StartShell>
{
    static bool sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger) ;
    static const std::string sname() noexcept ;
} ;

struct StartAnyApp : public BindedFuncWithCreator<StartAnyApp>
{
    static bool sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger) ;
    static const std::string sname() noexcept ;
} ;

#endif
