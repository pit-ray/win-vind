#include "mkdir.hpp"

#include "core/errlogger.hpp"
#include "core/path.hpp"

#include "util/def.hpp"
#include "util/string.hpp"
#include "util/winwrap.hpp"

#include "bind/file/explorer_util.hpp"


namespace vind
{
    namespace bind
    {
        MakeDir::MakeDir()
        : BindedFuncVoid("makedir")
        {}
        void MakeDir::sprocess(
                std::uint16_t UNUSED(count),
                const std::string& args) {
            std::filesystem::path path = util::trim(args) ;

            if(path.is_absolute()) {
                if(!std::filesystem::create_directories(path)) {
                    throw RUNTIME_EXCEPT("Could not create directories.") ;
                }
            }

            //pathument is directory name
            //get current directory
            auto current_path = get_current_explorer_path() ;
            if(current_path.empty()) {
                current_path = core::HOME_PATH() / "Desktop" ;
            }

            auto full_path = current_path / path ;

            std::filesystem::create_directories(full_path) ;
        }
    }
}
