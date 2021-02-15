#include "window_ctrl.hpp"

#include <algorithm>
#include <iostream>
#include <windows.h>
#include <psapi.h>

#include "change_mode.hpp"
#include "interval_timer.hpp"
#include "jump_cursor.hpp"
#include "key_absorber.hpp"
#include "key_binder.hpp"
#include "key_logger.hpp"
#include "keybrd_eventer.hpp"
#include "move_cursor.hpp"
#include "msg_logger.hpp"
#include "win_vind.hpp"
#include "utility.hpp"
#include "virtual_key_fwd.hpp"

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

    char path[MAX_PATH] = {0} ;
    if(!GetModuleFileNameExA(hproc, hmod, path, MAX_PATH)) {
        CloseHandle(hproc) ;
        throw RUNTIME_EXCEPT("Cannot get a process path of current window.") ;
    }
    CloseHandle(hproc) ;

    STARTUPINFOA si ;
    ZeroMemory(&si, sizeof(si)) ;
    si.cb = sizeof(si) ;

    PROCESS_INFORMATION pi ;
    ZeroMemory(&pi, sizeof(pi)) ;

    if(!CreateProcessA(
        NULL, path, NULL, NULL, FALSE,
        CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {
        throw RUNTIME_EXCEPT("Cannot call \"" + std::string(path) + "\".") ;
    }
    Sleep(50) ;
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
