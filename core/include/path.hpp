#ifndef _PATH_HPP
#define _PATH_HPP

#include <string>
#include <fstream>

#if defined(_MSC_VER) && _MSC_VER >= 1500
#include <filesystem>
#endif

#include <windows.h>
#include <userenv.h>

#include "i_params.hpp"
#include "utility.hpp"

namespace Path
{
    template <typename T>
    inline static auto to_u8path(T&& str) {
#if defined(_MSC_VER) && _MSC_VER >= 1500
#pragma warning(disable : 4996)
        return std::filesystem::u8path(std::forward<T>(str)) ;
#pragma warning(default : 4996)
#else
        return std::forward<T>(str) ;
#endif
    }

    inline static const auto& HOME_PATH() {
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

            return Utility::ws_to_s(path) + '\\' ;
        } () ;

        return obj ;
    }

    inline static const auto& MODULE_ROOT_PATH() {
#ifdef DEBUG
        static const auto path = std::string("") ; //project root
#else
        static const auto path = [] {
            WCHAR module_path[MAX_PATH] = {0} ;
            if(GetModuleFileNameW(NULL, module_path, MAX_PATH) == 0) {
                return std::string() ;
            }
            auto module_path_str = Utility::ws_to_s(module_path) ;
            auto root_dir_pos = module_path_str.find_last_of("/\\") ;
            if(root_dir_pos == std::string::npos) {
                return std::string() ;
            }

            return module_path_str.substr(0, root_dir_pos + 1) ;
        }() ;
#endif
        return path ;
    }

    inline static auto& _is_installer_used() {
        static const auto flag = [] {
            std::ifstream ifs{Path::to_u8path(MODULE_ROOT_PATH() + "default_config\\is_installer_used")} ;
            std::string str{} ;
            std::getline(ifs, str) ;
            return str.front() == 'y' || str.front() == 'Y' ;
        }() ;
        return flag ;
    }

    inline static const auto& ROOT_PATH() {
        static const auto path = _is_installer_used() ? HOME_PATH() + ".win-vind\\" : MODULE_ROOT_PATH() ;
        return path ;
    }

    inline static const auto& CONFIG_PATH() {
        static const auto path = _is_installer_used() ? ROOT_PATH() : ROOT_PATH() + "config\\" ;
        return path ;
    }

    inline static const auto& BINDINGS() {
        static const auto obj = CONFIG_PATH() + "bindings.json" ;
        return obj ;
    }
    inline static const auto& SETTINGS() {
        static const auto obj = CONFIG_PATH() + "settings.json" ;
        return obj ;
    }

    inline static const auto KEYBRD_MAP() {
        return CONFIG_PATH() + iParams::get_s("kb_type") ;
    }

    namespace Default {
        inline static const auto& BINDINGS() {
            static const auto& obj = MODULE_ROOT_PATH() + "default_config\\bindings.json" ;
            return obj ;
        }
        inline static const auto& SETTINGS() {
            static const auto obj = MODULE_ROOT_PATH() + "default_config\\settings.json" ;
            return obj ;
        }
        inline static const auto& UI() {
            static const auto& obj = MODULE_ROOT_PATH() + "default_config\\ui.json" ;
            return obj ;
        }
    }
}

#endif
