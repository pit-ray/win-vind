#include "command.hpp"

#include "core/charlogger.hpp"
#include "core/entry.hpp"
#include "core/errlogger.hpp"
#include "core/inputhub.hpp"
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
                std::uint16_t UNUSED(count),
                const std::string& args) {
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
                auto& ihub = core::InputHub::get_instance() ;
                // No argument version
                ihub.add_noremap(arg1, arg2, core::Mode::COMMAND) ;

                // To accept arguments
                ihub.add_noremap(arg1 + " <any>", arg2, core::Mode::COMMAND) ;
            }

            return SystemCall::NOTHING ;
        }

        // delcommand
        SyscmdDelcommand::SyscmdDelcommand()
        : BindedFuncFlex("system_command_delcommand")
        {}
        SystemCall SyscmdDelcommand::sprocess(
                std::uint16_t UNUSED(count),
                const std::string& args) {
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
                auto& ihub = core::InputHub::get_instance() ;
                ihub.remove_mapping(arg, core::Mode::COMMAND) ;
            }

            return SystemCall::NOTHING ;
        }

        // comclear
        SyscmdComclear::SyscmdComclear()
        : BindedFuncFlex("system_command_comclear")
        {}
        SystemCall SyscmdComclear::sprocess(
                std::uint16_t UNUSED(count),
                const std::string& UNUSED(args)) {
            core::InputHub::get_instance().clear_mapping(core::Mode::COMMAND) ;
            return SystemCall::NOTHING ;
        }
    }
}
