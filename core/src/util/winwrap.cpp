#include "util/winwrap.hpp"

#include "err_logger.hpp"
#include "util/def.hpp"
#include "util/point_2d.hpp"

#include <sstream>
#include <unordered_set>

#include <psapi.h>

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

            DWORD flags = CREATE_DEFAULT_ERROR_MODE ;
            if(show_console_window) {
                flags |= CREATE_NEW_CONSOLE ;
            }
            else {
                static const std::unordered_set<std::string> hardcoded_undetachable {
                    "powershell"
                } ;

                if(hardcoded_undetachable.find(util::A2a(cmd)) != hardcoded_undetachable.end()) {
                    // Currently, we can't figure out how to hide PowerShell window completely
                    // when executes other command with -c option.
                    // If set CREATE_NEW_CONSOLE, PowerShell shows for a moment,
                    // but we can execute -c option.
                    flags |= CREATE_NEW_CONSOLE ;
                }
                else {
                    flags |= DETACHED_PROCESS ;
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

            if(!CreateProcessW(
                NULL, const_cast<LPWSTR>(s_to_ws(cmd).c_str()),
                NULL, NULL, FALSE,
                flags, NULL,
                current_dir.empty() ? NULL : s_to_ws(current_dir).c_str(),
                &si, &pi)) {

                throw RUNTIME_EXCEPT("Cannot start \"" + cmd  + "\"") ;
            }

            CloseHandle(pi.hThread) ;
            CloseHandle(pi.hProcess) ;
        }

        int shell_execute(const std::string& url) {
            return static_cast<int>(reinterpret_cast<std::size_t>(
                        ShellExecuteW(
                            NULL, NULL,
                            util::s_to_ws(url).c_str(),
                            NULL, NULL, SW_SHOWNORMAL))) ;
        }

        std::string get_module_filename(HWND hwnd) {
            DWORD procid ;
            GetWindowThreadProcessId(hwnd, &procid) ;

            auto handle = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, procid) ;

            WCHAR fullpath[MAX_PATH] ;
            if(!GetModuleFileNameExW(handle, NULL, fullpath, MAX_PATH)) {
                CloseHandle(handle) ;
                throw RUNTIME_EXCEPT("Could not get module filename.") ;
            }
            CloseHandle(handle) ;
            return util::ws_to_s(fullpath) ;
        }

        bool is_failed(HRESULT result) noexcept {
            if(SUCCEEDED(result)) {
                return false ;
            }

            auto bool_result = FAILED(result) ;
            if(result == E_ABORT) {
                PRINT_ERROR("The operation was aborted because of an unspecified error.") ;
            }
            else if(result == E_ACCESSDENIED) {
                PRINT_ERROR("A general access-denied error.") ;
            }
            else if(result == E_FAIL) {
                PRINT_ERROR("An unspecified failure has occurred.") ;
            }
            else if(result == E_HANDLE) {
                PRINT_ERROR("An invalid handle was used.") ;
            }
            else if(result == E_INVALIDARG) {
                PRINT_ERROR("One or more arguments are invalid.") ;
            }
            else if(result == E_NOINTERFACE) {
                PRINT_ERROR("The QueryInterface method did not recognize the requested interface. The interface is not supported.") ;
            }
            else if(result == E_NOTIMPL) {
                PRINT_ERROR("The method is not implemented.") ;
            }
            else if(result == E_OUTOFMEMORY) {
                PRINT_ERROR("The method failed to allocate necessary memory.") ;
            }
            else if(result == E_PENDING) {
                PRINT_ERROR("The data necessary to complete the operation is not yet available.") ;
            }
            else if(result == E_POINTER) {
                PRINT_ERROR("An invalid pointer was used.") ;
            }
            else if(result == E_UNEXPECTED) {
                PRINT_ERROR("A catastrophic failure has occurred") ;
            }
            else {
                PRINT_ERROR("Unknown error was occurred") ;
            }
            return bool_result ;
        }

        HDCUPtr create_display_dc() {
            auto raw_hdc = CreateDCA("DISPLAY", NULL, NULL, NULL) ;
            if(!raw_hdc) {
                throw RUNTIME_EXCEPT("Could not create HDC of DISPLAY.") ;
            }
            return HDCUPtr(raw_hdc, delete_hdc) ;
        }

        void set_dc_text_color(
                std::shared_ptr<HDC__>& hdc,
                const COLORREF& color) {
            if(SetTextColor(hdc.get(), color) == CLR_INVALID) {
                throw RUNTIME_EXCEPT("Could not set a text color.") ;
            }
        }

        void set_dc_back_color(
                std::shared_ptr<HDC__>& hdc,
                const COLORREF& color) {
            if(SetBkColor(hdc.get(), color) == CLR_INVALID) {
                throw RUNTIME_EXCEPT("Could not set a background color.") ;
            }
        }

        HFontUPtr create_font(const LOGFONTA& logfont) {
            auto raw_font = CreateFontIndirectA(&logfont) ;
            if(!raw_font) {
                throw RUNTIME_EXCEPT("Could not create a font.") ;
            }
            return HFontUPtr(raw_font, delete_obj) ;
        }

        void attach_thread_input(HWND hwnd) {
            auto self_thid = GetCurrentThreadId() ;
            auto target_thid = GetWindowThreadProcessId(hwnd, NULL) ;
            if(!AttachThreadInput(self_thid, target_thid, TRUE)) {
                std::stringstream ss ;
                ss << "Could not attach input states with the another window (" << hwnd << ")" ;
                throw RUNTIME_EXCEPT(ss.str()) ;
            }
        }

        void detach_thread_input(HWND hwnd) {
            auto self_thid = GetCurrentThreadId() ;
            auto target_thid = GetWindowThreadProcessId(hwnd, NULL) ;
            if(!AttachThreadInput(self_thid, target_thid, FALSE)) {
                std::stringstream ss ;
                ss << "Could not detach input states with the another window (" << hwnd << ")" ;
                throw RUNTIME_EXCEPT(ss.str()) ;
            }
        }

        Point2D get_caret_pos(HWND hwnd) {
            attach_thread_input(hwnd) ;

            Point2D pos ;
            if(!GetCaretPos(&pos.data())) {
                throw RUNTIME_EXCEPT("Could not get the caret position.") ;
            }

            hwnd = GetFocus() ;

            detach_thread_input(hwnd) ;

            if(!ClientToScreen(hwnd, &pos.data())) {
                throw RUNTIME_EXCEPT("Could not convert a client position to screen one.") ;
            }

            return pos ;
        }
    }
}
