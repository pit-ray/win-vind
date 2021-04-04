#ifndef _EXTERNAL_APP_HPP
#define _EXTERNAL_APP_HPP

#include "bind/base/binded_func_with_creator.hpp"

namespace vind
{
    struct StartShell : public BindedFuncWithCreator<StartShell> {
        static void sprocess(
                bool first_call,
                unsigned int repeat_num,
                KeycodeLogger* const parent_keycodelgr,
                const CharLogger* const parent_charlgr) ;
        static const std::string sname() noexcept ;
    } ;

    struct StartAnyApp : public BindedFuncWithCreator<StartAnyApp> {
        static void sprocess(
                bool first_call,
                unsigned int repeat_num,
                KeycodeLogger* const parent_keycodelgr,
                const CharLogger* const parent_charlgr) ;
        static const std::string sname() noexcept ;

        void load_config() override ;
    } ;

    struct StartExplorer : public BindedFuncWithCreator<StartExplorer> {
        static void sprocess(
                bool first_call,
                unsigned int repeat_num,
                KeycodeLogger* const parent_keycodelgr,
                const CharLogger* const parent_charlgr) ;
        static const std::string sname() noexcept ;
    } ;

    struct OpenStartMenu : public BindedFuncWithCreator<OpenStartMenu> {
        static void sprocess(
                bool first_call,
                unsigned int repeat_num,
                KeycodeLogger* const parent_keycodelgr,
                const CharLogger* const parent_charlgr) ;
        static const std::string sname() noexcept ;
    } ;
}

#endif
