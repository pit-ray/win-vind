#ifndef _SYSTEM_HPP
#define _SYSTEM_HPP

#include <functional>

namespace System
{
    bool init() noexcept ;

    bool load_config() noexcept ;
    bool load_option_config() noexcept ;

    bool update() noexcept ;
    bool update_options() noexcept ;

    void register_show_window_func(std::function<void()> func) noexcept ;
    void register_exit_window_func(std::function<void()> func) noexcept ;
}

#endif
