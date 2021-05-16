#include "bind/syscmd/map.hpp"

#include "bind/binded_func.hpp"
#include "bind/binded_func_creator.hpp"
#include "entry.hpp"
#include "err_logger.hpp"
#include "g_maps.hpp"
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
    void SyscmdNoremap::sprocess(
            const mode::Mode mode,
            const std::string& args,
            bool reload_config) {
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

        gmaps::map(arg1, arg2, mode) ;

        if(reload_config) {
            vind::reconstruct_all_components() ;
        }
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
        sprocess(mode, args, true) ;
    }


    // unmap
    SyscmdUnmap::SyscmdUnmap()
    : BindedFuncCreator("system_command_unmap")
    {}

    void SyscmdUnmap::sprocess(
            mode::Mode mode,
            const std::string& args,
            bool reload_config) {
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
        gmaps::unmap(arg, mode) ;

        if(reload_config) {
            vind::reconstruct_all_components() ;
        }
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
        sprocess(mode, args, true) ;
    }

    // mapclear
    SyscmdMapclear::SyscmdMapclear()
    : BindedFuncCreator("system_command_mapclear")
    {}

    void SyscmdMapclear::sprocess(
            mode::Mode mode,
            bool reload_config) {

        gmaps::mapclear(mode) ;
        if(reload_config) {
            vind::reconstruct_all_components() ;
        }
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

        auto [prefix, _] = rcparser::divide_prefix_and_cmd(cmd, "m") ;
        auto mode = rcparser::parse_mode_prefix(prefix) ;
        if(mode == mode::Mode::None) {
            PRINT_ERROR(str + " is Invalid mode prefix.") ;
            VirtualCmdLine::msgout("E: Unsupported mode prefix") ;
            return ;
        }
        sprocess(mode, true) ;
    }
}
