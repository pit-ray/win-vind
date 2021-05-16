#include "bind/syscmd/set.hpp"

#include <iostream>
#include <string>

#include "bind/binded_func.hpp"
#include "bind/binded_func_creator.hpp"
#include "entry.hpp"
#include "g_params.hpp"
#include "key/char_logger.hpp"
#include "opt/virtual_cmd_line.hpp"
#include "parser/rc_parser.hpp"
#include "util/def.hpp"
#include "util/string.hpp"

namespace vind
{
    SyscmdSet::SyscmdSet()
    : BindedFuncCreator("system_command_set")
    {}

    void SyscmdSet::sprocess(
            const std::string& args,
            bool reload_config) {

        auto [key, val] = rcparser::divide_key_and_value(args, "=") ;

        if(key.empty()) {
            if(val.empty()) {
                VirtualCmdLine::msgout("E: Not support list of option yet") ;
            }
            else {
                VirtualCmdLine::msgout("E: Invalid syntax") ;
            }
            return ;
        }

        key = util::A2a(key) ;

        if(val.empty()) { // set option_name
            if(key.find(" ") != std::string::npos) {
                VirtualCmdLine::msgout("E: Unknown option") ;
                return ;
            }

            if(key.size() > 2 && key[0] == 'n' && key[1] == 'o') {
                gparams::set(key.substr(2), false) ;
            }
            else {
                gparams::set(key, true) ;
            }
        }
        else { // set option_name = value
            if(val.find_first_not_of("0123456789.") == std::string::npos) {
                gparams::set(key, std::stod(val)) ;
            }
            else {
                gparams::set(key, val) ;
            }
        }

        if(reload_config) {
            vind::reconstruct_all_components() ;
        }
    }

    void SyscmdSet::sprocess(NTypeLogger&) {
        return ;
    }

    void SyscmdSet::sprocess(const CharLogger& parent_lgr) {
        auto str = parent_lgr.to_str() ;
        if(str.empty()) {
            return ;
        }

        auto [cmd, args] = rcparser::divide_cmd_and_args(str) ;
        sprocess(args, true) ;
    }
}
