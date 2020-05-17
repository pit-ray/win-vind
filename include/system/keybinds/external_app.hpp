#ifndef _EXTERNAL_APP_HPP
#define _EXTERNAL_APP_HPP

#include "command_with_creator.hpp"

struct StartShell : public CommandWithCreator<StartShell>
{
    static bool sprocess(const std::string& cmd) ;
    static const std::string sname() noexcept ;
} ;

struct StartAnyApp : public CommandWithCreator<StartAnyApp>
{
    static bool sprocess(const std::string& cmd) ;
    static const std::string sname() noexcept ;
} ;

#endif