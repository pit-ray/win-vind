#include "window_ctrl.hpp"

#include <iostream>
#include <windows.h>
#include <psapi.h>

#include "keybrd_eventer.hpp"
#include "interval_timer.hpp"
#include "key_logger.hpp"
#include "key_absorber.hpp"
#include "utility.hpp"

#include "move_cursor.hpp"
#include "jump_cursor.hpp"
#include "msg_logger.hpp"

#include "change_mode.hpp"

using namespace std ;

//CloseCurrentWindow
const string CloseCurrentWindow::sname() noexcept
{
    return "close_current_window" ;
}

bool CloseCurrentWindow::common_process()
{
    if(!KeybrdEventer::pushup(VKC_LALT, VKC_F4)) {
        return false ;
    }
    return true ;
}

bool CloseCurrentWindow::sprocess(const bool first_call)
{
    if(!first_call) return true ;
    return common_process() ;
}

bool CloseCurrentWindow::sprocess(const string UNUSED(cmd))
{
    return common_process() ;
}


//SwitchWindow
const string SwitchWindow::sname() noexcept
{
    return "switch_window" ;
}

bool SwitchWindow::common_process()
{
    using namespace KeybrdEventer ;

    //if funcs is not initialized, these funcs is always uncallable.
    //and, I do not want to use process() of func, but refer key map.
    const auto left_pbf = MoveLeft::create_with_cache() ;
    const auto right_pbf = MoveRight::create_with_cache() ;

    auto palt = make_unique<SmartKey>(VKC_LALT) ;
    auto ptab = make_unique<SmartKey>(VKC_TAB) ;

    if(!palt->press()) {
        return false ;
    }

    if(!ptab->press()) {
        return false ;
    }
    ptab.reset() ; // release key

    KeyLogger logger{} ;

    auto select = [&logger] (const auto& pbf, const auto vkc) {
        //is callable?
        if(pbf->is_callable()) {
            if(KeyAbsorber::is_pressed(vkc)) {
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
            }
            else {
                if(!pushup(vkc)) {
                    return false ;
                }
            }
            logger.clear() ;
        }

        return true ;
    } ;

    MSG msg ;
    while(true) {
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

        //is changed ?
        if(!logger.is_changed_code()) {
            logger.remove_from_back(1) ;
            Sleep(5) ;
            continue ;
        }

        //inputed keys include key map of MoveLeft or MoveRight.
        auto at_least_exist = false ;
        if(left_pbf->matched_num(logger.back(), logger.size() - 1)) {
            at_least_exist = true ;
        }
        if(right_pbf->matched_num(logger.back(), logger.size() - 1)) {
            at_least_exist = true ;
        }
        if(!at_least_exist) {
            logger.clear() ;
            Sleep(5) ;
            continue ;
        }

        //select window
        if(!select(left_pbf, VKC_LEFT)) {
            return false ;
        }

        if(!select(right_pbf, VKC_RIGHT)) {
            return false ;
        }

        Sleep(5) ;
    }

    palt.reset() ;
    ptab.reset() ;

    //jump cursor to a selected window after releasing alt and tab.
    Sleep(50) ; //send select-message to OS(wait)
    if(!Jump2ActiveWindow::sprocess(true)) {
        return false ;
    }

    return true ;
}

bool SwitchWindow::sprocess(const bool first_call)
{
    if(!first_call) return true ;
    return common_process() ;
}

bool SwitchWindow::sprocess(const string UNUSED(cmd))
{
    if(!KeybrdEventer::release_keystate(VKC_ENTER)) {
        return false ;
    }
    return common_process() ;
}


//MaximizeCurrentWindow
const string MaximizeCurrentWindow::sname() noexcept
{
    return "maximize_current_window" ;
}

bool MaximizeCurrentWindow::common_process()
{
    return KeybrdEventer::pushup(VKC_LWIN, VKC_UP) ;
}

bool MaximizeCurrentWindow::sprocess(const bool first_call)
{
    if(!first_call) return true ;
    return common_process() ;
}

bool MaximizeCurrentWindow::sprocess(const string UNUSED(cmd))
{
    return common_process() ;
}


//MinimizeCurrentWindow
const string MinimizeCurrentWindow::sname() noexcept
{
    return "minimize_current_window" ;
}

bool MinimizeCurrentWindow::common_process()
{
    return KeybrdEventer::pushup(VKC_LWIN, VKC_DOWN) ;
}

bool MinimizeCurrentWindow::sprocess(const bool first_call)
{
    if(!first_call) return true ;
    return common_process() ;
}

bool MinimizeCurrentWindow::sprocess(const string UNUSED(cmd))
{
    return common_process() ;
}


//SnapCurrentWindow2Left
const string SnapCurrentWindow2Left::sname() noexcept
{
    return "snap_current_window_to_left" ;
}

bool SnapCurrentWindow2Left::common_process()
{
    return KeybrdEventer::pushup(VKC_LWIN, VKC_LEFT) ;
}

bool SnapCurrentWindow2Left::sprocess(const bool first_call)
{
    if(!first_call) return true ;
    return common_process() ;
}

bool SnapCurrentWindow2Left::sprocess(const string UNUSED(cmd))
{
    return common_process() ;
}


//SnapCurrentWindow2Right
const string SnapCurrentWindow2Right::sname() noexcept
{
    return "snap_current_window_to_right" ;
}

bool SnapCurrentWindow2Right::common_process()
{
    return KeybrdEventer::pushup(VKC_LWIN, VKC_RIGHT) ;
}

bool SnapCurrentWindow2Right::sprocess(const bool first_call)
{
    if(!first_call) return true ;
    return common_process() ;
}

bool SnapCurrentWindow2Right::sprocess(const string UNUSED(cmd))
{
    return common_process() ;
}


//OpenNewCurrentWindow
const string OpenNewCurrentWindow::sname() noexcept
{
    return "open_new_current_window" ;
}

bool OpenNewCurrentWindow::common_process()
{
    auto hwnd = GetForegroundWindow() ;
    if(!hwnd) {
        return false ;
    }

    DWORD proc_id = 0 ;
    GetWindowThreadProcessId(hwnd, &proc_id) ;

    HANDLE hproc = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, proc_id) ;
    if(!hproc) {
        WIN_ERROR_STREAM << "(OpenNewCurrentWindow::OpenProcess)\n" ;
        return false ;
    }

    HMODULE hmod = NULL ;
    DWORD cbneed = 0 ;
    if(!EnumProcessModules(hproc, &hmod, sizeof(HMODULE), &cbneed)) {
        WIN_ERROR_STREAM << "(OpenNewCurrentWindow::EnumProcessModules)\n" ;
        return false ;
    }

    TCHAR path[MAX_PATH] = {0} ;
    if(!GetModuleFileNameEx(hproc, hmod, path, MAX_PATH)) {
        WIN_ERROR_STREAM << "cannot get a process path of current window (OpenNewCurrentWindow::common_process::GetModuleFileNameEx)\n" ;
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
        WIN_ERROR_STREAM << ", cannot call \"" << path << "\"" << " (OpenNewCurrentWindow:::common_process::CreateProcess)\n" ;
        return false ;
    }
    return true ;
}

bool OpenNewCurrentWindow::sprocess(const bool first_call)
{
    if(!first_call) return true ;
    return common_process() ;
}

bool OpenNewCurrentWindow::sprocess(const string UNUSED(cmd))
{
    return common_process() ;
}


//ReloadCurrentWindow
const string ReloadCurrentWindow::sname() noexcept
{
    return "reload_current_window" ;
}

bool ReloadCurrentWindow::sprocess(const string UNUSED(cmd))
{
    return KeybrdEventer::pushup(VKC_F5) ;
}
