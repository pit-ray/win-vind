#ifndef _COMMAND_HPP
#define _COMMAND_HPP

#include "bind/binded_func.hpp"

namespace vind
{
    namespace bind
    {
        struct SyscmdCommand : BindedFuncFlex<SyscmdCommand> {
            explicit SyscmdCommand() ;
            static SystemCall sprocess(const std::string& args="") ;
            static SystemCall sprocess(core::NTypeLogger& parent_lgr) ;
            static SystemCall sprocess(const core::CharLogger& parent_lgr) ;
        } ;

        struct SyscmdDelcommand : BindedFuncFlex<SyscmdDelcommand> {
            explicit SyscmdDelcommand() ;
            static SystemCall sprocess(const std::string& args="") ;
            static SystemCall sprocess(core::NTypeLogger& parent_lgr) ;
            static SystemCall sprocess(const core::CharLogger& parent_lgr) ;
        } ;

        struct SyscmdComclear : BindedFuncFlex<SyscmdComclear> {
            explicit SyscmdComclear() ;
            static SystemCall sprocess() ;
            static SystemCall sprocess(core::NTypeLogger& parent_lgr) ;
            static SystemCall sprocess(const core::CharLogger& parent_lgr) ;
        } ;
    }
}

#endif
