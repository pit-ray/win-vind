#include "bind/file/mkdir.hpp"

#include "err_logger.hpp"
#include "key/char_logger.hpp"
#include "key/ntype_logger.hpp"
#include "path.hpp"

#include "util/def.hpp"
#include "util/string.hpp"
#include "util/winwrap.hpp"

#include "bind/file/explorer_util.hpp"

namespace vind
{
    MakeDir::MakeDir()
    : BindedFuncCreator("makedir")
    {}
    void MakeDir::sprocess(const std::string& path) {
        if(path.find("\\") != std::string::npos ||
                path.find("/") != std::string::npos) {
            //pathument is directory path
            if(path.length() > 248) {
                //over max path num
                util::create_directory(path.substr(0, 248)) ;
            }
            else {
                util::create_directory(path) ;
            }
        }

        //pathument is directory name
        //get current directory
        auto current_path = explorer::get_current_explorer_path() ;
        if(current_path.empty()) {
            current_path = path::HOME_PATH() + "\\Desktop" ;
        }

        auto full_path = current_path + "\\" + path ;
        util::create_directory(full_path) ;
    }
    void MakeDir::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess() ;
        }
    }
    void MakeDir::sprocess(const CharLogger& parent_lgr) {
        auto cmd = parent_lgr.to_str() ;
        auto pos = cmd.find_first_of(" ") ;
        sprocess(cmd.substr(pos + 1)) ;
    }
}
