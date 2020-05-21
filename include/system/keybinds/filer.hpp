#ifndef _FILER_HPP
#define _FILER_HPP

#include "command_with_creator.hpp"

struct SaveOpenedFile : public CommandWithCreator<SaveOpenedFile>
{
    static bool sprocess(const std::string cmd) ;
    static const std::string sname() noexcept ;
} ;

struct CloseOpenedFile : public CommandWithCreator<CloseOpenedFile>
{
    static bool sprocess(const std::string cmd) ;
    static const std::string sname() noexcept ;
} ;

#endif