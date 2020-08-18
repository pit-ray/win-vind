#ifndef _MYWINDOW_CTRL_HPP
#define _MYWINDOW_CTRL_HPP

#include "command_with_creator.hpp"
#include <functional>

struct ShowConfigWindow : public CommandWithCreator<ShowConfigWindow>
{
    static bool sprocess(const std::string cmd) ;
    static const std::string sname() noexcept ;

    static void register_show_func(std::function<void()> func) noexcept ;
} ;


struct ExitConfigWindow : public CommandWithCreator<ExitConfigWindow>
{
    static bool sprocess(const std::string cmd) ;
    static const std::string sname() noexcept ;

    static void register_exit_func(std::function<void()> func) noexcept ;
} ;

#endif