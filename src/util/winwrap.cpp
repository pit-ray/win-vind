#include "winwrap.hpp"

#include "core/errlogger.hpp"

#include "debug.hpp"
#include "def.hpp"
#include "point2d.hpp"

#include <sstream>
#include <unordered_set>

#include "disable_compiler_warning.hpp"
#include <ntstatus.h>
#include "enable_compiler_warning.hpp"

#include <psapi.h>


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

        HWND get_foreground_window() {
            auto hwnd = GetForegroundWindow() ;
            /*
            if(!hwnd) {
                throw std::runtime_error("There is no foreground window.") ;
            }
            */
            return hwnd ;
        }

        void set_foreground_window(HWND hwnd) {
            if(!SetForegroundWindow(hwnd)) {
                throw std::runtime_error("Could not set a window as foreground.") ;
            }
        }

        Box2D get_window_rect(HWND hwnd) {
            util::Box2D rect ;
            if(!GetWindowRect(hwnd, &(rect.data()))) {
                throw std::runtime_error("Could not get a rectangle of the window.") ;
            }
            return rect ;
        }

        Box2D get_client_rect(HWND hwnd) {
            util::Box2D rect ;
            if(!GetClientRect(hwnd, &(rect.data()))) {
                throw std::runtime_error("Could not get a rectangle of the clind area in the window.") ;
            }
            return rect ;
        }

        void move_window(HWND hwnd, long left, long top, long width, long height) {
            if(!MoveWindow(hwnd, left, top, width, height, TRUE)) {
                throw RUNTIME_EXCEPT("Could not move the foreground window.") ;
            }
        }

        template <typename StdString, typename Path>
        inline void create_process_core(
                StdString&& cmd,
                Path&& current_dir,
                DWORD flags,
                bool wait_until_finish=false) {
            STARTUPINFOW si = {} ;
            si.cb = sizeof(si) ;
            si.dwFlags = STARTF_USEPOSITION | STARTF_USESHOWWINDOW ;
            si.dwX = 0 ;
            si.dwY = 0 ;
            si.wShowWindow = SW_RESTORE ;

            PROCESS_INFORMATION pi = {} ;

            if(!CreateProcessW(
                NULL, const_cast<LPWSTR>(s_to_ws(cmd).c_str()),
                NULL, NULL, FALSE,
                flags, NULL,
                current_dir.empty() ? NULL : current_dir.wstring().c_str(),
                &si, &pi)) {

                throw RUNTIME_EXCEPT(
                        "Cannot start \"" + cmd + "\" " \
                        + "in the current directory \"" + current_dir.u8string() + "\".") ;
            }

            auto close_handle = [](HANDLE handle) {CloseHandle(handle) ;} ;
            std::unique_ptr<void, decltype(close_handle)> h_thread(pi.hThread, close_handle) ;
            std::unique_ptr<void, decltype(close_handle)> h_process(pi.hProcess, close_handle) ;

            if(wait_until_finish) {
                if(WaitForSingleObject(h_process.get(), INFINITE) == WAIT_FAILED) {
                    PRINT_ERROR("Failed to wait until process end.") ;
                }
            }
        }

        void create_process(
                const std::filesystem::path& current_dir,
                std::string cmd,
                const std::string& args,
                bool show_console_window,
                bool wait_until_finish) {

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

            create_process_core(cmd, current_dir, flags, wait_until_finish) ;
        }

        int shell_execute(const std::filesystem::path& url) {
            return static_cast<int>(reinterpret_cast<std::size_t>(
                        ShellExecuteW(
                            NULL, NULL,
                            url.wstring().c_str(),
                            NULL, NULL, SW_SHOWNORMAL))) ;
        }

        std::filesystem::path _get_module_path(DWORD procid) {
            auto handle_raw = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, procid) ;
            if(handle_raw == NULL) {
                throw RUNTIME_EXCEPT("Could not open process.") ;
            }
            auto close_handle = [](HANDLE handle) {CloseHandle(handle) ;} ;
            std::unique_ptr<void, decltype(close_handle)> handle(handle_raw, close_handle) ;

            WCHAR fullpath[MAX_PATH] ;
            if(!GetModuleFileNameExW(handle.get(), NULL, fullpath, MAX_PATH)) {
                throw RUNTIME_EXCEPT("Could not get module filename.") ;
            }
            return std::filesystem::path(fullpath) ;
        }

        std::filesystem::path _get_module_path(HWND hwnd) {
            DWORD procid ;
            GetWindowThreadProcessId(hwnd, &procid) ;
            return _get_module_path(procid) ;
        }

        std::string get_module_filename(HWND hwnd) {
            return _get_module_path(hwnd).filename().u8string() ;
        }

        std::string get_module_filename(DWORD procid) {
            return _get_module_path(procid).filename().u8string() ;
        }

        std::filesystem::path get_module_path(HWND hwnd) {
            return _get_module_path(hwnd) ;
        }

        std::filesystem::path get_module_path(DWORD procid) {
            return _get_module_path(procid) ;
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
            auto raw_hdc = CreateDCW(L"DISPLAY", NULL, NULL, NULL) ;
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

        HFontUPtr create_font(const LOGFONTW& logfont) {
            auto raw_font = CreateFontIndirectW(&logfont) ;
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

        Point2D get_cursor_pos() {
            Point2D pos ;
            if(!GetCursorPos(&(pos.data()))) {
                throw RUNTIME_EXCEPT("Could not get the cursor position.") ;
            }
            return pos ;
        }

        void set_cursor_pos(int x, int y) {
            if(!SetCursorPos(x, y)) {
                throw RUNTIME_EXCEPT("Could not move the cursor.") ;
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


        std::string get_Windows_edition(
                DWORD major_version,
                DWORD minor_version) {
            DWORD product_type ;
            if(!GetProductInfo(
                        major_version, minor_version,
                        0, 0, &product_type)) {
                throw RUNTIME_EXCEPT("Could not get the product infomation of Windows.") ;
            }

            switch(product_type) {
                case PRODUCT_BUSINESS:
                    return "Business" ;

                case PRODUCT_BUSINESS_N:
                    return "Business N" ;

                case PRODUCT_CLUSTER_SERVER:
                    return "HPC Edition" ;

                case PRODUCT_CLUSTER_SERVER_V:
                    return "Server Hyper Core V" ;

                case PRODUCT_CORE:
                    return "Windows 10 Home" ;

                case PRODUCT_CORE_COUNTRYSPECIFIC:
                    return "Windows 10 Home China" ;

                case PRODUCT_CORE_N:
                    return "Windows 10 Home N" ;

                case PRODUCT_CORE_SINGLELANGUAGE:
                    return "Windows 10 Home Single Language" ;

                case PRODUCT_DATACENTER_EVALUATION_SERVER:
                    return "Server Datacenter (evaluation installation)" ;

                case PRODUCT_DATACENTER_SERVER:
                    return "Server Datacenter (full installation)" ;

                case PRODUCT_DATACENTER_SERVER_CORE:
                    return "Server Datacenter (core installation)" ;

                case PRODUCT_DATACENTER_SERVER_CORE_V:
                    return "Server Datacenter without Hyper-V (core installation)" ;

                case PRODUCT_DATACENTER_SERVER_V:
                    return "Server Datacenter without Hyper-V (full installation)" ;

                case PRODUCT_EDUCATION:
                    return "Windows 10 Education" ;

                case PRODUCT_EDUCATION_N:
                    return "Windows 10 Education N" ;

                case PRODUCT_ENTERPRISE:
                    return "Windows 10 Enterprise" ;

                case PRODUCT_ENTERPRISE_E:
                    return "Windows 10 Enterprise E" ;

                case PRODUCT_ENTERPRISE_EVALUATION:
                    return "Windows 10 Enterprise Evaluation" ;

                case PRODUCT_ENTERPRISE_N:
                    return "Windows 10 Enterprise N" ;

                case PRODUCT_ENTERPRISE_N_EVALUATION:
                    return "Windows 10 Enterprise N Evaluation" ;

                case PRODUCT_ENTERPRISE_S:
                    return "Windows 10 Enterprise 2015 LTSB" ;

                case PRODUCT_ENTERPRISE_S_EVALUATION:
                    return "Windows 10 Enterprise 2015 LTSB Evaluation" ;

                case PRODUCT_ENTERPRISE_S_N:
                    return "Windows 10 Enterprise 2015 LTSB N" ;

                case PRODUCT_ENTERPRISE_S_N_EVALUATION:
                    return "Windows 10 Enterprise 2015 LTSB N Evaluation" ;

                case PRODUCT_ENTERPRISE_SERVER:
                    return "Server Enterprise (full installation)" ;

                case PRODUCT_ENTERPRISE_SERVER_CORE:
                    return "Server Enterprise (core installation)" ;

                case PRODUCT_ENTERPRISE_SERVER_CORE_V:
                    return "Server Enterprise without Hyper-V (core installation)" ;

                case PRODUCT_ENTERPRISE_SERVER_IA64:
                    return "Server Enterprise for Itanium-based Systems" ;

                case PRODUCT_ENTERPRISE_SERVER_V:
                    return "Server Enterprise without Hyper-V (full installation)" ;

                case PRODUCT_ESSENTIALBUSINESS_SERVER_ADDL:
                    return "Windows Essential Server Solution Additional" ;

                case PRODUCT_ESSENTIALBUSINESS_SERVER_ADDLSVC:
                    return "Windows Essential Server Solution Additional SVC" ;

                case PRODUCT_ESSENTIALBUSINESS_SERVER_MGMT:
                    return "Windows Essential Server Solution Management" ;

                case PRODUCT_ESSENTIALBUSINESS_SERVER_MGMTSVC:
                    return "Windows Essential Server Solution Management SVC" ;

                case PRODUCT_HOME_BASIC:
                    return "Home Basic" ;

                case PRODUCT_HOME_BASIC_N:
                    return "Home Basic N" ;

                case PRODUCT_HOME_PREMIUM:
                    return "Home Premium" ;

                case PRODUCT_HOME_PREMIUM_N:
                    return "Home Premium N" ;

                case PRODUCT_HOME_PREMIUM_SERVER:
                    return "Windows Home Server 2011" ;

                case PRODUCT_HOME_SERVER:
                    return "Windows Storage Server 2008 R2 Essentials" ;

                case PRODUCT_HYPERV:
                    return "Microsoft Hyper-V Server" ;

                case PRODUCT_IOTUAP:
                    return "Windows 10 IoT Core" ;

                case PRODUCT_MEDIUMBUSINESS_SERVER_MANAGEMENT:
                    return "Windows Essential Business Server Management Server" ;

                case PRODUCT_MEDIUMBUSINESS_SERVER_MESSAGING:
                    return "Windows Essential Business Server Messaging Server" ;

                case PRODUCT_MEDIUMBUSINESS_SERVER_SECURITY:
                    return "Windows Essential Business Server Security Server" ;

#if defined(PRODUCT_MOBILE_CORE)
                case PRODUCT_MOBILE_CORE:
                    return "Windows 10 Mobile" ;
#endif

#if defined(PRODUCT_MOBILE_ENTERPRISE)
                case PRODUCT_MOBILE_ENTERPRISE:
                    return "Windows 10 Mobile Enterprise" ;
#endif

                case PRODUCT_MULTIPOINT_PREMIUM_SERVER:
                    return "Windows MultiPoint Server Premium (full installation)" ;

                case PRODUCT_MULTIPOINT_STANDARD_SERVER:
                    return "Windows MultiPoint Server Standard (full installation)" ;

                case PRODUCT_PROFESSIONAL:
                    return "Windows 10 Pro" ;

                case PRODUCT_PROFESSIONAL_N:
                    return "Windows 10 Pro N" ;

                case PRODUCT_PROFESSIONAL_WMC:
                    return "Professional with Media Center" ;

                case PRODUCT_SB_SOLUTION_SERVER:
                    return "Windows Small Business Server 2011 Essentials" ;

                case PRODUCT_SB_SOLUTION_SERVER_EM:
                    return "Server For SB Solutions EM" ;

                case PRODUCT_SERVER_FOR_SB_SOLUTIONS:
                    return "Server For SB Solutions" ;

                case PRODUCT_SERVER_FOR_SB_SOLUTIONS_EM:
                    return "Server For SB Solutions EM" ;

                case PRODUCT_SERVER_FOR_SMALLBUSINESS:
                    return "Windows Server 2008 for Windows Essential Server Solutions" ;

                case PRODUCT_SERVER_FOR_SMALLBUSINESS_V:
                    return "Windows Server 2008 without Hyper-V for Windows Essential Server Solutions" ;

                case PRODUCT_SERVER_FOUNDATION:
                    return "Server Foundation" ;

                case PRODUCT_SMALLBUSINESS_SERVER:
                    return "Windows Small Business Server" ;

                case PRODUCT_SMALLBUSINESS_SERVER_PREMIUM:
                    return "Small Business Server Premium" ;

                case PRODUCT_SMALLBUSINESS_SERVER_PREMIUM_CORE:
                    return "Small Business Server Premium (core installation)" ;

                case PRODUCT_SOLUTION_EMBEDDEDSERVER:
                    return "Windows MultiPoint Server" ;

                case PRODUCT_STANDARD_EVALUATION_SERVER:
                    return "Server Standard (evaluation installation)" ;

                case PRODUCT_STANDARD_SERVER:
                    return "Server Standard (full installation)" ;

                case PRODUCT_STANDARD_SERVER_CORE:
                    return "Server Standard (core installation)" ;

                case PRODUCT_STANDARD_SERVER_CORE_V:
                    return "Server Standard without Hyper-V (core installation)" ;

                case PRODUCT_STANDARD_SERVER_V:
                    return "Server Standard without Hyper-V" ;

                case PRODUCT_STANDARD_SERVER_SOLUTIONS:
                    return "Server Solutions Premium" ;

                case PRODUCT_STANDARD_SERVER_SOLUTIONS_CORE:
                    return "Server Solutions Premium (core installation)" ;

                case PRODUCT_STARTER:
                    return "Starter" ;

                case PRODUCT_STARTER_N:
                    return "Starter N" ;

                case PRODUCT_STORAGE_ENTERPRISE_SERVER:
                    return "Storage Server Enterprise" ;

                case PRODUCT_STORAGE_ENTERPRISE_SERVER_CORE:
                    return "Storage Server Enterprise (core installation)" ;

                case PRODUCT_STORAGE_EXPRESS_SERVER:
                    return "Storage Server Express" ;

                case PRODUCT_STORAGE_EXPRESS_SERVER_CORE:
                    return "Storage Server Express (core installation)" ;

                case PRODUCT_STORAGE_STANDARD_EVALUATION_SERVER:
                    return "Storage Server Standard (evaluation installation)" ;

                case PRODUCT_STORAGE_STANDARD_SERVER:
                    return "Storage Server Standard" ;

                case PRODUCT_STORAGE_STANDARD_SERVER_CORE:
                    return "Storage Server Standard (core installation)" ;

                case PRODUCT_STORAGE_WORKGROUP_EVALUATION_SERVER:
                    return "Storage Server Workgroup (evaluation installation)" ;

                case PRODUCT_STORAGE_WORKGROUP_SERVER:
                    return "Storage Server Workgroup" ;

                case PRODUCT_STORAGE_WORKGROUP_SERVER_CORE:
                    return "Storage Server Workgroup (core installation)" ;

                case PRODUCT_ULTIMATE:
                    return "Ultimate" ;

                case PRODUCT_ULTIMATE_N:
                    return "Ultimate N" ;

                case PRODUCT_UNDEFINED:

                case PRODUCT_WEB_SERVER:
                    return "Web Server (full installation)" ;

                case PRODUCT_WEB_SERVER_CORE:
                    return "Web Server (core installation)" ;
            }

            return "Unknown" ;
        }


        std::string get_Windows_architecture() noexcept {
            SYSTEM_INFO sysinfo ;
            GetNativeSystemInfo(&sysinfo) ;

            switch(sysinfo.wProcessorArchitecture) {
                case PROCESSOR_ARCHITECTURE_AMD64:
                    return "x64" ;

#if defined(PROCESSOR_ARCHITECTURE_ARM)
                case PROCESSOR_ARCHITECTURE_ARM:
                    return "ARM" ;
#endif


#if defined(PROCESSOR_ARCHITECTURE_ARM64)
                case PROCESSOR_ARCHITECTURE_ARM64:
                    return "ARM64" ;
#endif


#if defined(PROCESSOR_ARCHITECTURE_IA64)
                case PROCESSOR_ARCHITECTURE_IA64:
                    return "Intel Itanium-based" ;
#endif

                case PROCESSOR_ARCHITECTURE_INTEL:
                    return "x86" ;
            }
            return "Unknown" ;
        }

        using RtlGetVersionType = NTSTATUS (WINAPI*)(PRTL_OSVERSIONINFOW) ;
        std::tuple<DWORD, DWORD, DWORD> get_Windows_versions() {
            const auto hmodule = LoadLibraryW(L"ntdll.dll") ;
            if(!hmodule) {
                throw RUNTIME_EXCEPT("Could not load ntdll.dll to get versions of Windows.") ;
            }

            // cast to void* once to avoid warnings about type conversion.
            const auto RtlGetVersion = reinterpret_cast<RtlGetVersionType>(
                    reinterpret_cast<void*>(GetProcAddress(hmodule, "RtlGetVersion"))) ;
            if(!RtlGetVersion) {
                FreeLibrary(hmodule) ;
                throw RUNTIME_EXCEPT("Could not get GetProcAddress function from ntdll.dll.") ;
            }

            OSVERSIONINFOW vinfo ;
            vinfo.dwOSVersionInfoSize = sizeof(decltype(vinfo)) ;
            auto result = RtlGetVersion(&vinfo) ;
            FreeLibrary(hmodule) ;

            if(result != STATUS_SUCCESS) {
                throw RUNTIME_EXCEPT("RtlGetVersion is failed, so could not get the version of Windows.") ;
            }

            return {
                vinfo.dwMajorVersion,
                vinfo.dwMinorVersion,
                vinfo.dwBuildNumber
            } ;
        }

        std::string get_Windows_display_version() {
            constexpr auto subkey = L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion" ;

            const std::wstring version_keys[] = {
                L"DisplayVersion",
                L"ReleaseId"
            } ;
            for (const auto& version_key : version_keys) {
                DWORD size ;
                if(ERROR_SUCCESS != RegGetValueW(
                        HKEY_LOCAL_MACHINE, subkey, version_key.c_str(),
                        RRF_RT_REG_SZ, NULL, NULL, &size) ) {
                    continue ;
                }

                std::wstring val ;
                val.resize(size) ; // Consider null-terminator

                if(ERROR_SUCCESS != RegGetValueW(
                        HKEY_LOCAL_MACHINE, subkey, version_key.c_str(),
                        RRF_RT_REG_SZ, NULL, &val[0], &size)) {
                    continue ;
                }

                // Resize down the string to avoid multiple null terminated string.
                val.resize(size - 1) ;
                return util::ws_to_s(val) ;
            }

            throw std::runtime_error(
                    "Could not read the registory value at "
                    "\"HKEY_LOCAL_MACHINE" + util::ws_to_s(subkey) + "\".") ;
        }
    }
}
