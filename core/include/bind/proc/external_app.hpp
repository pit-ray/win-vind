#ifndef _EXTERNAL_APP_HPP
#define _EXTERNAL_APP_HPP

#include "bind/binded_func_with_creator.hpp"

namespace vind
{
    namespace exapp {
        void load_config() ;
    }

    struct StartShell : public BindedFuncWithCreator<StartShell> {
        static void sprocess(
                bool first_call,
                unsigned int repeat_num,
                KeycodeLogger* const parent_vkclgr,
                const CharLogger* const parent_charlgr) ;
        static const std::string sname() noexcept ;
    } ;

    struct StartAnyApp : public BindedFuncWithCreator<StartAnyApp> {
        static void sprocess(
                bool first_call,
                unsigned int repeat_num,
                KeycodeLogger* const parent_vkclgr,
                const CharLogger* const parent_charlgr) ;
        static const std::string sname() noexcept ;
    } ;

    struct StartExplorer : public BindedFuncWithCreator<StartExplorer> {
        static void sprocess(
                bool first_call,
                unsigned int repeat_num,
                KeycodeLogger* const parent_vkclgr,
                const CharLogger* const parent_charlgr) ;
        static const std::string sname() noexcept ;
    } ;

    struct OpenStartMenu : public BindedFuncWithCreator<OpenStartMenu> {
        static void sprocess(
                bool first_call,
                unsigned int repeat_num,
                KeycodeLogger* const parent_vkclgr,
                const CharLogger* const parent_charlgr) ;
        static const std::string sname() noexcept ;
    } ;
}

#endif
