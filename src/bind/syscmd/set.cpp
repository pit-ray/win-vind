#include "set.hpp"

#include <sstream>
#include <string>

#include "bind/binded_func.hpp"
#include "bind/binded_func.hpp"
#include "core/char_logger.hpp"
#include "core/entry.hpp"
#include "core/g_params.hpp"
#include "core/rc_parser.hpp"
#include "opt/vcmdline.hpp"
#include "util/def.hpp"
#include "util/string.hpp"

#if defined(DEBUG)
#include <iostream>
#endif

namespace vind
{
    namespace bind
    {
        SyscmdSet::SyscmdSet()
        : BindedFuncFlex("system_command_set")
        {}

        SystemCall SyscmdSet::sprocess(const std::string& args) {
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

                    if(val.empty()) {
                        core::do_set(key, val) ;
                    }
                    else if(val.find_first_not_of("0123456789.") == std::string::npos) {
                        core::do_set(key, std::stod(val)) ;
                    }
                    else {
                        core::do_set(key, val) ;
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
                switch(core::validate_param_type(key)) {
                    using core::ParamType ;
                    case ParamType::BOOL:
                        if(core::get_b(key)) {
                            ss << key ;
                        }
                        else {
                            ss << "no" << key ;
                        }
                        break ;

                    case ParamType::NUMBER: {
                        ss << key << "=" ;
                        auto v_d = core::get_d(key) ;
                        auto v_z = core::get_z(key) ;
                        if(v_d == v_z) {
                            ss << std::to_string(v_z) ;
                        }
                        else {
                            ss << std::to_string(v_d) ;
                        }
                        break ;
                    }

                    case ParamType::STRING:
                        ss << key << "=" << core::get_s(key) ;
                        break ;

                    default:
                        ss << "E: Unknown option: " << key ;
                        break ;
                }

                opt::VCmdLine::print(opt::ErrorMessage(ss.str())) ;
            }
            else {
                if(key.find(" ") != std::string::npos) {
                    opt::VCmdLine::print(opt::ErrorMessage("E: Unknown option: " + key)) ;
                }
                else {
                    core::do_set(key, flag_value) ;
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
