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
#include "bind/base/ntype_logger.hpp"
#include "bind/base/char_logger.hpp"
#include "coreio/err_logger.hpp"
#include "coreio/path.hpp"
#include "io/keybrd.hpp"
#include "opt/virtual_cmd_line.hpp"
#include "util/winwrap.hpp"


namespace
{
    using mss_t = std::unordered_map<std::string, std::string> ;
    inline const mss_t _load_proc_list_core() {
        mss_t map{} ;

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

    mss_t g_proc_list{} ;
}


namespace vind
{
    namespace exapp
    {
        void load_config() {
            g_proc_list = _load_proc_list_core() ;
        }
    }

    //StartShell
    const std::string StartShell::sname() noexcept {
        return "start_shell" ;
    }
    void StartShell::sprocess() {
        try {
            util::create_process(path::HOME_PATH(), g_proc_list.at("shell")) ;
        }
        catch(const std::out_of_range&) {
            VirtualCmdLine::msgout("e: Not a command") ;
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
    const std::string StartAnyApp::sname() noexcept {
        return "start_any_app" ;
    }
    void StartAnyApp::sprocess(std::string cmd) {
        if(!cmd.empty()) {
            try {
                util::create_process(".", g_proc_list.at(cmd)) ;
            }
            catch(const std::out_of_range&) {
                VirtualCmdLine::msgout("e: Not a command") ;
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
    void StartAnyApp::load_config() {
        exapp::load_config() ;
    }

    //StartExplorer
    const std::string StartExplorer::sname() noexcept {
        return "start_explorer" ;
    }
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
    const std::string OpenStartMenu::sname() noexcept {
        return "open_start_menu" ;
    }
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
