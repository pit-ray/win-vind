#ifndef _SHOW_CONFIG_WINDOW_HPP
#define _SHOW_CONFIG_WINDOW_HPP

#include "command_with_creator.hpp"
#include <functional>

struct ShowConfigWindow : public CommandWithCreator<ShowConfigWindow>
{
    static bool sprocess(const std::string cmd) ;
    static const std::string sname() noexcept ;

    static void register_show_func(std::function<void()> func) noexcept ;
} ;

#endif