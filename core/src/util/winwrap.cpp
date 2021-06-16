#include "util/def.hpp"
#include "util/winwrap.hpp"

#if defined(DEBUG)
#include <iostream>
#endif

namespace vind
{
    namespace util {
        const MSG& get_win_message() noexcept {
            static MSG msg ;
            if(PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE)) {
                TranslateMessage(&msg) ;
                DispatchMessageW(&msg) ;
            }
            return msg ;
        }

        void refresh_display(HWND UNUSED(hwnd)) {
            //Some applications flicker by passing a NULL HWND as the first argument.  
            //This nedes improvement.
            if(!InvalidateRect(NULL, NULL, TRUE)) {
                throw RUNTIME_EXCEPT(" failed refresh display") ;
            }
        }

        bool is_existed_dir(const std::string& path)
        {
          auto flag = GetFileAttributesW(s_to_ws(path).c_str());
          return (flag != INVALID_FILE_ATTRIBUTES && (flag & FILE_ATTRIBUTE_DIRECTORY));
        }

        void create_directory(const std::string& path) {
            if(!CreateDirectoryW(s_to_ws(path).c_str(), NULL)) {
                throw RUNTIME_EXCEPT("Cannot create a directory " + path + ".") ;
            }
        }
        void copy_file(const std::string& src, const std::string& dst, bool allow_overwrite) {
            if(!CopyFileW(
                        s_to_ws(src).c_str(),
                        s_to_ws(dst).c_str(),
                        !b_to_B(allow_overwrite))) {

                throw RUNTIME_EXCEPT("Could not copy a file from " + src + " to " + dst + ".") ;
            }
        }

        void create_process(
                const std::string& current_dir,
                std::string cmd,
                const std::string& args,
                bool show_console_window) {

            //protect path with quotation marks for security.
            if(cmd.find(" ") != std::string::npos) {
                if(cmd.front() != '\"' || cmd.back() != '\"') {
                    cmd = "\"" + cmd + "\"" ;
                }
            }

            if(args.front() != ' ') {
                cmd += " " ;
            }
            cmd += args ;

            STARTUPINFOW si ;
            ZeroMemory(&si, sizeof(si)) ;
            si.cb = sizeof(si) ;

            PROCESS_INFORMATION pi ;
            ZeroMemory(&pi, sizeof(pi)) ;

            DWORD flags = CREATE_DEFAULT_ERROR_MODE ;
            flags |= show_console_window ? CREATE_NEW_CONSOLE : CREATE_NO_WINDOW ;

            if(!CreateProcessW(
                NULL, const_cast<LPWSTR>(s_to_ws(cmd).c_str()),
                NULL, NULL, FALSE,
                flags, NULL,
                current_dir.empty() ? NULL : s_to_ws(current_dir).c_str(),
                &si, &pi)) {

                throw RUNTIME_EXCEPT("Cannot start \"" + cmd  + "\"") ;
            }

            CloseHandle(pi.hProcess) ;
            CloseHandle(pi.hThread) ;
        }

        int shell_execute(const std::string& url) {
            return static_cast<int>(reinterpret_cast<std::size_t>(
                        ShellExecuteW(
                            NULL, NULL,
                            util::s_to_ws(url).c_str(),
                            NULL, NULL, SW_SHOWNORMAL))) ;
        }
    }
}
