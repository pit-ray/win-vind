#include "bind/proc/external_app.hpp"

#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>

#include <windows.h>

#include "disable_gcc_warning.hpp"

#include <nlohmann/json.hpp>

#include "enable_gcc_warning.hpp"

#include "bind/file/explorer_util.hpp"
#include "bind/mouse/jump_actwin.hpp"
#include "bind/proc/external_app.hpp"
#include "err_logger.hpp"
#include "g_params.hpp"
#include "io/keybrd.hpp"
#include "key/char_logger.hpp"
#include "key/ntype_logger.hpp"
#include "opt/virtual_cmd_line.hpp"
#include "path.hpp"
#include "util/string.hpp"
#include "util/winwrap.hpp"


namespace
{
    using namespace vind ;

    std::string get_shell_startupdirectory() {
        auto dir = explorer::get_current_explorer_path() ;
        if(dir.empty()) {
            dir = gparams::get_s("shell_startupdir") ;
            if(dir.empty()) {
                dir = path::HOME_PATH() ;
            }
        }
        return dir ;
    }
}


namespace vind
{
    //StartShell
    StartShell::StartShell()
    : BindedFuncCreator("start_shell")
    {}
    void StartShell::sprocess() {
        util::create_process(
                get_shell_startupdirectory(),
                gparams::get_s("shell")) ;

        Sleep(100) ; //wait until the window is selectable
        JumpToActiveWindow::sprocess() ;
    }
    void StartShell::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess() ;
        }
    }
    void StartShell::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess() ;
    }


    //StartExternal
    StartExternal::StartExternal()
    : BindedFuncCreator("start_external")
    {}
    void StartExternal::sprocess(std::string cmd) {
        if(!cmd.empty()) {
            auto last_char_pos = cmd.find_last_not_of(" ") ;
            if(last_char_pos == std::string::npos) {
                StartShell::sprocess() ;
                return ;
            }

            auto shell_cmd = gparams::get_s("shell") ;
            std::string shell_cmd_flag {} ;

            auto lower_shell_cmd = util::A2a(shell_cmd) ;
            if(lower_shell_cmd == "cmd" || lower_shell_cmd== "cmd.exe") { // DOS style
                shell_cmd_flag = "/c" ;
            }
            else { // shell style
                shell_cmd_flag = gparams::get_s("shellcmdflag") ;
            }

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
                        shell_cmd, util::concat_args(shell_cmd_flag, cmd)) ;
            }

            Sleep(100) ; //wait until the window is selectable
            JumpToActiveWindow::sprocess() ;
        }
    }
    void StartExternal::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess("shell") ;
        }
    }
    void StartExternal::sprocess(const CharLogger& parent_lgr) {
        auto cmd = parent_lgr.to_str() ;
        sprocess(cmd.substr(1)) ;
    }


    //StartExplorer
    StartExplorer::StartExplorer()
    : BindedFuncCreator("start_explorer")
    {}
    void StartExplorer::sprocess() {
        keybrd::pushup(KEYCODE_LWIN, KEYCODE_E) ;
        Sleep(100) ; //wait until select window by OS.
        JumpToActiveWindow::sprocess() ;
    }
    void StartExplorer::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess() ;
        }
    }
    void StartExplorer::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess() ;
    }

    //OpenStartMenu
    OpenStartMenu::OpenStartMenu()
    : BindedFuncCreator("open_startmenu")
    {}
    void OpenStartMenu::sprocess() {
        keybrd::pushup(KEYCODE_LWIN) ;
        Sleep(100) ; //wait until select window by OS.
        JumpToActiveWindow::sprocess() ;
    }
    void OpenStartMenu::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess() ;
        }
    }
    void OpenStartMenu::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess() ;
    }
}
