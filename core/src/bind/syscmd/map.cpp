#include "bind/syscmd/map.hpp"

#include "bind/binded_func.hpp"
#include "bind/binded_func_creator.hpp"
#include "err_logger.hpp"
#include "key/char_logger.hpp"
#include "mode.hpp"
#include "opt/virtual_cmd_line.hpp"
#include "parser/rc_parser.hpp"
#include "util/def.hpp"

#include <string>
#include <unordered_map>


namespace vind
{
    // noremap
    SyscmdNoremap::SyscmdNoremap()
    : BindedFuncCreator("system_command_noremap")
    {}
    void SyscmdNoremap::sprocess(const mode::Mode mode, const std::string& args) {
        if(args.empty()) {
            VirtualCmdLine::msgout("E: Not support list of map yet") ;
            return ;
        }

        auto [arg1, arg2] = rcparser::extract_double_args(args) ;
        if(arg1.empty()) {
            VirtualCmdLine::msgout("E: Not support list of map yet") ;
            return ;
        }
        if(arg2.empty()) {
            VirtualCmdLine::msgout("E: Not support reference of map yet") ;
            return ;
        }

        std::cout << "map " << mode::mode_name(mode) << ": " << arg1 << " ==> " << arg2 << std::endl ;

        // mode, src_cmd, target
    }
    void SyscmdNoremap::sprocess(NTypeLogger&) {
        return ;
    }
    void SyscmdNoremap::sprocess(const CharLogger& parent_lgr) {
        auto str = parent_lgr.to_str() ;
        if(str.empty()) {
            throw RUNTIME_EXCEPT("Empty command") ;
        }

        auto [cmd, args] = rcparser::divide_cmd_and_args(str) ;
        auto [prefix, _] = rcparser::divide_prefix_and_cmd(cmd, "n") ;

        auto mode = rcparser::parse_mode_prefix(prefix) ;
        if(mode == mode::Mode::None) {
            PRINT_ERROR(str + " is Invalid mode prefix.") ;
            VirtualCmdLine::msgout("E: Unsupported mode prefix") ;
            return ;
        }

        sprocess(mode, args) ;
    }


    // unmap
    SyscmdUnmap::SyscmdUnmap()
    : BindedFuncCreator("system_command_unmap")
    {}

    void SyscmdUnmap::sprocess(mode::Mode mode, const std::string& args) {
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

        std::cout << "unmap " << mode::mode_name(mode) << ": " << arg << std::endl ;
        // mode unmap
    }
    void SyscmdUnmap::sprocess(NTypeLogger&) {
    }
    void SyscmdUnmap::sprocess(const CharLogger& parent_lgr) {
        auto str = parent_lgr.to_str() ;
        if(str.empty()) {
            throw RUNTIME_EXCEPT("Empty command") ;
        }

        auto [cmd, args] = rcparser::divide_cmd_and_args(str) ;
        auto [prefix, _] = rcparser::divide_prefix_and_cmd(cmd, "u") ;

        auto mode = rcparser::parse_mode_prefix(prefix) ;
        if(mode == mode::Mode::None) {
            PRINT_ERROR(str + " is Invalid mode prefix.") ;
            VirtualCmdLine::msgout("E: Unsupported mode prefix") ;
            return ;
        }

        sprocess(mode, args) ;
    }

    // mapclear
    SyscmdMapclear::SyscmdMapclear()
    : BindedFuncCreator("system_command_mapclear")
    {}

    void SyscmdMapclear::sprocess(mode::Mode mode) {
        std::cout << "clear " << mode::mode_name(mode) << std::endl ;
        // mode clear
    }
    void SyscmdMapclear::sprocess(NTypeLogger&) {
    }
    void SyscmdMapclear::sprocess(const CharLogger& parent_lgr) {
        auto str = parent_lgr.to_str() ;
        if(str.empty()) {
            throw RUNTIME_EXCEPT("Empty command") ;
        }

        auto [cmd, args] = rcparser::divide_cmd_and_args(str) ;
        if(!args.empty()) {
            VirtualCmdLine::msgout("E: Invalid argument") ;
            return ;
        }

        auto [prefix, _] = rcparser::divide_prefix_and_cmd(cmd, "u") ;
        auto mode = rcparser::parse_mode_prefix(prefix) ;
        if(mode == mode::Mode::None) {
            PRINT_ERROR(str + " is Invalid mode prefix.") ;
            VirtualCmdLine::msgout("E: Unsupported mode prefix") ;
            return ;
        }

        sprocess(mode) ;
    }
}
