#include "window_ctrl.hpp"

#include <algorithm>
#include <iostream>
#include <windows.h>
#include <psapi.h>
#include <dwmapi.h>

#include "change_mode.hpp"
#include "interval_timer.hpp"
#include "jump_cursor.hpp"
#include "key_absorber.hpp"
#include "key_binder.hpp"
#include "key_logger.hpp"
#include "keybrd_eventer.hpp"
#include "move_cursor.hpp"
#include "msg_logger.hpp"
#include "path.hpp"
#include "screen_metrics.hpp"
#include "utility.hpp"
#include "virtual_key_fwd.hpp"
#include "win_vind.hpp"

namespace WindowCtrl
{
    bool is_valid_hwnd(HWND hwnd) {
        //is movable window ? -----------
        if(hwnd == GetDesktopWindow()) {
            return false ;
        }
        if(hwnd == GetShellWindow()) {
            return false ;
        }

        //Is visible ? ------------------
        if(!IsWindowEnabled(hwnd)) {
            return false ;
        }
        if(!IsWindowVisible(hwnd)) {
            return false ;
        }
        if(IsIconic(hwnd)) { //is minimized?
            return false ;
        }

        int n_cloaked ;
        if(DwmGetWindowAttribute(hwnd, DWMWA_CLOAKED, &n_cloaked, sizeof(int)) != S_OK) {
            return false ;
        }
        if(n_cloaked) {
            return false ;
        }
        return true ;
    }

    bool is_valid_rect(HWND hwnd, RECT& rect) {
        const auto width = ScreenMetrics::width(rect) ;
        if(width == 0) {
            return false ;
        }

        const auto height = ScreenMetrics::height(rect) ;
        if(height == 0) {
            return false ;
        }

        //is full screen window ??
        RECT client_rect ;
        if(!GetClientRect(hwnd, &client_rect)) {
            return false ;
        }
        if(ScreenMetrics::is_equel(rect, client_rect)) {
            return false ;
        }

        return true ;
    }
}


//CloseCurrentWindow
const std::string CloseCurrentWindow::sname() noexcept
{
    return "close_current_window" ;
}

void CloseCurrentWindow::sprocess(
        const bool first_call,
        const unsigned int UNUSED(repeat_num),
        KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr))
{
    if(first_call) {
        KeybrdEventer::pushup(VKC_LALT, VKC_F4) ;
    }
}

//OpenNewCurrentWindow
const std::string OpenNewCurrentWindow::sname() noexcept
{
    return "open_new_current_window" ;
}

void OpenNewCurrentWindow::sprocess(
        const bool first_call,
        const unsigned int UNUSED(repeat_num),
        KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr))
{
    if(!first_call) {
        return ;
    }
    auto hwnd = GetForegroundWindow() ;
    if(!hwnd) {
        throw RUNTIME_EXCEPT("The foreground window is not existed") ;
    }

    DWORD proc_id = 0 ;
    GetWindowThreadProcessId(hwnd, &proc_id) ;

    HANDLE hproc = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, proc_id) ;
    if(!hproc) {
        throw RUNTIME_EXCEPT("OpenProcess Failed.") ;
    }

    HMODULE hmod = NULL ;
    DWORD cbneed = 0 ;
    if(!EnumProcessModules(hproc, &hmod, sizeof(HMODULE), &cbneed)) {
        CloseHandle(hproc) ;
        throw RUNTIME_EXCEPT("Cannot enumerate process modules.") ;
    }

    WCHAR path[MAX_PATH] = {0} ;
    if(!GetModuleFileNameExW(hproc, hmod, path, MAX_PATH)) {
        CloseHandle(hproc) ;
        throw RUNTIME_EXCEPT("Cannot get a process path of current window.") ;
    }
    CloseHandle(hproc) ;

    Utility::create_process(Path::HOME_PATH(), Utility::ws_to_s(path)) ;
    Sleep(100) ;
}

//ReloadCurrentWindow
const std::string ReloadCurrentWindow::sname() noexcept
{
    return "reload_current_window" ;
}

void ReloadCurrentWindow::sprocess(
        const bool first_call,
        const unsigned int UNUSED(repeat_num),
        KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr))
{
    if(first_call) {
        KeybrdEventer::pushup(VKC_F5) ;
    }
}


