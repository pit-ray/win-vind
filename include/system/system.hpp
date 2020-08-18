#ifndef _SYSTEM_HPP
#define _SYSTEM_HPP

#include <functional>

namespace System
{
    bool init() ;

    void load_config() ;
    void load_option_config() ;

    bool update() ;

    void register_show_window_func(std::function<void()> func) noexcept ;
    void register_exit_window_func(std::function<void()> func) noexcept ;
}

#endif