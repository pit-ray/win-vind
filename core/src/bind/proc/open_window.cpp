#include "bind/proc/open_window.hpp"

#include <windows.h>
#include <psapi.h>

#include "coreio/path.hpp"
#include "util/def.hpp"
#include "util/winwrap.hpp"

namespace vind
{
    //OpenNewCurrentWindow
    const std::string OpenNewCurrentWindow::sname() noexcept {
        return "open_new_current_window" ;
    }

    void OpenNewCurrentWindow::sprocess(
            bool first_call,
            unsigned int UNUSED(repeat_num),
            KeycodeLogger* const UNUSED(parent_keycodelgr),
            const CharLogger* const UNUSED(parent_charlgr)) {
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

        util::create_process(path::HOME_PATH(), util::ws_to_s(path)) ;
        Sleep(100) ;
    }
}
