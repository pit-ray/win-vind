#include "external_app.hpp"

#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <windows.h>

#include "disable_gcc_warning.hpp"
#include <nlohmann/json.hpp>
#include "enable_gcc_warning.hpp"

#include "jump_cursor.hpp"
#include "key_logger.hpp"
#include "msg_logger.hpp"
#include "path.hpp"
#include "utility.hpp"

#include "virtual_cmd_line.hpp"

namespace ExAppUtility
{
    using mss_t = std::unordered_map<std::string, std::string> ;
    inline static const mss_t _load_proc_list_core() {
        mss_t map{} ;

        nlohmann::json j ;
        std::ifstream ifs(Path::SETTINGS()) ;
        ifs >> j ;

        for(const auto& i : j.at("exapps").at("choices")) {
            try {
                auto&& key = i.at("name").get<std::string>() ;
                auto&& val = i.at("value").get<std::string>() ;
                map[key]   = val ;
            }
            catch(const std::exception& e) {
                ERROR_PRINT(std::string(e.what()) + ", so one shortcut application is skipped.") ;
                continue ;
            }
        }
        return map ;
    }

    static mss_t g_proc_list{} ;
    void load_config() {
        g_proc_list = _load_proc_list_core() ;
    }

    inline static const auto _cvt_to_protected_path(const std::string name) noexcept {
        try {
            const auto& origin = g_proc_list.at(name) ;
            //is origin path?
            if(origin.find("/") == std::string::npos) {
                return origin ;
            }
            if(origin.find("\\") == std::string::npos) {
                return origin ;
            }
            return "\"" + origin + "\"" ;
        }
        catch(const std::out_of_range& e) {
            VirtualCmdLine::msgout("e: Not a command") ;
            return std::string() ;
        }
    }

    inline static void _create_process(const std::string path)
    {
        STARTUPINFOA si ;
        ZeroMemory(&si, sizeof(si)) ;
        si.cb = sizeof(si) ;

        PROCESS_INFORMATION pi ;
        ZeroMemory(&pi, sizeof(pi)) ;

        if(!CreateProcessA(
            NULL, const_cast<LPSTR>(path.c_str()),
            NULL, NULL, FALSE,
            CREATE_NEW_CONSOLE, NULL,
            Path::HOME_PATH().c_str(),
            &si, &pi)) {
            throw RUNTIME_EXCEPT("Cannot call \"" + path + "\"") ;
        }
    }
}

using namespace ExAppUtility ;

//StartShell
const std::string StartShell::sname() noexcept
{
    return "start_shell" ;
}

void StartShell::sprocess(
        const bool first_call,
        const unsigned int UNUSED(repeat_num),
        KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr))
{
    if(!first_call) return ;
    _create_process("shell") ;

    Sleep(100) ; //wait until select window by OS.
    Jump2ActiveWindow::sprocess(true, 1, nullptr, nullptr) ;
}


/*
 * -- ToDo --
 * connect directly to command prompt
 *
 */

//StartAnyApp
const std::string StartAnyApp::sname() noexcept
{
    return "start_any_app" ;
}

void StartAnyApp::sprocess(
        const bool first_call,
        const unsigned int UNUSED(repeat_num),
        KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const parent_charlgr)
{
    if(!first_call) return ;

    if(!parent_charlgr)
        throw LOGIC_EXCEPT("The passed parent logger is null") ;

    auto cmd = KyLgr::log2str(*parent_charlgr) ;
    cmd = _cvt_to_protected_path(cmd.substr(1)) ;
    if(!cmd.empty()) {
        _create_process(cmd) ;

        Sleep(100) ; //wait until select window by OS.
        Jump2ActiveWindow::sprocess(true, 1, nullptr, nullptr) ;
    }
}
