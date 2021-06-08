#ifndef _WINWRAP_HPP
#define _WINWRAP_HPP

#include <windows.h>

#include <string>

#include "def.hpp"
#include "string.hpp"

#if defined(DEBUG)
#include <iostream>
#endif


namespace vind
{
    namespace util {
        const MSG& get_win_message() noexcept ;

        void refresh_display(HWND hwnd) ;

        inline BOOL b_to_B(bool b) noexcept {
            return b ? TRUE : FALSE ;
        }

        bool is_existed_dir(const std::string& path) ;

        void create_directory(const std::string& path) ;
        void copy_file(
                const std::string& src,
                const std::string& dst,
                bool allow_overwrite=false) ;

        template <typename... Ts>
        void create_process(
                const std::string& current_dir,
                std::string cmd,
                Ts&&... args) {

            std::initializer_list<std::string> arglist = {
                std::forward<Ts>(args)...} ;

            //protect path with quotation marks for security.
            if(cmd.find(" ") != std::string::npos) {
                if(cmd.front() != '\"' || cmd.back() != '\"') {
                    cmd = "\"" + cmd + "\"" ;
                }
            }

            for(const auto& arg : arglist) {
                cmd += " " + arg ;
            }

            STARTUPINFOW si ;
            ZeroMemory(&si, sizeof(si)) ;
            si.cb = sizeof(si) ;

            PROCESS_INFORMATION pi ;
            ZeroMemory(&pi, sizeof(pi)) ;

            if(!CreateProcessW(
                NULL, const_cast<LPWSTR>(s_to_ws(cmd).c_str()),
                NULL, NULL, FALSE,
                CREATE_NEW_CONSOLE | CREATE_DEFAULT_ERROR_MODE, NULL,
                current_dir.empty() ? NULL : s_to_ws(current_dir).c_str(),
                &si, &pi)) {

                throw RUNTIME_EXCEPT("Cannot start \"" + cmd  + "\"") ;
            }

            CloseHandle(pi.hProcess) ;
            CloseHandle(pi.hThread) ;
        }
    }
}

#endif
