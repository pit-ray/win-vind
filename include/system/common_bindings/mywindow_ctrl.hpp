#ifndef _MYWINDOW_CTRL_HPP
#define _MYWINDOW_CTRL_HPP

#include "binded_func_with_creator.hpp"
#include <functional>

struct ShowConfigWindow : public BindedFuncWithCreator<ShowConfigWindow>
{
    static bool sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger) ;
    static const std::string sname() noexcept ;

    static void register_show_func(std::function<void()> func) noexcept ;
} ;


struct ExitConfigWindow : public BindedFuncWithCreator<ExitConfigWindow>
{
    static bool sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger) ;
    static const std::string sname() noexcept ;

    static void register_exit_func(std::function<void()> func) noexcept ;
} ;

#endif
