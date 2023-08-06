#include "openwin.hpp"

#include <windows.h>

#include <psapi.h>

#include <memory>

#include "core/path.hpp"
#include "util/def.hpp"
#include "util/winwrap.hpp"

namespace vind
{
    namespace bind
    {
        //OpenNewWindow
        OpenNewWindow::OpenNewWindow()
        : BindedFuncVoid("open_new_window")
        {}
        void OpenNewWindow::sprocess(
                std::uint16_t UNUSED(count),
                const std::string& UNUSED(args)) {
            auto hwnd = util::get_foreground_window() ;

            DWORD proc_id = 0 ;
            GetWindowThreadProcessId(hwnd, &proc_id) ;

            HANDLE hproc_raw = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, proc_id) ;
            if(!hproc_raw) {
                throw RUNTIME_EXCEPT("OpenProcess Failed.") ;
            }

            auto close_handle = [](HANDLE handle) {CloseHandle(handle) ;} ;
            std::unique_ptr<void, decltype(close_handle)> hproc(hproc_raw, close_handle) ;

            HMODULE hmod = NULL ;
            DWORD cbneed = 0 ;
            if(!EnumProcessModules(hproc.get(), &hmod, sizeof(HMODULE), &cbneed)) {
                throw RUNTIME_EXCEPT("Cannot enumerate process modules.") ;
            }

            WCHAR path[MAX_PATH] = {0} ;
            if(!GetModuleFileNameExW(hproc.get(), hmod, path, MAX_PATH)) {
                throw RUNTIME_EXCEPT("Cannot get a process path of current window.") ;
            }

            util::create_process(core::HOME_PATH(), util::ws_to_s(path)) ;
            Sleep(100) ;
        }
    }
}
