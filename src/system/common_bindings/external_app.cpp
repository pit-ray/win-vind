#include "external_app.hpp"

#include <unordered_map>
#include <string>
#include <iostream>
#include <fstream>
#include <windows.h>

#include "disable_gcc_warning.hpp"
#include <nlohmann/json.hpp>
#include "enable_gcc_warning.hpp"

#include "msg_logger.hpp"
#include "path.hpp"
#include "jump_cursor.hpp"
#include "utility.hpp"
#include "key_logger.hpp"

using namespace std ;

namespace ExAppUtility
{
    using mss_t = unordered_map<string, string> ;

    inline static const mss_t _load_proc_list_core() {
        mss_t map{} ;

        nlohmann::json j ;
        std::ifstream ifs(Path::SETTINGS()) ;
        ifs >> j ;

        for(const auto& i : j.at("exapps").at("choices")) {
            try {
                auto&& key = i.at("name").get<std::string>() ;
                auto&& val = i.at("value").get<std::string>() ;
                map[key] = val ;
            }
            catch(const std::exception& e) {
                ERROR_PRINT(std::string(e.what()) + ", so one shortcut application is skipped") ;
                continue ;
            }
        }
        return map ;
    }

    static auto proc_list{_load_proc_list_core()} ;

    void load_config() {
        proc_list = _load_proc_list_core() ;
    }

    inline static const auto _get_protected_path(const string name) {
        const auto& origin = proc_list.at(name) ;
        //is origin path?
        if(origin.find("/") == string::npos) {
            return origin ;
        }
        if(origin.find("\\") == string::npos) {
            return origin ;
        }
        return "\"" + origin + "\"" ;
    }

    inline static bool _create_process(const string path)
    {
        STARTUPINFOA si ;
        ZeroMemory(&si, sizeof(si)) ;
        si.cb = sizeof(si) ;

        PROCESS_INFORMATION pi ;
        ZeroMemory(&pi, sizeof(pi)) ;

        if(!CreateProcessA(
            NULL, const_cast<LPSTR>(path.c_str()), NULL, NULL, FALSE,
            CREATE_NEW_CONSOLE, NULL, Path::HOME_PATH().c_str(), &si, &pi)) {
            throw RUNTIME_EXCEPT("cannot call \"" + path + "\"") ;
        }
    }
}

using namespace ExAppUtility ;


//StartShell
const string StartShell::sname() noexcept
{
    return "start_shell" ;
}

void StartShell::sprocess(const bool first_call, const unsigned int UNUSED(repeat_num), const KeyLogger* const parent_logger)
{
    if(!first_call) return ;

    _create_process(_get_protected_path("shell")) ;

    //wait until select window by OS.
    Sleep(100) ;
    Jump2ActiveWindow::sprocess(true, 1, parent_logger) ;
}


//StartAnyApp
const string StartAnyApp::sname() noexcept
{
    return "start_any_app" ;
}

void StartAnyApp::sprocess(const bool first_call, const unsigned int UNUSED(repeat_num), const KeyLogger* const parent_logger)
{
    if(!first_call) return ;

    if(!parent_logger) {
        throw LOGIC_EXCEPT("The passed parent logger is null") ;
    }
    auto cmd = parent_logger->get_as_str() ;

    _create_process(_get_protected_path(cmd.substr(1))) ;

    //wait until select window by OS.
    Sleep(100) ;

    Jump2ActiveWindow::sprocess(true, 1, parent_logger) ;
}
