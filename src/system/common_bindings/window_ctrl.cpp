#include "window_ctrl.hpp"

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
#include "utility.hpp"

using namespace std ;

//CloseCurrentWindow
const string CloseCurrentWindow::sname() noexcept
{
    return "close_current_window" ;
}

bool CloseCurrentWindow::sprocess(const bool first_call, const unsigned int UNUSED(repeat_num), const KeyLogger* const UNUSED(parent_logger))
{
    if(!first_call) return true ;
    return KeybrdEventer::pushup(VKC_LALT, VKC_F4) ;
}


//SwitchWindow

namespace WCtrlUtility {
    template <typename T>
    auto preserve_pushup(const T vkc) {
        using namespace KeybrdEventer ;
        if(!KeyAbsorber::is_pressed(vkc)) {
            return pushup(vkc) ;
        }

        if(!release_keystate(vkc)) {
            return false ;
        }
        if(!pushup(vkc)) {
            return false ;
        }
        //undo
        if(!press_keystate(vkc)) {
            return false ;
        }
        return true ;
    }
}
const string SwitchWindow::sname() noexcept
{
    return "switch_window" ;
}

bool SwitchWindow::sprocess(const bool first_call, const unsigned int UNUSED(repeat_num), const KeyLogger* const UNUSED(parent_logger))
{
    if(!first_call) return true ;
    using namespace KeybrdEventer ;

    //reset all keystate
    for(const auto& vkc : KeyAbsorber::get_pressed_list()) {
        if(!KeybrdEventer::release_keystate(vkc)) {
            return false ;
        }
    }

    static SmartKey alt(VKC_LALT) ;
    if(!alt.press()) return false ;
    KeyAbsorber::release_vertually(VKC_LALT) ;

    if(!pushup(VKC_TAB)) return false ;

    KeyLogger logger{} ;
    auto main_loop = [&logger] {
        if(!logger.is_changed_code()) {
            logger.remove_from_back(1) ;
            return true ;
        }
        if(KeyBinder::is_invalid_log(logger, KeyBinder::InvalidPolicy::UnbindedSystemKey)) {
            logger.remove_from_back(1) ;
            return true ;
        }

        auto matched_func = KeyBinder::find_keybinds(logger, nullptr, ModeManager::Mode::Normal) ;
        if(!matched_func) {
            logger.clear() ;
            return true ;
        }

        if(matched_func->is_callable()) {
            const auto name = matched_func->name() ;
            logger.clear() ;
            if(name == MoveLeft::sname()) {
                return WCtrlUtility::preserve_pushup(VKC_LEFT) ;
            }
            if(name == MoveRight::sname()) {
                return WCtrlUtility::preserve_pushup(VKC_RIGHT) ;
            }
        }
        return true ;
    } ;

    MSG msg ;
    while(main_loop()) {
        //MessageRoop
        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg) ;
            DispatchMessage(&msg) ;
        }

        //check system keys
        if(KeyAbsorber::is_pressed(VKC_ESC)) {
            if(!release_keystate(VKC_ESC)) {
                return false;
            }
            break ;
        }
        if(KeyAbsorber::is_pressed(VKC_ENTER)) {
            if(!release_keystate(VKC_ENTER)) {
                return false ;
            }
            break ;
        }
        Sleep(5) ;
    }

    if(!alt.release()) {
        return false ;
    }

    //jump cursor to a selected window after releasing alt and tab.
    Sleep(50) ; //send select-message to OS(wait)
    return Jump2ActiveWindow::sprocess(true, 1, nullptr) ;
}


//MaximizeCurrentWindow
const string MaximizeCurrentWindow::sname() noexcept
{
    return "maximize_current_window" ;
}

bool MaximizeCurrentWindow::sprocess(const bool first_call, const unsigned int UNUSED(repeat_num), const KeyLogger* const UNUSED(parent_logger))
{
    if(!first_call) return true ;
    return KeybrdEventer::pushup(VKC_LWIN, VKC_UP) ;
}


//MinimizeCurrentWindow
const string MinimizeCurrentWindow::sname() noexcept
{
    return "minimize_current_window" ;
}

bool MinimizeCurrentWindow::sprocess(const bool first_call, const unsigned int UNUSED(repeat_num), const KeyLogger* const UNUSED(parent_logger))
{
    if(!first_call) return true ;
    return KeybrdEventer::pushup(VKC_LWIN, VKC_DOWN) ;
}


//SnapCurrentWindow2Left
const string SnapCurrentWindow2Left::sname() noexcept
{
    return "snap_current_window_to_left" ;
}

bool SnapCurrentWindow2Left::sprocess(const bool first_call, const unsigned int UNUSED(repeat_num), const KeyLogger* const UNUSED(parent_logger))
{
    if(!first_call) return true ;
    return KeybrdEventer::pushup(VKC_LWIN, VKC_LEFT) ;
}


//SnapCurrentWindow2Right
const string SnapCurrentWindow2Right::sname() noexcept
{
    return "snap_current_window_to_right" ;
}

bool SnapCurrentWindow2Right::sprocess(const bool first_call, const unsigned int UNUSED(repeat_num), const KeyLogger* const UNUSED(parent_logger))
{
    if(!first_call) return true ;
    return KeybrdEventer::pushup(VKC_LWIN, VKC_RIGHT) ;
}


//OpenNewCurrentWindow
const string OpenNewCurrentWindow::sname() noexcept
{
    return "open_new_current_window" ;
}

bool OpenNewCurrentWindow::sprocess(const bool first_call, const unsigned int UNUSED(repeat_num), const KeyLogger* const UNUSED(parent_logger))
{
    if(!first_call) return true ;
    auto hwnd = GetForegroundWindow() ;
    if(!hwnd) {
        return false ;
    }

    DWORD proc_id = 0 ;
    GetWindowThreadProcessId(hwnd, &proc_id) ;

    HANDLE hproc = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, proc_id) ;
    if(!hproc) {
        WIN_ERROR_PRINT("cannot open process") ;
        return false ;
    }

    HMODULE hmod = NULL ;
    DWORD cbneed = 0 ;
    if(!EnumProcessModules(hproc, &hmod, sizeof(HMODULE), &cbneed)) {
        WIN_ERROR_PRINT("cannot enumerate process modules") ;
        return false ;
    }

    TCHAR path[MAX_PATH] = {0} ;
    if(!GetModuleFileNameEx(hproc, hmod, path, MAX_PATH)) {
        WIN_ERROR_PRINT("cannot get a process path of current window") ;
        return false ;
    }

    CloseHandle(hproc) ;

    STARTUPINFO si ;
    ZeroMemory(&si, sizeof(si)) ;
    si.cb = sizeof(si) ;

    PROCESS_INFORMATION pi ;
    ZeroMemory(&pi, sizeof(pi)) ;

    if(!CreateProcess(
        NULL, path, NULL, NULL, FALSE,
        CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {
        WIN_ERROR_PRINT(", cannot call \"" + std::string(path) + "\"") ;
        return false ;
    }
    return true ;
}


//ReloadCurrentWindow
const string ReloadCurrentWindow::sname() noexcept
{
    return "reload_current_window" ;
}

bool ReloadCurrentWindow::sprocess(const bool first_call, const unsigned int UNUSED(repeat_num), const KeyLogger* const UNUSED(parent_logger))
{
    if(!first_call) return true ;
    return KeybrdEventer::pushup(VKC_F5) ;
}


//Switch2LeftTab
const string Switch2LeftTab::sname() noexcept
{
    return "switch_to_left_tab" ;
}

bool Switch2LeftTab::sprocess(const bool first_call, const unsigned int UNUSED(repeat_num), const KeyLogger* const UNUSED(parent_logger))
{
    if(!first_call) return true ;
    return KeybrdEventer::pushup(VKC_LCTRL, VKC_LSHIFT, VKC_TAB) ;
}


//Switch2RightTab
const string Switch2RightTab::sname() noexcept
{
    return "switch_to_right_tab" ;
}

bool Switch2RightTab::sprocess(const bool first_call, const unsigned int UNUSED(repeat_num), const KeyLogger* const UNUSED(parent_logger))
{
    if(!first_call) return true ;
    return KeybrdEventer::pushup(VKC_LCTRL, VKC_TAB) ;
}
