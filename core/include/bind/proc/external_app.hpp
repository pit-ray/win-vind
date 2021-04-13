#ifndef _EXTERNAL_APP_HPP
#define _EXTERNAL_APP_HPP

#include "bind/base/binded_func_with_creator.hpp"

namespace vind
{
    struct StartShell : public BindedFuncCreator<StartShell> {
        static void sprocess() ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
        static const std::string sname() noexcept ;
    } ;

    struct StartAnyApp : public BindedFuncCreator<StartAnyApp> {
        static void sprocess(std::string cmd="") ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
        static const std::string sname() noexcept ;

        void load_config() override ;
    } ;

    struct StartExplorer : public BindedFuncCreator<StartExplorer> {
        static void sprocess() ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
        static const std::string sname() noexcept ;
    } ;

    struct OpenStartMenu : public BindedFuncCreator<OpenStartMenu> {
        static void sprocess() ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
        static const std::string sname() noexcept ;
    } ;
}

#endif
