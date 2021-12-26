#ifndef _EXTERNAL_APP_HPP
#define _EXTERNAL_APP_HPP

#include "bind/bindedfunc.hpp"

#include <filesystem>


namespace vind
{
    namespace bind
    {
        struct StartShell : public BindedFuncVoid<StartShell> {
            explicit StartShell() ;
            static void sprocess() ;
            static void sprocess(core::NTypeLogger& parent_lgr) ;
            static void sprocess(const core::CharLogger& parent_lgr) ;
        } ;

        struct StartExternal : public BindedFuncVoid<StartExternal> {
            explicit StartExternal() ;
            static void sprocess(std::string cmd="") ;
            static void sprocess(core::NTypeLogger& parent_lgr) ;
            static void sprocess(const core::CharLogger& parent_lgr) ;
        } ;

        struct StartExplorer : public BindedFuncVoid<StartExplorer> {
            explicit StartExplorer() ;
            static void sprocess() ;
            static void sprocess(core::NTypeLogger& parent_lgr) ;
            static void sprocess(const core::CharLogger& parent_lgr) ;
        } ;

        struct OpenStartMenu : public BindedFuncVoid<OpenStartMenu> {
            explicit OpenStartMenu() ;
            static void sprocess() ;
            static void sprocess(core::NTypeLogger& parent_lgr) ;
            static void sprocess(const core::CharLogger& parent_lgr) ;
        } ;
    }
}

#endif
