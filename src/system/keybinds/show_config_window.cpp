#include "show_config_window.hpp"

#include <windows.h>

#include "jump_cursor.hpp"

namespace SCWUtility
{
    std::function<void()> show_func{} ;
}

bool ShowConfigWindow::sprocess(const std::string cmd)
{
    SCWUtility::show_func() ;

    Sleep(50) ; //wait until opened window.
    if(!Jump2ActiveWindow::sprocess(true)) {
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
    SCWUtility::show_func = std::move(func) ;
}