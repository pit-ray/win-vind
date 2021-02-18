#ifndef _PATH_HPP
#define _PATH_HPP

#include <string>
#include <fstream>
#include <windows.h>

#include "i_params.hpp"
#include "utility.hpp"

namespace Path
{
    static inline const auto _get_home_path() {
        WCHAR home_path[MAX_PATH] = {0} ;
        if(!GetEnvironmentVariableW(L"HOMEPATH", home_path, MAX_PATH)) {
            throw RUNTIME_EXCEPT("Cannot find %HOMEPATH% with GetEnviromentVariable.") ;
        }

        return Utility::ws_to_s(home_path) + '\\' ;
    }

    inline static const auto& HOME_PATH() {
        static const auto obj = _get_home_path() ;
        return obj ;
    }

    inline static auto& _is_installer_used() {
        static const auto flag = [] {
            std::ifstream ifs{"default_config/is_installer_used"} ;
            std::string str{} ;
            std::getline(ifs, str) ;
            return str.front() == 'y' || str.front() == 'Y' ;
        }() ;
        return flag ;
    }

    inline static const auto& ROOT_PATH() {
        static const auto path = _is_installer_used() ? HOME_PATH() + ".win-vind\\" : std::string("") ;
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
        inline static const auto BINDINGS() {
            return std::string("default_config/bindings.json") ;
        }
        inline static const auto SETTINGS() {
            return std::string("default_config/settings.json") ;
        }
        inline static const auto UI() {
            return std::string("default_config/ui.json") ;
        }
    }
}

#endif
