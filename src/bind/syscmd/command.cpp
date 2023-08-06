#include "command.hpp"

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
        Command::Command()
        : BindedFuncFlex("command")
        {}
        SystemCall Command::sprocess(
                std::uint16_t UNUSED(count),
                const std::string& args,
                bool as_default) {
            auto [_, pargs] = core::divide_cmd_and_args(args) ;
            if(pargs.empty()) {
                opt::VCmdLine::print(opt::ErrorMessage("E: Not support list of command yet")) ;
                return SystemCall::SUCCEEDED ;
            }

            auto [arg1, arg2] = core::extract_double_args(pargs) ;
            if(arg1.empty()) {
                opt::VCmdLine::print(opt::ErrorMessage("E: Not support list of command yet")) ;
            }
            else if(arg2.empty()) {
                opt::VCmdLine::print(opt::ErrorMessage("E: Not support reference of command yet")) ;
            }
            else {
                auto& ihub = core::InputHub::get_instance() ;
                // No argument version
                ihub.add_noremap(arg1, arg2, core::Mode::COMMAND, as_default) ;

                // To accept arguments
                ihub.add_noremap(arg1 + " <any>", arg2, core::Mode::COMMAND, as_default) ;
            }

            return SystemCall::SUCCEEDED ;
        }

        // delcommand
        Delcommand::Delcommand()
        : BindedFuncFlex("delcommand")
        {}
        SystemCall Delcommand::sprocess(
                std::uint16_t UNUSED(count),
                const std::string& args,
                bool as_default) {
            auto [_, pargs] = core::divide_cmd_and_args(args) ;
            if(pargs.empty()) {
                // does not have argument is empty
                opt::VCmdLine::print(opt::ErrorMessage("E: Invalid argument")) ;
                return SystemCall::SUCCEEDED ;
            }

            auto arg = core::extract_single_arg(pargs) ;
            if(arg.empty()) {
                opt::VCmdLine::print(opt::ErrorMessage("E: Invalid argument")) ;
            }
            else {
                auto& ihub = core::InputHub::get_instance() ;
                ihub.remove_mapping(arg, core::Mode::COMMAND, as_default) ;
            }

            return SystemCall::SUCCEEDED ;
        }

        // comclear
        Comclear::Comclear()
        : BindedFuncFlex("comclear")
        {}
        SystemCall Comclear::sprocess(
                std::uint16_t UNUSED(count),
                const std::string& UNUSED(args),
                bool as_default) {
            core::InputHub::get_instance().clear_mapping(core::Mode::COMMAND, as_default) ;
            return SystemCall::SUCCEEDED ;
        }
    }
}
