#ifndef _PATH_HPP
#define _PATH_HPP

#include <filesystem>
#include <string>


namespace vind
{
    namespace core {
        std::filesystem::path replace_path_magic(std::filesystem::path path) ;
        std::string replace_path_magic(std::string path) ;

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

        const std::filesystem::path& RESOURCE_ROOT_PATH() ;

        inline const auto& RC() {
            static const auto obj = CONFIG_PATH() / ".vindrc" ;
            return obj ;
        }
    }
}

#endif
