#ifndef _COMMAND_HPP
#define _COMMAND_HPP

#include "bind/bindedfunc.hpp"

namespace vind
{
    namespace bind
    {
        struct SyscmdCommand : BindedFuncFlex<SyscmdCommand> {
            explicit SyscmdCommand() ;
            static SystemCall sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;

        struct SyscmdDelcommand : BindedFuncFlex<SyscmdDelcommand> {
            explicit SyscmdDelcommand() ;
            static SystemCall sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;

        struct SyscmdComclear : BindedFuncFlex<SyscmdComclear> {
            explicit SyscmdComclear() ;
            static SystemCall sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;
    }
}

#endif
