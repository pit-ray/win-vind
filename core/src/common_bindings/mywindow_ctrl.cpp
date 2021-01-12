#include "mywindow_ctrl.hpp"

#include <windows.h>

#include "jump_cursor.hpp"
#include "utility.hpp"

namespace MYWUtility
{
    std::function<void()> show_func = [] {return ;} ;
    std::function<void()> exit_func = [] {return ;} ;
}

//ShowConfigWindow
const std::string ShowConfigWindow::sname() noexcept
{
    return "show_config_window" ;
}
void ShowConfigWindow::sprocess(
        const bool first_call,
        const unsigned int UNUSED(repeat_num),
        KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr))
{
    if(!first_call) return ;
    MYWUtility::show_func() ;
    Sleep(50) ; //wait until opened window.
    Jump2ActiveWindow::sprocess(true, 1, nullptr, nullptr) ;
}
void ShowConfigWindow::register_show_func(std::function<void()> func) noexcept
{
    MYWUtility::show_func = std::move(func) ;
}


//ExitConfigWindow
const std::string ExitConfigWindow::sname() noexcept
{
    return "exit_config_window" ;
}
void ExitConfigWindow::sprocess(
        const bool first_call,
        const unsigned int UNUSED(repeat_num),
        KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr))
{
    if(!first_call) return ;
    MYWUtility::exit_func() ;
}
void ExitConfigWindow::register_exit_func(std::function<void()> func) noexcept
{
    MYWUtility::exit_func = std::move(func) ;
}
