#include "command.hpp"

#include "core/char_logger.hpp"
#include "core/entry.hpp"
#include "core/err_logger.hpp"
#include "core/g_maps.hpp"
#include "core/rc_parser.hpp"
#include "opt/vcmdline.hpp"
#include "util/def.hpp"

namespace vind
{
    namespace bind
    {
        // command
        SyscmdCommand::SyscmdCommand()
        : BindedFuncFlex("system_command_command")
        {}
        core::SystemCall SyscmdCommand::sprocess(const std::string& args) {
            if(args.empty()) {
                opt::VCmdLine::print(opt::ErrorMessage("E: Not support list of command yet")) ;
                return core::SystemCall::NOTHING ;
            }

            auto [arg1, arg2] = core::extract_double_args(args) ;
            if(arg1.empty()) {
                opt::VCmdLine::print(opt::ErrorMessage("E: Not support list of command yet")) ;
            }
            else if(arg2.empty()) {
                opt::VCmdLine::print(opt::ErrorMessage("E: Not support reference of command yet")) ;
            }
            else {
                core::do_noremap(arg1, arg2, core::Mode::COMMAND) ;
            }

            return core::SystemCall::NOTHING ;
        }
        core::SystemCall SyscmdCommand::sprocess(core::NTypeLogger&) {
            return core::SystemCall::NOTHING ;
        }
        core::SystemCall SyscmdCommand::sprocess(const core::CharLogger& parent_lgr) {
            auto str = parent_lgr.to_str() ;
            if(str.empty()) {
                throw RUNTIME_EXCEPT("Empty command") ;
            }
            auto [cmd, args] = core::divide_cmd_and_args(str) ;
            sprocess(args) ;

            return core::SystemCall::RECONSTRUCT ;
        }


        // delcommand
        SyscmdDelcommand::SyscmdDelcommand()
        : BindedFuncFlex("system_command_delcommand")
        {}
        core::SystemCall SyscmdDelcommand::sprocess(const std::string& args) {
            if(args.empty()) {
                // does not have argument is empty
                opt::VCmdLine::print(opt::ErrorMessage("E: Invalid argument")) ;
                return core::SystemCall::NOTHING ;
            }

            auto arg = core::extract_single_arg(args) ;
            if(arg.empty()) {
                opt::VCmdLine::print(opt::ErrorMessage("E: Invalid argument")) ;
            }
            else {
                core::do_unmap(arg, core::Mode::COMMAND) ;
            }

            return core::SystemCall::NOTHING ;
        }
        core::SystemCall SyscmdDelcommand::sprocess(core::NTypeLogger&) {
            return core::SystemCall::NOTHING ;
        }
        core::SystemCall SyscmdDelcommand::sprocess(const core::CharLogger& parent_lgr) {
            auto str = parent_lgr.to_str() ;
            if(str.empty()) {
                throw RUNTIME_EXCEPT("Empty command") ;
            }

            auto [cmd, args] = core::divide_cmd_and_args(str) ;
            sprocess(args) ;

            return core::SystemCall::RECONSTRUCT ;
        }

        // comclear
        SyscmdComclear::SyscmdComclear()
        : BindedFuncFlex("system_command_comclear")
        {}
        core::SystemCall SyscmdComclear::sprocess() {
            core::do_mapclear(core::Mode::COMMAND) ;
            return core::SystemCall::NOTHING ;
        }
        core::SystemCall SyscmdComclear::sprocess(core::NTypeLogger&) {
            return core::SystemCall::NOTHING ;
        }
        core::SystemCall SyscmdComclear::sprocess(const core::CharLogger& parent_lgr) {
            auto str = parent_lgr.to_str() ;
            if(str.empty()) {
                throw RUNTIME_EXCEPT("Empty command") ;
            }

            auto [cmd, args] = core::divide_cmd_and_args(str) ;
            if(!args.empty()) {
                opt::VCmdLine::print(opt::ErrorMessage("E: Invalid argument")) ;
                return core::SystemCall::NOTHING ;
            }

            sprocess() ;
            return core::SystemCall::RECONSTRUCT ;
        }
    }
}
