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
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;

        struct StartExternal : public BindedFuncVoid<StartExternal> {
            explicit StartExternal() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;

        struct StartExplorer : public BindedFuncVoid<StartExplorer> {
            explicit StartExplorer() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;

        struct OpenStartMenu : public BindedFuncVoid<OpenStartMenu> {
            explicit OpenStartMenu() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;
    }
}

#endif
