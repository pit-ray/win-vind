#include "opt/suppress_for_vim.hpp"

#include <windows.h>

#include <string>

#include "bind/mode/change_mode.hpp"
#include "err_logger.hpp"
#include "key/key_absorber.hpp"
#include "mode.hpp"
#include "opt/virtual_cmd_line.hpp"
#include "util/string.hpp"
#include "util/winwrap.hpp"


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

        auto exename = util::get_module_filename(hwnd) ;
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
