#include "external_app.hpp"

#include "disable_gcc_warning.hpp"
#include <boost/property_tree/ini_parser.hpp>
#include "enable_gcc_warning.hpp"

#include "msg_logger.hpp"
#include "path.hpp"
#include "jump_cursor.hpp"

#include <unordered_map>
#include <string>
#include <iostream>
#include <windows.h>

using namespace std ;

namespace ExAppUtility
{
    using mss_t = unordered_map<string, string> ;

    template <typename T>
    inline static void catch_boost_except(T& e) {
        ERROR_STREAM << e.what() << " (ExAppUtility::_load_proc_list_core)\n" ;
    }

    inline static const mss_t _load_proc_list_core(const string& filename) noexcept {
        using namespace boost::property_tree ;

        ptree pt ;
        mss_t map{} ;

        try {
            read_ini(filename, pt) ;

            for(const auto& sect : pt) {
                if(sect.first != "ExAppPath") {
                    continue ;
                }

                for(const auto& key : sect.second) {
                    const auto key_str = static_cast<string>(key.first) ;
                    const auto val_str = key.second.get_value<string>() ;
                    map[key_str] = val_str ;
                }
            }

            MESSAGE_STREAM << "Loaded External Proc List (" << filename << ")\n" ;

            return map ;
        }
        catch(ini_parser_error& e) {
            catch_boost_except(e) ;
            return map ;
        }
        catch(ptree_bad_path& e) {
            catch_boost_except(e) ;
            return map ;
        }
        catch(ptree_bad_data& e) {
            catch_boost_except(e) ;
            return map ;
        }
    }

    static auto proc_list{_load_proc_list_core(Path::CONFIG_EXAPP_INI())} ;

    void load_config() {
        proc_list = _load_proc_list_core(Path::CONFIG_EXAPP_INI()) ;
    }

    inline static const auto get_protected_path(const string name) noexcept {
        try {
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
        catch(const out_of_range& e) {
            ERROR_STREAM << "Not an external-application command : " << name \
            << " (bf_external_app.cpp::ExAppUtility::get_protected_path)\n" ;
            return string{} ;
        }
    }

    inline static bool is_create_process(const string path) noexcept
    {
        STARTUPINFOA si ;
        ZeroMemory(&si, sizeof(si)) ;
        si.cb = sizeof(si) ;

        PROCESS_INFORMATION pi ;
        ZeroMemory(&pi, sizeof(pi)) ;

        if(!CreateProcessA(
            NULL, const_cast<LPSTR>(path.c_str()), NULL, NULL, FALSE,
            CREATE_NEW_CONSOLE, NULL, Path::HOME_PATH().c_str(), &si, &pi)) {
            WIN_ERROR_STREAM << "cannot call \"" << path << "\"" \
            << " (bf_external_app.cpp::ExAppUtility::is_create_process::CreateProcessA)\n" ;
            return false ;
        }
        return true ;
    }
}

using namespace ExAppUtility ;


//StartShell
const string StartShell::sname() noexcept
{
    return "start_shell" ;
}

bool StartShell::sprocess(const string cmd)
{
    if(!is_create_process(get_protected_path("shell"))) {
        return false ;
    }

    //wait until select window by OS.
    Sleep(100) ;

    if(!Jump2ActiveWindow::sprocess(true)) {
        return false ;
    }

    return true ;
}


//StartAnyApp
const string StartAnyApp::sname() noexcept
{
    return "start_any_app" ;
}

bool StartAnyApp::sprocess(const string cmd)
{
    if(!is_create_process(get_protected_path(cmd.substr(1)))) {
        return false ;
    }

    //wait until select window by OS.
    Sleep(100) ;

    if(!Jump2ActiveWindow::sprocess(true)) {
        return false ;
    }

    return true ;
}