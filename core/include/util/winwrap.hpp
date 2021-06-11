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

        template <typename ...Args>
        std::string concat_args(Args&&... args) {
            std::initializer_list<std::string> arglist = {
                std::forward<Args>(args)...} ;
            std::string out ;
            for(auto& arg : arglist) {
                out += " " + arg ;
            }
            return out ;
        }

        void create_process(
                const std::string& current_dir,
                std::string cmd,
                const std::string& args="",
                bool show_console_window=true) ;

        void shell_execute_open(const std::string& url) ;
    }
}

#endif
