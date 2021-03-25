#ifndef _PATH_HPP
#define _PATH_HPP

#include <string>

#if defined(_MSC_VER) && _MSC_VER >= 1500
#include <filesystem>
#endif

#include "i_params.hpp"

namespace vind
{
    namespace Path {
        template <typename T>
        inline auto to_u8path(T&& str) {
#if defined(_MSC_VER) && _MSC_VER >= 1500
#pragma warning(disable : 4996)
            return std::filesystem::u8path(std::forward<T>(str)) ;
#pragma warning(default : 4996)
#else
            return std::forward<T>(str) ;
#endif
        }

        const std::string& HOME_PATH() ;

        const std::string& MODULE_ROOT_PATH() ;

        const std::string& ROOT_PATH() ;

        const std::string& CONFIG_PATH() ;

        inline const auto& BINDINGS() {
            static const auto obj = CONFIG_PATH() + "bindings.json" ;
            return obj ;
        }
        inline const auto& SETTINGS() {
            static const auto obj = CONFIG_PATH() + "settings.json" ;
            return obj ;
        }

        inline const std::string KEYBRD_MAP() {
            return CONFIG_PATH() + iParams::get_s("kb_type") ;
        }

        namespace Default {
            inline const auto& BINDINGS() {
                static const auto& obj = MODULE_ROOT_PATH() + "default_config\\bindings.json" ;
                return obj ;
            }
            inline const auto& SETTINGS() {
                static const auto obj = MODULE_ROOT_PATH() + "default_config\\settings.json" ;
                return obj ;
            }
            inline const auto& UI() {
                static const auto& obj = MODULE_ROOT_PATH() + "default_config\\ui.json" ;
                return obj ;
            }
        }
    }
}

#endif
