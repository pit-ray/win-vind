#ifndef _COMMAND_HPP
#define _COMMAND_HPP

#include "bind/bindedfunc.hpp"

namespace vind
{
    namespace bind
    {
        struct Command : public BindedFuncFlex<Command> {
            explicit Command() ;
            static SystemCall sprocess(
                std::uint16_t count,
                const std::string& args,
                bool as_default=false) ;
        } ;

        struct Delcommand : public BindedFuncFlex<Delcommand> {
            explicit Delcommand() ;
            static SystemCall sprocess(
                std::uint16_t count,
                const std::string& args,
                bool as_default=false) ;
        } ;

        struct Comclear : public BindedFuncFlex<Comclear> {
            explicit Comclear() ;
            static SystemCall sprocess(
                std::uint16_t count,
                const std::string& args,
                bool as_default=false) ;
        } ;
    }
}

#endif
