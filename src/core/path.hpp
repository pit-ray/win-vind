#ifndef _PATH_HPP
#define _PATH_HPP

#include <filesystem>
#include <string>

#include "g_params.hpp"

namespace vind
{
    namespace path {
        std::filesystem::path replace_magic(std::filesystem::path path) ;
        std::string replace_magic(std::string path) ;

        enum class InstallType {
            PORTABLE,
            INSTALLER,
            CHOCOLATEY,
        } ;

        InstallType get_install_type() ;

        bool is_installer_used() ;

        const std::filesystem::path& HOME_PATH() ;

        const std::filesystem::path& MODULE_PATH() ;

        const std::filesystem::path& MODULE_ROOT_PATH() ;

        const std::filesystem::path& ROOT_PATH() ;

        const std::filesystem::path& CONFIG_PATH() ;

        const std::filesystem::path& RESOUECE_ROOT_PATH() ;

        const std::filesystem::path& DEFAULT_CONFIG_PATH() ;

        inline const auto& RC() {
            static const auto obj = CONFIG_PATH() / ".vindrc" ;
            return obj ;
        }

        inline const auto& BINDINGS() {
            static const auto obj = CONFIG_PATH() / "bindings.json" ;
            return obj ;
        }
        inline const auto& SETTINGS() {
            static const auto obj = CONFIG_PATH() / "settings.json" ;
            return obj ;
        }

        namespace Default {
            inline const auto& BINDINGS() {
                static const auto& obj = DEFAULT_CONFIG_PATH() / "bindings.json" ;
                return obj ;
            }
            inline const auto& SETTINGS() {
                static const auto obj = DEFAULT_CONFIG_PATH() / "settings.json" ;
                return obj ;
            }
        }
    }
}

#endif
