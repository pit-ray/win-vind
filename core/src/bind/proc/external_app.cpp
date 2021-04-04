#include "external_app.hpp"

#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>

#include <windows.h>

#include "disable_gcc_warning.hpp"
#include <nlohmann/json.hpp>
#include "enable_gcc_warning.hpp"

#include "io/keybrd.hpp"
#include "mouse/jump_actwin.hpp"
#include "err_logger.hpp"
#include "opt/virtual_cmd_line.hpp"
#include "path.hpp"
#include "proc/external_app.hpp"
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

    void StartShell::sprocess(
            bool first_call,
            unsigned int UNUSED(repeat_num),
            KeycodeLogger* const UNUSED(parent_keycodelgr),
            const CharLogger* const UNUSED(parent_charlgr)) {
        if(!first_call) return ;
        try {

            util::create_process(path::HOME_PATH(), g_proc_list.at("shell")) ;
        }
        catch(const std::out_of_range&) {
            VirtualCmdLine::msgout("e: Not a command") ;
            return ;
        }

        Sleep(100) ; //wait until select window by OS.
        Jump2ActiveWindow::sprocess(true, 1, nullptr, nullptr) ;
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

    void StartAnyApp::sprocess(
            bool first_call,
            unsigned int UNUSED(repeat_num),
            KeycodeLogger* const UNUSED(parent_keycodelgr),
            const CharLogger* const parent_charlgr) {
        if(!first_call) return ;

        if(!parent_charlgr)
            throw LOGIC_EXCEPT("The passed parent logger is null") ;

        auto cmd = parent_charlgr->to_str() ;
        if(!cmd.empty()) {
            try {
                util::create_process(".", g_proc_list.at(cmd.substr(1))) ;
            }
            catch(const std::out_of_range&) {
                VirtualCmdLine::msgout("e: Not a command") ;
                return ;
            }

            Sleep(100) ; //wait until select window by OS.
            Jump2ActiveWindow::sprocess(true, 1, nullptr, nullptr) ;
        }
    }

    //must fix
    void StartAnyApp::load_config() {
        exapp::load_config() ;
    }

    //StartExplorer
    const std::string StartExplorer::sname() noexcept {
        return "start_explorer" ;
    }

    void StartExplorer::sprocess(
            bool first_call,
            unsigned int UNUSED(repeat_num),
            KeycodeLogger* const UNUSED(parent_keycodelgr),
            const CharLogger* const UNUSED(parent_charlgr)) {
        if(!first_call) return ;
        //_create_process("explorer") ;
        keybrd::pushup(KEYCODE_LWIN, KEYCODE_E) ;

        Sleep(100) ; //wait until select window by OS.
        Jump2ActiveWindow::sprocess(true, 1, nullptr, nullptr) ;
    }

    //OpenStartMenu
    const std::string OpenStartMenu::sname() noexcept {
        return "open_start_menu" ;
    }

    void OpenStartMenu::sprocess(
            bool first_call,
            unsigned int UNUSED(repeat_num),
            KeycodeLogger* const UNUSED(parent_keycodelgr),
            const CharLogger* const UNUSED(parent_charlgr)) {
        if(!first_call) return ;
        keybrd::pushup(KEYCODE_LWIN) ;
        Sleep(100) ; //wait until select window by OS.
        Jump2ActiveWindow::sprocess(true, 1, nullptr, nullptr) ;
    }
}
