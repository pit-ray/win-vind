#include "startup.hpp"

#include "path.hpp"
#include "util/def.hpp"
#include "util/string.hpp"

#include <objidl.h>
#include <shlobj.h>
#include <windows.h>

#include <cstring>
#include <memory>

#ifdef DEBUG
#include <iostream>
#endif

#include "err_logger.hpp"
#include <sstream>


namespace
{
    const std::wstring& get_startup_dir() {
        static std::wstring path{} ;
        if(!path.empty()) {
            return path ;
        }

        WCHAR* ptr = NULL ;

        auto sh_get_res = SHGetKnownFolderPath(FOLDERID_Startup, 0, NULL, &ptr) ;
        PRINT_ERROR("Report: SHGetKnownFolderPath returned " + std::to_string(static_cast<int>(sh_get_res))) ;

        if(S_OK != sh_get_res) {
            PRINT_ERROR("Report: SHGetKnownFolderPath is failed") ;
            throw RUNTIME_EXCEPT("Could not get startup directory.") ;
        }
        PRINT_ERROR("Report: SHGetKnownFolderPath Done") ;

        auto len = std::wcslen(ptr) ;
        PRINT_ERROR("Report: Path length is " + std::to_string(static_cast<int>(len))) ;
        path.resize(len) ;
        PRINT_ERROR("Report: std::wstring resized") ;

        PRINT_ERROR("Report: string length is " + std::to_string(static_cast<int>(path.length()))) ;
        std::memcpy(path.data(), ptr, path.length() * sizeof(WCHAR)) ;
        PRINT_ERROR("Report: std::memcpy done") ;

        CoTaskMemFree(ptr) ;
        PRINT_ERROR("Report: CoTaskMemFree Done") ;

        PRINT_ERROR("Report: startup dir is " + vind::util::ws_to_s(path)) ;
        return path ;
    }

    inline const std::wstring& get_shell_link_path() {
        static auto p = get_startup_dir() + L"\\win-vind.lnk" ;
        return p ;
    }

    auto shell_releaser = [] (IShellLinkW* ptr) {
        ptr->Release() ;
        PRINT_ERROR("Report: Released IShellLinkW") ;
    } ;
    using ShellLinkInterface = std::shared_ptr<IShellLinkW> ;

    ShellLinkInterface create_shell_link() {
        IShellLinkW* psl_raw ;
        auto res = CoCreateInstance(
                        CLSID_ShellLink, NULL,
                        CLSCTX_INPROC_SERVER, IID_IShellLink, reinterpret_cast<LPVOID*>(&psl_raw)) ;
        PRINT_ERROR("Report: CoCreateInstance returned " + std::to_string(static_cast<int>(res))) ;
        if(FAILED(res)) {
            PRINT_ERROR("Report: Failed CoCreateInstance") ;
            throw RUNTIME_EXCEPT("Could not create interface of ShellLink.") ;
        }
        PRINT_ERROR("Report: Finished") ;
        return ShellLinkInterface(psl_raw, shell_releaser) ;
    }

    auto pfile_releaser = [] (IPersistFile* ptr) {
        ptr->Release() ;
        PRINT_ERROR("Report: Released IPersistFile") ;
    } ;
    using PersistFileInterface = std::shared_ptr<IPersistFile> ;
    PersistFileInterface create_persist_file(IShellLink* psl) {
        IPersistFile* ppf_raw ;
        auto res = psl->QueryInterface(IID_IPersistFile, reinterpret_cast<LPVOID*>(&ppf_raw)) ;
        PRINT_ERROR("Report: QueryInterface returned " + std::to_string(static_cast<int>(res))) ;

        if(FAILED(res)) {
            PRINT_ERROR("Report: Failed QueryInterface") ;
            throw RUNTIME_EXCEPT("Could not create interface of PersistFile.") ;
        }

        PRINT_ERROR("Report: Finised") ;
        return PersistFileInterface(ppf_raw, pfile_releaser) ;
    }


    template <typename EachProc>
    void enumerate_startup_files(EachProc&& proc) {
        PRINT_ERROR("Report: MAX_PATH = " + std::to_string(MAX_PATH)) ;
        PRINT_ERROR("Report: Called enumerate_startup_files") ;
        using namespace vind ;

        auto psl = create_shell_link() ;
        PRINT_ERROR("Report: Created ShellLink instance") ;
        auto ppf = create_persist_file(psl.get()) ;
        PRINT_ERROR("Report: Created PersistFile instance") ;

        PRINT_ERROR("Report: Detected directory - " + util::ws_to_s(get_startup_dir() + L"\\*.lnk")) ;
        WIN32_FIND_DATAW wfd = {} ;
        auto handle = FindFirstFileW(
                (get_startup_dir() + L"\\*.lnk").c_str(),
                &wfd) ;
        if(handle == INVALID_HANDLE_VALUE) {
            PRINT_ERROR("Report: FindFirstFileW INVALID HANDLE") ;
            return ;
        }
        if(GetLastError() == ERROR_FILE_NOT_FOUND) {
            PRINT_ERROR("Report: FindFirstFileW - ERROR_FILE_NOT_FOUND (New Appended Code)") ;
        }
        PRINT_ERROR("Report: FindFirstFilew done") ;

        do {
            auto path = get_startup_dir() + L"\\" + wfd.cFileName ;
            PRINT_ERROR("Report: Detected file - " + util::ws_to_s(path)) ;

            auto res = ppf->Load(path.c_str(), STGM_READ) ;
            PRINT_ERROR("Report: ppf->Load returned " + std::to_string(static_cast<int>(res))) ;
            if(FAILED(res)) {
                PRINT_ERROR("Report: Failed ppf->Load") ;
                continue ;
            }
            PRINT_ERROR("Report: ppf->Load Done") ;

            WIN32_FIND_DATAW target_wfd = {} ;
            std::wstring target_path ;
            target_path.resize(MAX_PATH) ;
            PRINT_ERROR("Report: Created std::wstring instance") ;

            auto gp_res = psl->GetPath(target_path.data(), MAX_PATH, &target_wfd, SLGP_SHORTPATH) ;
            PRINT_ERROR("Report: psl->GetPath returned " + std::to_string(static_cast<int>(gp_res))) ;
            if(FAILED(gp_res)) {
                PRINT_ERROR("Report: Failed psl->GetPath") ;
                continue ;
            }
            PRINT_ERROR("Report: psl->GetPath Done") ;

            PRINT_ERROR("Report: Call proc arg[0]: " + util::ws_to_s(path) + ", arg[1] " + util::ws_to_s(target_path)) ;
            if(!proc(path, target_path)) {
                PRINT_ERROR("Report: Loop is breaked") ;
                break ;
            }
            PRINT_ERROR("Report: --- Finished Loop ---") ;
        } while(FindNextFileW(handle, &wfd)) ;

        PRINT_ERROR("Report: Finided Scanning") ;
        FindClose(handle) ;
        PRINT_ERROR("Report: Closed Find Handle") ;
    }
}


namespace vindgui
{
    void register_to_startup() {
        auto psl = create_shell_link() ;

        using namespace vind ;
        static auto module_path = util::s_to_ws(path::MODULE_PATH()) ;
        psl->SetPath(module_path.c_str()) ;

        auto ppf = create_persist_file(psl.get()) ;

        if(FAILED(ppf->Save(get_shell_link_path().c_str(), TRUE))) {
            throw RUNTIME_EXCEPT("Could not create startup link.") ;
        }
    }

    void remove_from_startup() {
        enumerate_startup_files([] (auto&& link, auto&& target) {
            using namespace vind ;
            auto lower = util::A2a(util::ws_to_s(target)) ;
            if(lower.find("win-vind") != std::string::npos) {
                DeleteFileW(link.c_str()) ;
            }
            return true ;
        }) ;
    }

    bool check_if_registered() {
        PRINT_ERROR("Report: Called check_if_registered()") ;
        bool exist_shortcut = false ;
        enumerate_startup_files([&exist_shortcut] (auto&& UNUSED(link), auto&& target) {
            using namespace vind ;
            auto lower = util::A2a(util::ws_to_s(target)) ;
            PRINT_ERROR("Report: tartget is " + lower) ;
            if(lower.find("win-vind") != std::string::npos) {
                PRINT_ERROR("Report: Found win-vind string") ;
                exist_shortcut = true ;
                return false ;
            }
            PRINT_ERROR("Report: Finished") ;
            return true ;
        }) ;

        std::stringstream ss ;
        ss << "Report: Return check_if_registered -> " ;
        ss << (exist_shortcut ? "true" : "false") ;
        PRINT_ERROR(ss.str()) ;
        return exist_shortcut ;
    }
}
