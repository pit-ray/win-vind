#ifndef _PATH_HPP
#define _PATH_HPP
#include "msg_logger.hpp"

#include <string>
#include <fstream>
#include <windows.h>

namespace Path
{
    static constexpr auto _EXECUTION_MODE_IDX() noexcept {
        return "admin_config/execution_mode.idx" ;
    }

    static const auto& _USE_INST_CONFIG_DIR() noexcept {
        static const std::string obj(".win-vind") ;
        return obj ;
    }
    static const auto&  _UNUSE_INST_CONFIG_DIR() noexcept {
        static const std::string obj("config") ;
        return obj ;
    }

    static inline const auto _get_home_path() noexcept {
        char home_path[200] = {0} ;
        if(!GetEnvironmentVariableA("HOMEPATH", home_path, 1000)) {
            WIN_ERROR_STREAM << "cannot find %HOMEPATH% (DefaultConfig::get_home_path::GetEnviromentVariable)\n" ;
            return std::string() ;
        }
        return std::string(home_path) + '\\' ;
    }

    inline static const auto& HOME_PATH() noexcept {
        static const auto obj = _get_home_path() ;
        return obj ;
    }

    inline static const auto _get_path(std::string filename) noexcept {
        static const auto use_installer = [] {
            std::ifstream ifs{_EXECUTION_MODE_IDX()} ;
            std::string index_str{} ;
            std::getline(ifs, index_str) ;
            return index_str.front() == '1' ;
        }() ;

        if(!use_installer) {
            return _UNUSE_INST_CONFIG_DIR() +"\\" + filename ;
        }
        return HOME_PATH() + _USE_INST_CONFIG_DIR() + "\\" + filename ;
    }

    inline static const auto& CONFIG_INI() noexcept {
        static const auto obj = _get_path("custom.ini") ;
        return obj ;
    }

    inline static const auto& CONFIG_OPTS_INI() noexcept {
        static const auto obj = _get_path("custom_opts.ini") ;
        return obj ;
    }

    inline static const auto& CONFIG_OPTS_BOOL_INI() noexcept {
        static const auto obj = _get_path("custom_opts_bool.ini") ;
        return obj ;
    }

    inline static const auto& CONFIG_XML() noexcept {
        static const auto obj = _get_path("custom.xml") ;
        return obj ;
    }

    inline static const auto& CONFIG_EXAPP_INI() noexcept {
        static const auto obj = _get_path("custom_exapp.ini") ;
        return obj ;
    }

    inline static const auto& KBTYPE_PTH() noexcept {
        static const auto obj = _get_path("custom_kbtype.pth") ;
        return obj ;
    }

    inline static const auto& KEYBRD_MAP() noexcept {
        static const auto obj = _get_path([] {
            std::ifstream ifs{KBTYPE_PTH()} ;
            std::string filename{} ;
            std::getline(ifs, filename) ;
            return filename ;
        }()) ;
        return obj ;
    }
}

#endif