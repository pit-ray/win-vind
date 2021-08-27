#include "bind/syscmd/map.hpp"

#include "bind/binded_func.hpp"
#include "bind/binded_func_creator.hpp"
#include "entry.hpp"
#include "err_logger.hpp"
#include "g_maps.hpp"
#include "key/char_logger.hpp"
#include "mode.hpp"
#include "opt/vcmdline.hpp"
#include "parser/rc_parser.hpp"
#include "util/def.hpp"

#include <string>
#include <unordered_map>

namespace
{
    using namespace vind ;

    template <typename T>
    inline bool parse_argument_as_map(
            T&& args,
            std::string& return_arg1,
            std::string& return_arg2) {
        if(args.empty()) {
            VCmdLine::print(ErrorMessage("E: Not support list of map yet")) ;
            return false ;
        }

        auto [a1, a2] = rcparser::extract_double_args(std::forward<T>(args)) ;
        if(a1.empty()) {
            VCmdLine::print(ErrorMessage("E: Not support list of map yet")) ;
            return false ;
        }
        if(a2.empty()) {
            VCmdLine::print(ErrorMessage("E: Not support reference of map yet")) ;
            return false ;
        }

        return_arg1 = std::move(a1) ;
        return_arg2 = std::move(a2) ;
        return true ;
    }


    template <typename T>
    inline bool parse_arguments_from_logger(
            const CharLogger& lgr,
            T&& after_prefix,
            mode::Mode& return_mode,
            std::string& return_args) {
        auto str = lgr.to_str() ;
        if(str.empty()) {
            throw RUNTIME_EXCEPT("Empty command") ;
        }

        auto [cmd, args] = rcparser::divide_cmd_and_args(str) ;
        auto [prefix, _] = rcparser::divide_prefix_and_cmd(cmd, after_prefix) ;

        auto mode = mode::parse_prefix(prefix) ;
        if(mode == mode::Mode::UNDEFINED) {
            PRINT_ERROR(str + " is Invalid mode prefix.") ;
            VCmdLine::print(ErrorMessage("E: Unsupported mode prefix")) ;
            return false ;
        }

        return_mode = mode ;
        return_args = std::move(args) ;
        return true ;
    }
}


namespace vind
{
    // map
    SyscmdMap::SyscmdMap()
    : BindedFuncCreator("system_command_map")
    {}
    void SyscmdMap::sprocess(
            const mode::Mode mode,
            const std::string& args,
            bool reload_config) {

        std::string arg1, arg2 ;
        if(parse_argument_as_map(args, arg1, arg2)) {
            gmaps::map(arg1, arg2, mode) ;
            if(reload_config) {
                vind::reconstruct_all_components() ;
            }
        }
    }
    void SyscmdMap::sprocess(NTypeLogger&) {
        return ;
    }
    void SyscmdMap::sprocess(const CharLogger& parent_lgr) {
        try {
            mode::Mode mode ;
            std::string args ;
            if(parse_arguments_from_logger(parent_lgr, "m", mode, args)) {
                sprocess(mode, args, true) ;
            }
        }
        // If received syntax error as std::logic_error,
        // convert to runtime_error not to terminate application.
        catch(const std::logic_error& e) {
            throw std::runtime_error(e.what()) ;
        }
    }


    // noremap
    SyscmdNoremap::SyscmdNoremap()
    : BindedFuncCreator("system_command_noremap")
    {}
    void SyscmdNoremap::sprocess(
            const mode::Mode mode,
            const std::string& args,
            bool reload_config) {

        std::string arg1, arg2 ;
        if(parse_argument_as_map(args, arg1, arg2)) {
            gmaps::noremap(arg1, arg2, mode) ;
            if(reload_config) {
                vind::reconstruct_all_components() ;
            }
        }
    }
    void SyscmdNoremap::sprocess(NTypeLogger&) {
        return ;
    }
    void SyscmdNoremap::sprocess(const CharLogger& parent_lgr) {
        try {
            mode::Mode mode ;
            std::string args ;
            if(parse_arguments_from_logger(parent_lgr, "n", mode, args)) {
                sprocess(mode, args, true) ;
            }
        }
        // If received syntax error as std::logic_error,
        // convert to runtime_error not to terminate application.
        catch(const std::logic_error& e) {
            throw std::runtime_error(e.what()) ;
        }
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
            VCmdLine::print(ErrorMessage("E: Invalid argument")) ;
            return ;
        }

        auto arg = rcparser::extract_single_arg(args) ;
        if(arg.empty()) {
            VCmdLine::print(ErrorMessage("E: Invalid argument")) ;
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

        auto mode = mode::parse_prefix(prefix) ;
        if(mode == mode::Mode::UNDEFINED) {
            PRINT_ERROR(str + " is Invalid mode prefix.") ;
            VCmdLine::print(ErrorMessage("E: Unsupported mode prefix")) ;
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
            VCmdLine::print(ErrorMessage("E: Invalid argument")) ;
            return ;
        }

        auto [prefix, _] = rcparser::divide_prefix_and_cmd(cmd, "m") ;
        auto mode = mode::parse_prefix(prefix) ;
        if(mode == mode::Mode::UNDEFINED) {
            PRINT_ERROR(str + " is Invalid mode prefix.") ;
            VCmdLine::print(ErrorMessage("E: Unsupported mode prefix")) ;
            return ;
        }
        sprocess(mode, true) ;
    }
}
