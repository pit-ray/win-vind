#include "set.hpp"

#include <sstream>
#include <stdexcept>
#include <string>

#include "bind/binded_func.hpp"
#include "bind/binded_func.hpp"
#include "core/charlogger.hpp"
#include "core/entry.hpp"
#include "core/rcparser.hpp"
#include "core/settable.hpp"
#include "opt/vcmdline.hpp"
#include "util/debug.hpp"
#include "util/def.hpp"
#include "util/string.hpp"


namespace vind
{
    namespace bind
    {
        SyscmdSet::SyscmdSet()
        : BindedFuncFlex("system_command_set")
        {}

        SystemCall SyscmdSet::sprocess(const std::string& args) {
            auto& settable = core::SetTable::get_instance() ;

            if(args.find("=") != std::string::npos) { // set option_name = value
                auto [key, val] = core::divide_key_and_value(args, "=") ;

                if(key.empty()) {
                    if(val.empty()) {
                        opt::VCmdLine::print(opt::ErrorMessage("E: Not support list of option yet")) ;
                    }
                    else {
                        opt::VCmdLine::print(opt::ErrorMessage("E: Invalid syntax")) ;
                    }
                }
                else {
                    key = util::A2a(key) ;

                    if(val.empty()) { // empty string
                        settable.set(key, val) ;
                    }
                    else if(val.find_first_not_of("0123456789.") == std::string::npos) { // number
                        if(val.find_first_of(".") != std::string::npos) {
                            settable.set(key, std::stof(val)) ;
                        }
                        else { 
                            settable.set(key, std::stol(val)) ;
                        }
                    }
                    else { // string
                        settable.set(key, val) ;
                    }
                }

                return SystemCall::NOTHING ;
            }

            // set option_name
            auto key = core::extract_single_arg(util::A2a(args)) ;

            bool flag_value = true ;
            if(key.size() > 2 && key[0] == 'n' && key[1] == 'o') {
                key.erase(0, 2) ;
                flag_value = false ;
            }

            // reference of value
            if(key.back() == '?') {
                auto name_last_pos = key.find_last_not_of(" ", key.size() - 2) ;
                key.erase(name_last_pos + 1) ;

                if(key.find(" ") != std::string::npos) {
                    opt::VCmdLine::print(opt::ErrorMessage("E: Unknown option: " + key)) ;
                    return SystemCall::NOTHING ;
                }

                std::stringstream ss ;

                try {
                    const auto& param = settable.get(key) ;
                    if(param.is_bool()) {
                        if(settable.get(key).get<bool>()) {
                            ss << key ;
                        }
                        else {
                            ss << "no" << key ;
                        }
                    }
                    else {
                        ss << key << "=" ;
                        ss << settable.get(key).get<std::string>() ;
                    }
                }
                catch(const std::out_of_range&) {
                    ss << "E: Unknown option: " << key ;
                }

                opt::VCmdLine::print(opt::ErrorMessage(ss.str())) ;
            }
            else {
                if(key.find(" ") != std::string::npos) {
                    opt::VCmdLine::print(opt::ErrorMessage("E: Unknown option: " + key)) ;
                }
                else {
                    settable.set(key, flag_value) ;
                }
            }

            return SystemCall::NOTHING ;
        }

        SystemCall SyscmdSet::sprocess(core::NTypeLogger&) {
            return SystemCall::NOTHING ;
        }

        SystemCall SyscmdSet::sprocess(const core::CharLogger& parent_lgr) {
            auto str = parent_lgr.to_str() ;
            if(str.empty()) {
                return SystemCall::NOTHING ;
            }

            auto [cmd, args] = core::divide_cmd_and_args(str) ;
            sprocess(args) ;
            return SystemCall::RECONSTRUCT ;
        }
    }
}
