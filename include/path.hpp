#ifndef _PATH_HPP
#define _PATH_HPP

#include <string>
#include <fstream>
#include <windows.h>

#include "msg_logger.hpp"
#include "i_params.hpp"

namespace Path
{
    static inline const auto _get_home_path() noexcept {
        char home_path[200] = {0} ;
        if(!GetEnvironmentVariableA("HOMEPATH", home_path, 1000)) {
            WIN_ERROR_PRINT("cannot find %HOMEPATH% in GetEnviromentVariable") ;
            return std::string() ;
        }
        return std::string(home_path) + '\\' ;
    }

    inline static const auto& HOME_PATH() noexcept {
        static const auto obj = _get_home_path() ;
        return obj ;
    }

    inline static const auto _get_path(std::string filename) noexcept {
        static const auto flag = [] {
            std::ifstream ifs{"default_config/is_installer_used"} ;
            std::string str{} ;
            std::getline(ifs, str) ;
            return str.front() == 'y' || str.front() == 'Y' ;
        }() ;
        return flag ? (HOME_PATH() + ".win-vind\\" + filename) : ("config\\" + filename) ;
    }

    inline static const auto& BINDINGS() noexcept {
        static const auto obj = _get_path("bindings.json") ;
        return obj ;
    }
    inline static const auto& SETTINGS() noexcept {
        static const auto obj = _get_path("settings.json") ;
        return obj ;
    }

    inline static const auto KEYBRD_MAP() noexcept {
        return _get_path(iParams::get_s("kb_type")) ;
    }

    namespace Default {
        inline static const auto BINDINGS() noexcept {
            return std::string("default_config/bindings.json") ;
        }
        inline static const auto SETTINGS() noexcept {
            return std::string("default_config/settings.json") ;
        }
        inline static const auto UI() noexcept {
            return std::string("default_config/ui.json") ;
        }
    }
}

#endif
