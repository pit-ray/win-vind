#ifndef _EXTERNAL_APP_HPP
#define _EXTERNAL_APP_HPP

#include "bind/binded_func_creator.hpp"

#include <filesystem>


namespace vind
{
    struct StartShell : public BindedFuncCreator<StartShell> {
        explicit StartShell() ;
        static void sprocess() ;
        static void sprocess(core::NTypeLogger& parent_lgr) ;
        static void sprocess(const core::CharLogger& parent_lgr) ;
    } ;

    struct StartExternal : public BindedFuncCreator<StartExternal> {
        explicit StartExternal() ;
        static void sprocess(std::string cmd="") ;
        static void sprocess(core::NTypeLogger& parent_lgr) ;
        static void sprocess(const core::CharLogger& parent_lgr) ;
    } ;

    struct StartExplorer : public BindedFuncCreator<StartExplorer> {
        explicit StartExplorer() ;
        static void sprocess() ;
        static void sprocess(core::NTypeLogger& parent_lgr) ;
        static void sprocess(const core::CharLogger& parent_lgr) ;
    } ;

    struct OpenStartMenu : public BindedFuncCreator<OpenStartMenu> {
        explicit OpenStartMenu() ;
        static void sprocess() ;
        static void sprocess(core::NTypeLogger& parent_lgr) ;
        static void sprocess(const core::CharLogger& parent_lgr) ;
    } ;
}

#endif
