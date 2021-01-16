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

using namespace std ;

//CloseCurrentWindow
const string CloseCurrentWindow::sname() noexcept
{
    return "close_current_window" ;
}

void CloseCurrentWindow::sprocess(
        const bool first_call,
        const unsigned int UNUSED(repeat_num),
        KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr))
{
    if(!first_call) return ;
    KeybrdEventer::pushup(VKC_LALT, VKC_F4) ;
}


namespace WCtrlUtility {
    template <typename T>
    void preserve_pushup(const T vkc) {
        using namespace KeybrdEventer ;
        if(!KeyAbsorber::is_pressed(vkc)) {
            pushup(vkc) ;
            return ;
        }

        release_keystate(vkc) ;
        pushup(vkc) ;
        press_keystate(vkc) ;
    }
}

//SwitchWindow
const string SwitchWindow::sname() noexcept
{
    return "switch_window" ;
}

void SwitchWindow::sprocess(
        const bool first_call,
        const unsigned int UNUSED(repeat_num),
        KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr))
{
    using namespace KeybrdEventer ;
    if(!first_call) {
        return ;
    }

    KeyAbsorber::InstantKeyAbsorber ika ;

    static SmartKey alt(VKC_LALT) ;
    alt.press() ;
    KeyAbsorber::release_virtually(VKC_LALT) ;
    pushup(VKC_TAB) ;

    KeyLogger logger{} ;
    auto main_loop = [&logger] {
        if(!KyLgr::log_as_vkc(logger)) {
            Utility::remove_from_back(logger, 1) ;
            return ;
        }
        if(KeyBinder::is_invalid_log(logger.back(), KeyBinder::InvalidPolicy::UnbindedSystemKey)) {
            Utility::remove_from_back(logger, 1) ;
            return ;
        }

        auto matched_func = KeyBinder::find_func(logger, nullptr, false, ModeManager::Mode::Normal) ;
        if(!matched_func) {
            logger.clear() ;
            return ;
        }

        if(matched_func->is_callable()) {
            const auto name = matched_func->name() ;
            logger.clear() ;
            if(name == MoveLeft::sname()) {
                WCtrlUtility::preserve_pushup(VKC_LEFT) ;
                return ;
            }
            if(name == MoveRight::sname()) {
                WCtrlUtility::preserve_pushup(VKC_RIGHT) ;
                return ;
            }
        }
    } ;

    while(win_vind::update_background()) {
        if(KeyAbsorber::is_pressed(VKC_ESC)) {
            break ;
        }
        if(KeyAbsorber::is_pressed(VKC_ENTER)) {
            break ;
        }
        main_loop() ;
    }

    KeyAbsorber::release_virtually(VKC_ESC) ;
    KeyAbsorber::release_virtually(VKC_ENTER) ;

    alt.release() ;

    //jump cursor to a selected window after releasing alt and tab.
    Sleep(50) ; //send select-message to OS(wait)
    Jump2ActiveWindow::sprocess(true, 1, nullptr, nullptr) ;
}


//MaximizeCurrentWindow
const string MaximizeCurrentWindow::sname() noexcept
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
const string MinimizeCurrentWindow::sname() noexcept
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
const string SnapCurrentWindow2Left::sname() noexcept
{
    return "snap_current_window_to_left" ;
}

void SnapCurrentWindow2Left::sprocess(
        const bool first_call,
        const unsigned int UNUSED(repeat_num),
        KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr))
{
    if(!first_call) return ;
    KeybrdEventer::pushup(VKC_LWIN, VKC_LEFT) ;
}


//SnapCurrentWindow2Right
const string SnapCurrentWindow2Right::sname() noexcept
{
    return "snap_current_window_to_right" ;
}

void SnapCurrentWindow2Right::sprocess(
        const bool first_call,
        const unsigned int UNUSED(repeat_num),
        KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr))
{
    if(!first_call) return ;
    KeybrdEventer::pushup(VKC_LWIN, VKC_RIGHT) ;
}


//OpenNewCurrentWindow
const string OpenNewCurrentWindow::sname() noexcept
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
const string ReloadCurrentWindow::sname() noexcept
{
    return "reload_current_window" ;
}

void ReloadCurrentWindow::sprocess(
        const bool first_call,
        const unsigned int UNUSED(repeat_num),
        KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr))
{
    if(!first_call) return ;
    KeybrdEventer::pushup(VKC_F5) ;
}


//Switch2LeftTab
const string Switch2LeftTab::sname() noexcept
{
    return "switch_to_left_tab" ;
}

void Switch2LeftTab::sprocess(
        const bool first_call,
        const unsigned int repeat_num,
        KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr))
{
    if(!first_call) return ;
    for(unsigned int i = 0 ; i < repeat_num ; i ++) {
        KeybrdEventer::pushup(VKC_LCTRL, VKC_LSHIFT, VKC_TAB) ;
    }
}


//Switch2RightTab
const string Switch2RightTab::sname() noexcept
{
    return "switch_to_right_tab" ;
}

void Switch2RightTab::sprocess(
        const bool first_call,
        unsigned int repeat_num,
        KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr))
{
    if(!first_call) return ;
    for(unsigned int i = 0 ; i < repeat_num ; i ++) {
        KeybrdEventer::pushup(VKC_LCTRL, VKC_TAB) ;
    }
}

//OpenNewTab
const string OpenNewTab::sname() noexcept
{
    return "open_new_tab" ;
}

void OpenNewTab::sprocess(
        const bool first_call,
        unsigned int UNUSED(repeat_num),
        KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr))
{
    if(!first_call) return ;
    KeybrdEventer::pushup(VKC_LCTRL, VKC_T) ;
}
