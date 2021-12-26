#include "exapp.hpp"

#include <fstream>
#include <string>
#include <unordered_map>

#include <windows.h>

#include "bind/file/explorer_util.hpp"
#include "bind/mouse/jump_actwin.hpp"
#include "core/charlogger.hpp"
#include "core/errlogger.hpp"
#include "core/inputgate.hpp"
#include "core/ntypelogger.hpp"
#include "core/path.hpp"
#include "core/settable.hpp"
#include "exapp.hpp"
#include "opt/vcmdline.hpp"
#include "util/debug.hpp"
#include "util/string.hpp"
#include "util/winwrap.hpp"


namespace
{
    using namespace vind ;

    std::filesystem::path get_shell_startupdirectory() {
        auto& settable = core::SetTable::get_instance() ;
        auto dir = settable.get("shell_startupdir").get<std::string>() ;
        if(!dir.empty()) {
            std::filesystem::path dir_path(dir) ;
            dir_path.make_preferred() ;
            return dir_path ;
        }

        try {
            auto dir_path = bind::get_current_explorer_path() ;
            if(!dir_path.empty()) {
                return dir_path ;
            }
        }
        catch(const std::exception&) {
            // If you open QuickAccess Page, the path getter will occur exception.
            return core::HOME_PATH() ;
        }
        return core::HOME_PATH() ;
    }
}


namespace vind
{
    namespace bind
    {
        //StartShell
        StartShell::StartShell()
        : BindedFuncVoid("start_shell")
        {}
        void StartShell::sprocess() {
            auto& settable = core::SetTable::get_instance() ;
            util::create_process(
                    get_shell_startupdirectory(),
                    settable.get("shell").get<std::string>()) ;

            Sleep(100) ; //wait until the window is selectable
            JumpToActiveWindow::sprocess() ;
        }
        void StartShell::sprocess(core::NTypeLogger& parent_lgr) {
            if(!parent_lgr.is_long_pressing()) {
                sprocess() ;
            }
        }
        void StartShell::sprocess(const core::CharLogger& UNUSED(parent_lgr)) {
            sprocess() ;
        }


        //StartExternal
        StartExternal::StartExternal()
        : BindedFuncVoid("start_external")
        {}
        void StartExternal::sprocess(std::string cmd) {
            if(cmd.empty()) {
                return ;
            }

            auto& settable = core::SetTable::get_instance() ;
            auto last_char_pos = cmd.find_last_not_of(" ") ;
            if(last_char_pos == std::string::npos) {
                StartShell::sprocess() ;
                return ;
            }

            auto shell_cmd = settable.get("shell").get<std::string>() ;
            std::string shell_cmd_flag {} ;

            auto lower_shell_cmd = util::A2a(shell_cmd) ;
            if(lower_shell_cmd == "cmd" || lower_shell_cmd== "cmd.exe") { // DOS style
                shell_cmd_flag = "/c" ;
            }
            else { // shell style
                shell_cmd_flag = settable.get("shellcmdflag").get<std::string>() ;
            }

            cmd = core::replace_path_magic(cmd) ;

            if(cmd[last_char_pos] == ';') { //keep console window
                cmd.erase(last_char_pos) ;

                // wrap a command with "pause" to keep console window instead of vimrun.exe.
                util::create_process(
                        get_shell_startupdirectory(),
                        "cmd", util::concat_args("/c",
                        shell_cmd, shell_cmd_flag, cmd,
                        "& pause")) ;
            }
            else {
                util::create_process(
                        get_shell_startupdirectory(),
                        shell_cmd, util::concat_args(shell_cmd_flag, cmd), false) ;
            }

            Sleep(100) ; //wait until the window is selectable
            JumpToActiveWindow::sprocess() ;
        }
        void StartExternal::sprocess(core::NTypeLogger& parent_lgr) {
            if(!parent_lgr.is_long_pressing()) {
                sprocess("shell") ;
            }
        }
        void StartExternal::sprocess(const core::CharLogger& parent_lgr) {
            auto cmd = parent_lgr.to_str() ;
            sprocess(cmd.substr(1)) ;
        }


        //StartExplorer
        StartExplorer::StartExplorer()
        : BindedFuncVoid("start_explorer")
        {}
        void StartExplorer::sprocess() {
            core::InputGate::get_instance().pushup(
                    KEYCODE_LWIN, KEYCODE_E) ;
            Sleep(100) ; //wait until select window by OS.
            JumpToActiveWindow::sprocess() ;
        }
        void StartExplorer::sprocess(core::NTypeLogger& parent_lgr) {
            if(!parent_lgr.is_long_pressing()) {
                sprocess() ;
            }
        }
        void StartExplorer::sprocess(const core::CharLogger& UNUSED(parent_lgr)) {
            sprocess() ;
        }

        //OpenStartMenu
        OpenStartMenu::OpenStartMenu()
        : BindedFuncVoid("open_startmenu")
        {}
        void OpenStartMenu::sprocess() {
            core::InputGate::get_instance().pushup(KEYCODE_LWIN) ;
            Sleep(100) ; //wait until select window by OS.
            JumpToActiveWindow::sprocess() ;
        }
        void OpenStartMenu::sprocess(core::NTypeLogger& parent_lgr) {
            if(!parent_lgr.is_long_pressing()) {
                sprocess() ;
            }
        }
        void OpenStartMenu::sprocess(const core::CharLogger& UNUSED(parent_lgr)) {
            sprocess() ;
        }
    }
}
