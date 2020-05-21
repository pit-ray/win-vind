#ifndef _PATH_HPP
#define _PATH_HPP
#include "msg_logger.hpp"

#include <string>
#include <fstream>
#include <windows.h>

namespace Path
{
    static constexpr auto _USE_INSTALLER = false ;
    static const std::string _USE_INST_CONFIG_DIR{".win-vind"} ;
    static const std::string _UNUSE_INST_CONFIG_DIR{"config"} ;

    static inline const auto _get_home_path() noexcept {
        char home_path[200] = {0} ;
        if(!GetEnvironmentVariableA("HOMEPATH", home_path, 1000)) {
            ERROR_STREAM << "windows.h: " << GetLastError() << ", cannot find %HOMEPATH% (DefaultConfig::get_home_path::GetEnviromentVariable)\n" ;
            return std::string() ;
        }

        std::string str(home_path) ;
        str.push_back('\\') ;

        return std::move(str) ;
    }

    inline static const auto& HOME_PATH() noexcept {
        static const auto obj = _get_home_path() ;
        return obj ;
    }

    template <typename T>
    inline static const auto _get_path(const T& filename) noexcept {
        using namespace std ;
        string fstr(filename) ;

        if(!_USE_INSTALLER) {
            fstr = _UNUSE_INST_CONFIG_DIR +"\\" + fstr ;
            return move(fstr) ;
        }

        fstr = HOME_PATH() + _USE_INST_CONFIG_DIR + "\\" + fstr ;

        return std::move(fstr) ;
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
        static const auto get_file_name = [] {
            std::ifstream ifs{KBTYPE_PTH()} ;
            std::string filename{} ;
            std::getline(ifs, filename) ;
            return filename ;
        } ;

        static const auto obj = _get_path(get_file_name());
        return obj ;
    }
}

#endif