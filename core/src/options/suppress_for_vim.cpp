#include "suppress_for_vim.hpp"

#include <windows.h>

#include <psapi.h>
#include <string>

#include "change_mode.hpp"
#include "mode_manager.hpp"
#include "utility.hpp"
#include "key_absorber.hpp"
#include "virtual_cmd_line.hpp"

const std::string SuppressForVim::sname() noexcept
{
    return "suppress_for_vim" ;
}

void SuppressForVim::do_enable() const
{
}

void SuppressForVim::do_disable() const
{
}

void SuppressForVim::do_process() const
{
    using namespace ModeManager ;
    if(get_mode() == Mode::Insert) {
        return ;
    }

    static HWND pre_hwnd = NULL ;

    const auto hwnd = GetForegroundWindow() ;
    if(pre_hwnd == hwnd) return ;

    pre_hwnd = hwnd ;

    DWORD procid ;
    GetWindowThreadProcessId(hwnd, &procid) ;

    auto close_proc = [] (HANDLE h) {CloseHandle(h) ;} ;
    std::unique_ptr<void, decltype(close_proc)> hproc(
            OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, procid), close_proc) ;

    char fullpath[1024] ;
    if(!GetModuleFileNameExA(hproc.get(), NULL, fullpath, 1024)) {
        return ;
    }

    std::string exename(fullpath) ;
    auto lpos = exename.find_last_of("\\") + 1 ;
    exename = Utility::A2a(exename.substr(lpos)) ;

    if(exename == "win-vind.exe") return ;

    //Whether it is vim
    if(exename.find("vim") != std::string::npos) {
        KeyAbsorber::open_all_ports() ;
        KeyAbsorber::unabsorb() ;
        change_mode(Mode::Insert) ;
        VirtualCmdLine::msgout("-- GUI INSERT --") ;
    }
    else {
        if(get_mode() == Mode::Insert)
            Change2Normal::sprocess(true, 1, nullptr, nullptr) ;
    }

}
