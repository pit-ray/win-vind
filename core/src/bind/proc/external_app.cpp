#include "bind/proc/external_app.hpp"

#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>

#include <windows.h>

#include "disable_gcc_warning.hpp"

#include <nlohmann/json.hpp>

#include "enable_gcc_warning.hpp"

#include "bind/mouse/jump_actwin.hpp"
#include "bind/proc/external_app.hpp"
#include "err_logger.hpp"
#include "io/keybrd.hpp"
#include "key/char_logger.hpp"
#include "key/ntype_logger.hpp"
#include "opt/virtual_cmd_line.hpp"
#include "path.hpp"
#include "util/winwrap.hpp"


namespace
{
    using AliasCommandList = std::unordered_map<std::string, std::string> ;
    inline AliasCommandList load_proc_list_core() {
        AliasCommandList map{} ;

        nlohmann::json j ;
        std::ifstream ifs(vind::path::to_u8path(vind::path::SETTINGS())) ;
        ifs >> j ;

        for(const auto& i : j.at("exapps").at("choices")) {
            try {
                auto&& key = i.at("name").get<std::string>() ;
                auto&& val = i.at("value").get<std::string>() ;
                map[key]   = val ;
            }
            catch(const std::exception& e) {
                PRINT_ERROR(std::string(e.what()) + ", so one shortcut application is skipped.") ;
                continue ;
            }
        }
        return map ;
    }

    AliasCommandList g_proc_list{} ;
}


namespace vind
{
    namespace exapp
    {
        void load_config() {
            g_proc_list = load_proc_list_core() ;
        }
    }

    //StartShell
    StartShell::StartShell()
    : BindedFuncCreator("start_shell")
    {}
    void StartShell::sprocess() {
        try {
            util::create_process(path::HOME_PATH(), g_proc_list.at("shell")) ;
        }
        catch(const std::out_of_range&) {
            VirtualCmdLine::msgout("E: Not a command") ;
            return ;
        }

        Sleep(100) ; //wait until select window by OS.
        Jump2ActiveWindow::sprocess() ;
    }
    void StartShell::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess() ;
        }
    }
    void StartShell::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess() ;
    }


    /*
     * -- ToDo --
     * connect directly to command prompt
     *
     */

    //StartAnyApp
    StartAnyApp::StartAnyApp()
    : BindedFuncCreator("start_any_app")
    {}
    void StartAnyApp::sprocess(const std::string& cmd) {
        if(!cmd.empty()) {
            try {
                util::create_process(".", g_proc_list.at(cmd)) ;
            }
            catch(const std::out_of_range&) {
                VirtualCmdLine::msgout("E: Not a command") ;
                return ;
            }

            Sleep(100) ; //wait until select window by OS.
            Jump2ActiveWindow::sprocess() ;
        }
    }
    void StartAnyApp::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess("shell") ;
        }
    }
    void StartAnyApp::sprocess(const CharLogger& parent_lgr) {
        auto cmd = parent_lgr.to_str() ;
        sprocess(cmd.substr(1)) ;
    }

    //must fix
    void StartAnyApp::reconstruct() {
        exapp::load_config() ;
    }

    //StartExplorer
    StartExplorer::StartExplorer()
    : BindedFuncCreator("start_explorer")
    {}
    void StartExplorer::sprocess() {
        keybrd::pushup(KEYCODE_LWIN, KEYCODE_E) ;
        Sleep(100) ; //wait until select window by OS.
        Jump2ActiveWindow::sprocess() ;
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
    : BindedFuncCreator("open_start_menu")
    {}
    void OpenStartMenu::sprocess() {
        keybrd::pushup(KEYCODE_LWIN) ;
        Sleep(100) ; //wait until select window by OS.
        Jump2ActiveWindow::sprocess() ;
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
