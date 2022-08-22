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
        SystemCall SyscmdCommand::sprocess(
                std::uint16_t count, const std::string& args) {
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

        // delcommand
        SyscmdDelcommand::SyscmdDelcommand()
        : BindedFuncFlex("system_command_delcommand")
        {}
        SystemCall SyscmdDelcommand::sprocess(
                std::uint16_t count, const std::string& args) {
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

        // comclear
        SyscmdComclear::SyscmdComclear()
        : BindedFuncFlex("system_command_comclear")
        {}
        SystemCall SyscmdComclear::sprocess(
                std::uint16_t count, const std::string& args) {
            auto& maptable = core::MapTable::get_instance() ;
            maptable.clear(core::Mode::COMMAND) ;
            return SystemCall::NOTHING ;
        }
    }
}
