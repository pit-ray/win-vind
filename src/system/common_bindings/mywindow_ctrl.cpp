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
bool ShowConfigWindow::sprocess(const bool first_call, const unsigned int UNUSED(repeat_num), const KeyLogger* const UNUSED(parent_logger))
{
    if(!first_call) return true ;

    MYWUtility::show_func() ;

    Sleep(50) ; //wait until opened window.
    if(!Jump2ActiveWindow::sprocess(true, 1, nullptr)) {
        return false ;
    }
    return true ;
}

const std::string ShowConfigWindow::sname() noexcept
{
    return "show_config_window" ;
}

void ShowConfigWindow::register_show_func(std::function<void()> func) noexcept
{
    MYWUtility::show_func = std::move(func) ;
}


//ExitConfigWindow
bool ExitConfigWindow::sprocess(const bool first_call, const unsigned int UNUSED(repeat_num), const KeyLogger* const UNUSED(parent_logger))
{
    if(!first_call) return true ;
    MYWUtility::exit_func() ;
    return true ;
}

const std::string ExitConfigWindow::sname() noexcept
{
    return "exit_config_window" ;
}

void ExitConfigWindow::register_exit_func(std::function<void()> func) noexcept
{
    MYWUtility::exit_func = std::move(func) ;
}
