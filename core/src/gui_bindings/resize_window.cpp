#include "resize_window.hpp"

#include <windows.h>

#include "keybrd_eventer.hpp"
#include "utility.hpp"
#include "jump_cursor.hpp"
#include "screen_metrics.hpp"

namespace ResizeWindow {
    static ScreenMetrics g_screen_metrics{} ;

    inline static void resize_current_window(const LONG left, const LONG top, const LONG width, const LONG height) {
        auto hwnd = GetForegroundWindow() ;
        if(hwnd == NULL) {
            throw RUNTIME_EXCEPT("There is not the foreground window.") ;
        }

        using ResizeWindow::g_screen_metrics ;
        if(!MoveWindow(hwnd, left, top, width, height, TRUE)) {
            throw RUNTIME_EXCEPT("Could not change window size") ;
        }

        Jump2ActiveWindow::sprocess(true, 1, nullptr, nullptr) ;
    }
}

//MaximizeCurrentWindow
const std::string MaximizeCurrentWindow::sname() noexcept
{
    return "maximize_current_window" ;
}

void MaximizeCurrentWindow::sprocess(
        const bool first_call,
        const unsigned int repeat_num,
        KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr))
{
    if(!first_call) return ;
    for(unsigned int i = 0 ; i < repeat_num ; i ++) {
        KeybrdEventer::pushup(VKC_LWIN, VKC_UP) ;
    }
}


//MinimizeCurrentWindow
const std::string MinimizeCurrentWindow::sname() noexcept
{
    return "minimize_current_window" ;
}

void MinimizeCurrentWindow::sprocess(
        const bool first_call,
        const unsigned int UNUSED(repeat_num),
        KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr))
{
    if(!first_call) return ;
    KeybrdEventer::pushup(VKC_LWIN, VKC_DOWN) ;
}


//SnapCurrentWindow2Left
const std::string SnapCurrentWindow2Left::sname() noexcept
{
    return "snap_current_window_to_left" ;
}

void SnapCurrentWindow2Left::sprocess(
        const bool first_call,
        const unsigned int UNUSED(repeat_num),
        KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr))
{
    if(first_call) {
        using namespace ResizeWindow ;
        resize_current_window(
                g_screen_metrics.primary_client_left(),
                g_screen_metrics.primary_client_top(),
                g_screen_metrics.primary_client_width() / 2,
                g_screen_metrics.primary_client_height()) ;
    }
}


//SnapCurrentWindow2Right
const std::string SnapCurrentWindow2Right::sname() noexcept
{
    return "snap_current_window_to_right" ;
}

void SnapCurrentWindow2Right::sprocess(
        const bool first_call,
        const unsigned int UNUSED(repeat_num),
        KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr))
{
    if(first_call) {
        using namespace ResizeWindow ;
        const auto half_of_width = g_screen_metrics.primary_client_width() / 2 ;
        resize_current_window(
                g_screen_metrics.primary_client_left() + half_of_width,
                g_screen_metrics.primary_client_top(),
                half_of_width,
                g_screen_metrics.primary_client_height()) ;
    }
}


//SnapCurrentWindow2Top
const std::string SnapCurrentWindow2Top::sname() noexcept
{
    return "snap_current_window_to_top" ;
}

void SnapCurrentWindow2Top::sprocess(
        const bool first_call,
        const unsigned int UNUSED(repeat_num),
        KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr))
{
    if(first_call) {
        using namespace ResizeWindow ;
        resize_current_window(
                g_screen_metrics.primary_client_left(),
                g_screen_metrics.primary_client_top(),
                g_screen_metrics.primary_client_width(),
                g_screen_metrics.primary_client_height() / 2) ;
    }
}

//SnapCurrentWindow2Bottom
const std::string SnapCurrentWindow2Bottom::sname() noexcept
{
    return "snap_current_window_to_bottom" ;
}

void SnapCurrentWindow2Bottom::sprocess(
        const bool first_call,
        const unsigned int UNUSED(repeat_num),
        KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr))
{
    if(first_call) {
        using namespace ResizeWindow ;
        const auto half_of_height = g_screen_metrics.primary_client_height() / 2 ;
        resize_current_window(
                g_screen_metrics.primary_client_left(),
                g_screen_metrics.primary_client_top() + half_of_height,
                g_screen_metrics.primary_client_width(),
                half_of_height) ;
    }
}

//RotateWindowInCurrentMonitor
const std::string RotateWindowInCurrentMonitor::sname() noexcept
{
    return "rotate_window_in_current_monitor" ;
}

void RotateWindowInCurrentMonitor::sprocess(
        const bool first_call,
        const unsigned int UNUSED(repeat_num),
        KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr))
{

    /* NOT IMPLEMENTED*/

}

//ExchangeWindowWithNextOne
const std::string ExchangeWindowWithNextOne::sname() noexcept
{
    return "exchange_window_with_next_one" ;
}

void ExchangeWindowWithNextOne::sprocess(
        const bool first_call,
        const unsigned int UNUSED(repeat_num),
        KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr))
{

    /* NOT IMPLEMENTED*/

}

//ArrangeWindow
const std::string ArrangeWindow::sname() noexcept
{
    return "arrange_window" ;
}

void ArrangeWindow::sprocess(
        const bool first_call,
        const unsigned int UNUSED(repeat_num),
        KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr))
{

    /* NOT IMPLEMENTED*/

}
