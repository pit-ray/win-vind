#include "bind/syscmd/command.hpp"

#include "err_logger.hpp"
#include "key/char_logger.hpp"
#include "opt/virtual_cmd_line.hpp"
#include "parser/rc_parser.hpp"
#include "util/def.hpp"

namespace vind
{
    // command
    SyscmdCommand::SyscmdCommand()
    : BindedFuncCreator("system_command_command")
    {}
    void SyscmdCommand::sprocess(const std::string& args) {
        if(args.empty()) {
            VirtualCmdLine::msgout("E: Not support list of command yet") ;
            return ;
        }

        auto [arg1, arg2] = rcparser::extract_double_args(args) ;
        if(arg1.empty()) {
            VirtualCmdLine::msgout("E: Not support list of command yet") ;
            return ;
        }
        if(arg2.empty()) {
            VirtualCmdLine::msgout("E: Not support reference of command yet") ;
            return ;
        }

        std::cout << "command " << arg1 << " ==> " << arg2 << std::endl ;

        // mode, src_cmd, target
    }
    void SyscmdCommand::sprocess(NTypeLogger&) {
        return ;
    }
    void SyscmdCommand::sprocess(const CharLogger& parent_lgr) {
        auto str = parent_lgr.to_str() ;
        if(str.empty()) {
            throw RUNTIME_EXCEPT("Empty command") ;
        }
        auto [cmd, args] = rcparser::divide_cmd_and_args(str) ;
        sprocess(args) ;
    }


    // delcommand
    SyscmdDelcommand::SyscmdDelcommand()
    : BindedFuncCreator("system_command_delcommand")
    {}
    void SyscmdDelcommand::sprocess(const std::string& args) {
        if(args.empty()) {
            // does not have argument is empty
            VirtualCmdLine::msgout("E: Invalid argument") ;
            return ;
        }

        auto arg = rcparser::extract_single_arg(args) ;
        if(arg.empty()) {
            VirtualCmdLine::msgout("E: Invalid argument") ;
            return ;
        }

        // mode unmap
    }
    void SyscmdDelcommand::sprocess(NTypeLogger&) {
    }
    void SyscmdDelcommand::sprocess(const CharLogger& parent_lgr) {
        auto str = parent_lgr.to_str() ;
        if(str.empty()) {
            throw RUNTIME_EXCEPT("Empty command") ;
        }

        auto [cmd, args] = rcparser::divide_cmd_and_args(str) ;
        sprocess(args) ;
    }

    // comclear
    SyscmdComclear::SyscmdComclear()
    : BindedFuncCreator("system_command_comclear")
    {}
    void SyscmdComclear::sprocess() {
        // mode clear
    }
    void SyscmdComclear::sprocess(NTypeLogger&) {
    }
    void SyscmdComclear::sprocess(const CharLogger& parent_lgr) {
        auto str = parent_lgr.to_str() ;
        if(str.empty()) {
            throw RUNTIME_EXCEPT("Empty command") ;
        }

        auto [cmd, args] = rcparser::divide_cmd_and_args(str) ;
        if(!args.empty()) {
            VirtualCmdLine::msgout("E: Invalid argument") ;
            return ;
        }

        sprocess() ;
    }
}
