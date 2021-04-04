#ifndef _MYWINDOW_CTRL_HPP
#define _MYWINDOW_CTRL_HPP

#include "bind/binded_func_with_creator.hpp"
#include <functional>

namespace vind
{
    struct ShowConfigWindow : public BindedFuncWithCreator<ShowConfigWindow> {
        static void sprocess(
                bool first_call,
                unsigned int repeat_num,
                KeycodeLogger* const parent_keycodelgr,
                const CharLogger* const parent_charlgr) ;
        static const std::string sname() noexcept ;

        static void register_show_func(std::function<void()> func) noexcept ;
    } ;


    struct ExitConfigWindow : public BindedFuncWithCreator<ExitConfigWindow> {
        static void sprocess(
                bool first_call,
                unsigned int repeat_num,
                KeycodeLogger* const parent_keycodelgr,
                const CharLogger* const parent_charlgr) ;
        static const std::string sname() noexcept ;

        static void register_exit_func(std::function<void()> func) noexcept ;
    } ;
}

#endif
