#include "opt/suppress_for_vim.hpp"

#include <windows.h>

#include <psapi.h>
#include <string>

#include "bind/mode/change_mode.hpp"
#include "err_logger.hpp"
#include "key/key_absorber.hpp"
#include "mode.hpp"
#include "opt/virtual_cmd_line.hpp"
#include "util/string.hpp"

namespace vind
{
    std::string SuppressForVim::sname() noexcept {
        return "suppress_for_vim" ;
    }

    void SuppressForVim::do_enable() const {
    }

    void SuppressForVim::do_disable() const {
    }

    void SuppressForVim::do_process() const {
        using namespace mode ;
        if(get_global_mode() == Mode::RESIDENT) {
            return ;
        }

        static HWND pre_hwnd = NULL ;

        auto hwnd = GetForegroundWindow() ;
        if(pre_hwnd == hwnd) return ;

        pre_hwnd = hwnd ;

        DWORD procid ;
        GetWindowThreadProcessId(hwnd, &procid) ;

        auto close_proc = [] (HANDLE h) {CloseHandle(h) ;} ;
        std::unique_ptr<void, decltype(close_proc)> hproc(
                OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, procid), close_proc) ;

        WCHAR fullpath[MAX_PATH] ;
        if(!GetModuleFileNameExW(hproc.get(), NULL, fullpath, 1024)) {
            return ;
        }

        auto exename = util::ws_to_s(fullpath) ;
        auto lpos = exename.find_last_of("\\") + 1 ;
        exename = util::A2a(exename.substr(lpos)) ;

        if(exename == "win-vind.exe") return ;

        //Whether it is vim
        if(exename.find("vim") != std::string::npos) {
            keyabsorber::close_all_ports() ;
            keyabsorber::unabsorb() ;
            set_global_mode(Mode::RESIDENT) ;
            VirtualCmdLine::msgout("-- RESIDENT --") ;
        }
        else {
            if(get_global_mode() == Mode::RESIDENT)
                ToInsert::sprocess(true) ;
        }

    }
}
