#ifndef _COMMAND_HPP
#define _COMMAND_HPP

#include "bind/binded_func_creator.hpp"

namespace vind
{
    namespace bind
    {
        struct SyscmdCommand : BindedFuncCreator<SyscmdCommand> {
            explicit SyscmdCommand() ;
            static void sprocess(
                    const std::string& args="",
                    bool reload_config=false) ;
            static void sprocess(core::NTypeLogger& parent_lgr) ;
            static void sprocess(const core::CharLogger& parent_lgr) ;
        } ;

        struct SyscmdDelcommand : BindedFuncCreator<SyscmdDelcommand> {
            explicit SyscmdDelcommand() ;
            static void sprocess(
                    const std::string& args="",
                    bool reload_config=false) ;
            static void sprocess(core::NTypeLogger& parent_lgr) ;
            static void sprocess(const core::CharLogger& parent_lgr) ;
        } ;

        struct SyscmdComclear : BindedFuncCreator<SyscmdComclear> {
            explicit SyscmdComclear() ;
            static void sprocess(bool reload_config=false) ;
            static void sprocess(core::NTypeLogger& parent_lgr) ;
            static void sprocess(const core::CharLogger& parent_lgr) ;
        } ;
    }
}

#endif