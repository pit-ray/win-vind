#include "path.hpp"

#include <userenv.h>
#include <windows.h>

#include <fstream>

#include "util/def.hpp"
#include "util/string.hpp"

#ifdef DEBUG
#include <iostream>
#endif


namespace vind
{
    namespace path {

        std::filesystem::path replace_magic(std::filesystem::path path) {
            auto strpath = path.u8string() ;
            auto rel_root = strpath.substr(0, 2) ;
            if(rel_root == "~/" || rel_root == "~\\") {
                path = HOME_PATH() / strpath.substr(2) ;
            }
            path.make_preferred() ;
            return path ;
        }

        std::string replace_magic(std::string path) {
            path = util::replace_all(path, "~", path::HOME_PATH().u8string()) ;
            path = util::replace_all(path, "/", "\\") ;
            return path ;
        }

        InstallType get_install_type() {
            static const auto type = [] {
                std::ifstream ifs{DEFAULT_CONFIG_PATH() / "instype"} ;
                if(!ifs.is_open()) {
                    return InstallType::PORTABLE ;
                }
                std::string str{} ;
                std::getline(ifs, str) ;

                if(str.front() == '1') {
                    return InstallType::INSTALLER ;
                }
                else if(str.front() == '2') {
                    return InstallType::CHOCOLATEY ;
                }

                return InstallType::PORTABLE ;
            }() ;
            return type ;
        }

        bool is_installer_used() {
            return get_install_type() != InstallType::PORTABLE ;
        }

        const std::filesystem::path& HOME_PATH() {
            static const auto obj = [] {
                HANDLE token ;
                if(!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &token)) {
                    throw RUNTIME_EXCEPT("Could not open process token.") ;
                }

                WCHAR path[MAX_PATH] = {0} ;
                DWORD size = MAX_PATH ;
                if(!GetUserProfileDirectoryW(token, path, &size)) {
                    CloseHandle(token) ;
                    throw RUNTIME_EXCEPT("Could not get the home directory.") ;
                }
                CloseHandle(token) ;

                return std::filesystem::path(path) ;
            } () ;

            return obj ;
        }

        const std::filesystem::path& MODULE_PATH() {
            static auto path = [] {
                WCHAR module_path[MAX_PATH] = {0} ;
                if(GetModuleFileNameW(NULL, module_path, MAX_PATH) == 0) {
                    return std::filesystem::path() ;
                }
                return std::filesystem::path(module_path) ;
            } () ;
            return path ;
        }

        const std::filesystem::path& MODULE_ROOT_PATH() {
#ifdef DEBUG
            //project root
            static const auto path = [] {
                WCHAR root_path[MAX_PATH] = {0} ;
                if(GetCurrentDirectory(MAX_PATH, root_path) == 0) {
                    return std::filesystem::path() ;
                }
                return std::filesystem::path(root_path) ;
            } () ;
#else
            static const auto path = MODULE_PATH().parent_path() ;
#endif
            return path ;
        }

        const std::filesystem::path& ROOT_PATH() {
            static const auto path = is_installer_used() ? HOME_PATH() / ".win-vind" : MODULE_ROOT_PATH() ;
            return path ;
        }

        const std::filesystem::path& CONFIG_PATH() {
            static const auto path = is_installer_used() ? ROOT_PATH() : ROOT_PATH() / "config" ;
            return path ;
        }

        const std::filesystem::path& RESOUECE_ROOT_PATH() {
#if defined(DEBUG)
            static const auto path = MODULE_ROOT_PATH() / "res" ;
#else
            static const auto path = MODULE_ROOT_PATH() ;
#endif
            return path ;
        }

        const std::filesystem::path& DEFAULT_CONFIG_PATH() {
#if defined(DEBUG)
            static const auto path = MODULE_ROOT_PATH() / "res" / "default_config" ;
#else
            static const auto path = MODULE_ROOT_PATH() / "default_config" ;
#endif
            return path ;
        }
    }
}
