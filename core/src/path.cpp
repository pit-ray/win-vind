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

        std::string replace_magic(std::string path) {
            path = util::replace_all(path, "~", path::HOME_PATH()) ;
            path = util::replace_all(path, "/", "\\") ;
            return path ;
        }

        InstallType get_install_type() {
            static const auto type = [] {
                std::ifstream ifs{to_u8path(DEFAULT_CONFIG_PATH() + "\\instype")} ;
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

        const std::string& HOME_PATH() {
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

                return util::ws_to_s(path) ;
            } () ;

            return obj ;
        }

        const std::string& MODULE_PATH() {
            static auto path = [] {
                WCHAR module_path[MAX_PATH] = {0} ;
                if(GetModuleFileNameW(NULL, module_path, MAX_PATH) == 0) {
                    return std::string() ;
                }
                return util::ws_to_s(module_path) ;
            } () ;
            return path ;
        }

        const std::string& MODULE_ROOT_PATH() {
#ifdef DEBUG
            //project root
            static const auto path = [] {
                WCHAR root_path[MAX_PATH] = {0} ;
                if(GetCurrentDirectory(MAX_PATH, root_path) == 0) {
                    return std::string() ;
                }
                return util::ws_to_s(root_path) ;
            } () ;
#else
            static const auto path = [] {
                auto module_path_str = MODULE_PATH() ;
                auto root_dir_pos = module_path_str.find_last_of("/\\") ;
                if(root_dir_pos == std::string::npos) {
                    return std::string() ;
                }

                return module_path_str.substr(0, root_dir_pos) ;
            } () ;
#endif
            return path ;
        }

        const std::string& ROOT_PATH() {
            static const auto path = is_installer_used() ? HOME_PATH() + "\\.win-vind" : MODULE_ROOT_PATH() ;
            return path ;
        }

        const std::string& CONFIG_PATH() {
            static const auto path = is_installer_used() ? ROOT_PATH() : ROOT_PATH() + "\\config" ;
            return path ;
        }

        const std::string& RESOUECE_ROOT_PATH() {
#if defined(DEBUG)
            static const auto path = MODULE_ROOT_PATH() + "\\res" ;
#else
            static const auto path = MODULE_ROOT_PATH() ;
#endif
            return path ;
        }

        const std::string& DEFAULT_CONFIG_PATH() {
#if defined(DEBUG)
            static const auto path = MODULE_ROOT_PATH() + "\\res\\default_config" ;
#else
            static const auto path = MODULE_ROOT_PATH() + "\\default_config" ;
#endif
            return path ;
        }
    }
}
