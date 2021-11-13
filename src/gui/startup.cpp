#include "startup.hpp"

#include "core/path.hpp"
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


namespace
{
    const std::wstring& get_startup_dir() {
        static std::wstring path{} ;
        if(!path.empty()) {
            return path ;
        }

        WCHAR* ptr = NULL ;
        if(S_OK != SHGetKnownFolderPath(FOLDERID_Startup, 0, NULL, &ptr)) {
            throw RUNTIME_EXCEPT("Could not get startup directory.") ;
        }

        path.resize(std::wcslen(ptr)) ;

        std::memcpy(path.data(), ptr, path.length() * sizeof(WCHAR)) ;

        CoTaskMemFree(ptr) ;
        return path ;
    }

    inline const std::wstring& get_shell_link_path() {
        static auto p = get_startup_dir() + L"\\win-vind.lnk" ;
        return p ;
    }

    auto shell_releaser = [] (IShellLinkW* ptr) {
        ptr->Release() ;
    } ;
    using ShellLinkInterface = std::shared_ptr<IShellLinkW> ;

    ShellLinkInterface create_shell_link() {
        IShellLinkW* psl_raw ;
        if(FAILED(CoCreateInstance(
                        CLSID_ShellLink, NULL,
                        CLSCTX_INPROC_SERVER, IID_IShellLink, reinterpret_cast<LPVOID*>(&psl_raw)))) {
            throw RUNTIME_EXCEPT("Could not create interface of ShellLink.") ;
        }

        return ShellLinkInterface(psl_raw, shell_releaser) ;
    }

    auto pfile_releaser = [] (IPersistFile* ptr) {
        ptr->Release() ;
    } ;
    using PersistFileInterface = std::shared_ptr<IPersistFile> ;
    PersistFileInterface create_persist_file(IShellLink* psl) {
        IPersistFile* ppf_raw ;
        if(FAILED(psl->QueryInterface(IID_IPersistFile, reinterpret_cast<LPVOID*>(&ppf_raw)))) {
            throw RUNTIME_EXCEPT("Could not create interface of PersistFile.") ;
        }

        return PersistFileInterface(ppf_raw, pfile_releaser) ;
    }


    template <typename EachProc>
    void enumerate_startup_files(EachProc&& proc) {
        using namespace vind ;
        auto psl = create_shell_link() ;
        auto ppf = create_persist_file(psl.get()) ;

        WIN32_FIND_DATAW wfd = {} ;
        auto handle = FindFirstFileW(
                (get_startup_dir() + L"\\*.lnk").c_str(),
                &wfd) ;
        if(handle == INVALID_HANDLE_VALUE) {
            return ;
        }

        do {
            auto path = get_startup_dir() + L"\\" + wfd.cFileName ;
            if(FAILED(ppf->Load(path.c_str(), STGM_READ))) {
                continue ;
            }

            WIN32_FIND_DATAW target_wfd = {} ;
            std::wstring target_path ;
            target_path.resize(MAX_PATH) ;
            if(FAILED(psl->GetPath(target_path.data(), MAX_PATH,
                            &target_wfd, SLGP_SHORTPATH))) {
                continue ;
            }

            if(!proc(path, target_path)) {
                break ;
            }
        } while(FindNextFileW(handle, &wfd)) ;

        FindClose(handle) ;
    }
}


namespace vind
{
    namespace gui
    {
        void register_to_startup() {
            auto psl = create_shell_link() ;

            using namespace vind ;
            psl->SetPath(core::MODULE_PATH().wstring().c_str()) ;

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
            bool exist_shortcut = false ;
            enumerate_startup_files([&exist_shortcut] (auto&& UNUSED(link), auto&& target) {
                using namespace vind ;
                auto lower = util::A2a(util::ws_to_s(target)) ;
                if(lower.find("win-vind") != std::string::npos) {
                    exist_shortcut = true ;
                    return false ;
                }
                return true ;
            }) ;
            return exist_shortcut ;
        }
    }
}
