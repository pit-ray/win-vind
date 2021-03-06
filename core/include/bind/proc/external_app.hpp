#ifndef _EXTERNAL_APP_HPP
#define _EXTERNAL_APP_HPP

#include "bind/binded_func_creator.hpp"

namespace vind
{
    struct StartShell : public BindedFuncCreator<StartShell> {
        explicit StartShell() ;
        static void sprocess() ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;

    struct StartExternal : public BindedFuncCreator<StartExternal> {
        explicit StartExternal() ;
        static void sprocess(std::string cmd="") ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;

    struct StartExplorer : public BindedFuncCreator<StartExplorer> {
        explicit StartExplorer() ;
        static void sprocess() ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;

    struct OpenStartMenu : public BindedFuncCreator<OpenStartMenu> {
        explicit OpenStartMenu() ;
        static void sprocess() ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;
}

#endif
