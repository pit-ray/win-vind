#include "opt/suppress_for_vim.hpp"

#include <windows.h>

#include <string>

#include "bind/mode/change_mode.hpp"
#include "core/err_logger.hpp"
#include "core/key_absorber.hpp"
#include "core/mode.hpp"
#include "opt/vcmdline.hpp"
#include "util/string.hpp"
#include "util/winwrap.hpp"


namespace vind
{
    namespace opt
    {
        SuppressForVim::SuppressForVim()
        : OptionCreator("suppress_for_vim")
        {}

        void SuppressForVim::do_enable() const {
        }

        void SuppressForVim::do_disable() const {
        }

        void SuppressForVim::do_process() const {
            if(core::get_global_mode() == core::Mode::RESIDENT) {
                return ;
            }

            static HWND pre_hwnd = NULL ;

            auto hwnd = GetForegroundWindow() ;
            if(pre_hwnd == hwnd) return ;

            pre_hwnd = hwnd ;

            auto exename = util::A2a(util::get_module_filename(hwnd)) ;

            if(exename == "win-vind.exe") return ;

            //Whether it is vim
            if(exename.find("vim") != std::string::npos) {
                core::close_all_ports() ;
                core::unabsorb() ;
                core::set_global_mode(core::Mode::RESIDENT) ;
                opt::VCmdLine::print(GeneralMessage("-- RESIDENT --")) ;
            }
            else {
                if(core::get_global_mode() == core::Mode::RESIDENT)
                    bind::ToInsert::sprocess(true) ;
            }
        }
    }
}
