#include "coreio/path.hpp"

#include <windows.h>
#include <userenv.h>

#include <fstream>

#include "util/def.hpp"
#include "util/string.hpp"

namespace
{
    using namespace vind::path ;
    inline auto& is_installer_used() {
        static const auto flag = [] {
            std::ifstream ifs{to_u8path(MODULE_ROOT_PATH() + "default_config\\is_installer_used")} ;
            std::string str{} ;
            std::getline(ifs, str) ;
            return str.front() == 'y' || str.front() == 'Y' ;
        }() ;
        return flag ;
    }
}

namespace vind
{
    namespace path {
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

                return util::ws_to_s(path) + '\\' ;
            } () ;

            return obj ;
        }

        const std::string& MODULE_ROOT_PATH() {
#ifdef DEBUG
            static const auto path = std::string("") ; //project root
#else
            static const auto path = [] {
                WCHAR module_path[MAX_PATH] = {0} ;
                if(GetModuleFileNameW(NULL, module_path, MAX_PATH) == 0) {
                    return std::string() ;
                }
                auto module_path_str = util::ws_to_s(module_path) ;
                auto root_dir_pos = module_path_str.find_last_of("/\\") ;
                if(root_dir_pos == std::string::npos) {
                    return std::string() ;
                }

                return module_path_str.substr(0, root_dir_pos + 1) ;
            }() ;
#endif
            return path ;
        }

        const std::string& ROOT_PATH() {
            static const auto path = is_installer_used() ? HOME_PATH() + ".win-vind\\" : MODULE_ROOT_PATH() ;
            return path ;
        }

        const std::string& CONFIG_PATH() {
            static const auto path = is_installer_used() ? ROOT_PATH() : ROOT_PATH() + "config\\" ;
            return path ;
        }
    }
}
