#include "command.hpp"

#include "core/charlogger.hpp"
#include "core/entry.hpp"
#include "core/errlogger.hpp"
#include "core/maptable.hpp"
#include "core/rcparser.hpp"
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
        SystemCall SyscmdCommand::sprocess(const std::string& args) {
            if(args.empty()) {
                opt::VCmdLine::print(opt::ErrorMessage("E: Not support list of command yet")) ;
                return SystemCall::NOTHING ;
            }

            auto [arg1, arg2] = core::extract_double_args(args) ;
            if(arg1.empty()) {
                opt::VCmdLine::print(opt::ErrorMessage("E: Not support list of command yet")) ;
            }
            else if(arg2.empty()) {
                opt::VCmdLine::print(opt::ErrorMessage("E: Not support reference of command yet")) ;
            }
            else {
                auto& maptable = core::MapTable::get_instance() ;
                maptable.add_noremap(arg1, arg2, core::Mode::COMMAND) ;
            }

            return SystemCall::NOTHING ;
        }
        SystemCall SyscmdCommand::sprocess(core::NTypeLogger&) {
            return SystemCall::NOTHING ;
        }
        SystemCall SyscmdCommand::sprocess(const core::CharLogger& parent_lgr) {
            auto str = parent_lgr.to_str() ;
            if(str.empty()) {
                throw RUNTIME_EXCEPT("Empty command") ;
            }
            auto [cmd, args] = core::divide_cmd_and_args(str) ;
            sprocess(args) ;

            return SystemCall::RECONSTRUCT ;
        }


        // delcommand
        SyscmdDelcommand::SyscmdDelcommand()
        : BindedFuncFlex("system_command_delcommand")
        {}
        SystemCall SyscmdDelcommand::sprocess(const std::string& args) {
            if(args.empty()) {
                // does not have argument is empty
                opt::VCmdLine::print(opt::ErrorMessage("E: Invalid argument")) ;
                return SystemCall::NOTHING ;
            }

            auto arg = core::extract_single_arg(args) ;
            if(arg.empty()) {
                opt::VCmdLine::print(opt::ErrorMessage("E: Invalid argument")) ;
            }
            else {
                auto& maptable = core::MapTable::get_instance() ;
                maptable.remove(arg, core::Mode::COMMAND) ;
            }

            return SystemCall::NOTHING ;
        }
        SystemCall SyscmdDelcommand::sprocess(core::NTypeLogger&) {
            return SystemCall::NOTHING ;
        }
        SystemCall SyscmdDelcommand::sprocess(const core::CharLogger& parent_lgr) {
            auto str = parent_lgr.to_str() ;
            if(str.empty()) {
                throw RUNTIME_EXCEPT("Empty command") ;
            }

            auto [cmd, args] = core::divide_cmd_and_args(str) ;
            sprocess(args) ;

            return SystemCall::RECONSTRUCT ;
        }

        // comclear
        SyscmdComclear::SyscmdComclear()
        : BindedFuncFlex("system_command_comclear")
        {}
        SystemCall SyscmdComclear::sprocess() {
            auto& maptable = core::MapTable::get_instance() ;
            maptable.clear(core::Mode::COMMAND) ;
            return SystemCall::NOTHING ;
        }
        SystemCall SyscmdComclear::sprocess(core::NTypeLogger&) {
            return SystemCall::NOTHING ;
        }
        SystemCall SyscmdComclear::sprocess(const core::CharLogger& parent_lgr) {
            auto str = parent_lgr.to_str() ;
            if(str.empty()) {
                throw RUNTIME_EXCEPT("Empty command") ;
            }

            auto [cmd, args] = core::divide_cmd_and_args(str) ;
            if(!args.empty()) {
                opt::VCmdLine::print(opt::ErrorMessage("E: Invalid argument")) ;
                return SystemCall::NOTHING ;
            }

            sprocess() ;
            return SystemCall::RECONSTRUCT ;
        }
    }
}
