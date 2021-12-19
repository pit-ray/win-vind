#include "map.hpp"

#include "bind/binded_func.hpp"
#include "bind/binded_func.hpp"
#include "core/char_logger.hpp"
#include "core/entry.hpp"
#include "core/err_logger.hpp"
#include "core/maptable.hpp"
#include "core/mode.hpp"
#include "core/rc_parser.hpp"
#include "opt/vcmdline.hpp"
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
            opt::VCmdLine::print(opt::ErrorMessage("E: Not support list of map yet")) ;
            return false ;
        }

        auto [a1, a2] = core::extract_double_args(std::forward<T>(args)) ;
        if(a1.empty()) {
            opt::VCmdLine::print(opt::ErrorMessage("E: Not support list of map yet")) ;
            return false ;
        }
        if(a2.empty()) {
            opt::VCmdLine::print(opt::ErrorMessage("E: Not support reference of map yet")) ;
            return false ;
        }

        return_arg1 = std::move(a1) ;
        return_arg2 = std::move(a2) ;
        return true ;
    }


    template <typename T>
    inline bool parse_arguments_from_logger(
            const core::CharLogger& lgr,
            T&& after_prefix,
            core::Mode& return_mode,
            std::string& return_args) {
        auto str = lgr.to_str() ;
        if(str.empty()) {
            throw RUNTIME_EXCEPT("Empty command") ;
        }

        auto [cmd, args] = core::divide_cmd_and_args(str) ;
        auto [prefix, _] = core::divide_prefix_and_cmd(cmd, after_prefix) ;

        auto mode = core::parse_mode_prefix(prefix) ;
        if(mode == core::Mode::UNDEFINED) {
            PRINT_ERROR(str + " is Invalid mode prefix.") ;
            opt::VCmdLine::print(opt::ErrorMessage("E: Unsupported mode prefix")) ;
            return false ;
        }

        return_mode = mode ;
        return_args = std::move(args) ;
        return true ;
    }
}

namespace vind
{
    namespace bind
    {
        // map
        SyscmdMap::SyscmdMap()
        : BindedFuncFlex("system_command_map")
        {}
        SystemCall SyscmdMap::sprocess(
                const core::Mode mode,
                const std::string& args) {

            std::string arg1, arg2 ;
            if(parse_argument_as_map(args, arg1, arg2)) {
                auto& maptable = core::MapTable::get_instance() ;
                maptable.add_map(arg1, arg2, mode) ;
            }

            return SystemCall::NOTHING ;
        }
        SystemCall SyscmdMap::sprocess(core::NTypeLogger&) {
            return SystemCall::NOTHING ;
        }
        SystemCall SyscmdMap::sprocess(const core::CharLogger& parent_lgr) {
            try {
                core::Mode mode ;
                std::string args ;
                if(parse_arguments_from_logger(parent_lgr, "m", mode, args)) {
                    sprocess(mode, args) ;
                    return SystemCall::RECONSTRUCT ;
                }
            }
            // If received syntax error as std::logic_error,
            // convert to runtime_error not to terminate application.
            catch(const std::logic_error& e) {
                throw std::runtime_error(e.what()) ;
            }

            return SystemCall::NOTHING ;
        }


        // noremap
        SyscmdNoremap::SyscmdNoremap()
        : BindedFuncFlex("system_command_noremap")
        {}
        SystemCall SyscmdNoremap::sprocess(
                const core::Mode mode,
                const std::string& args) {
            std::string arg1, arg2 ;
            if(parse_argument_as_map(args, arg1, arg2)) {
                auto& maptable = core::MapTable::get_instance() ;
                maptable.add_noremap(arg1, arg2, mode) ;
            }
            return SystemCall::NOTHING ;
        }
        SystemCall SyscmdNoremap::sprocess(core::NTypeLogger&) {
            return SystemCall::NOTHING ;
        }
        SystemCall SyscmdNoremap::sprocess(const core::CharLogger& parent_lgr) {
            try {
                core::Mode mode ;
                std::string args ;
                if(parse_arguments_from_logger(parent_lgr, "n", mode, args)) {
                    sprocess(mode, args) ;
                    return SystemCall::RECONSTRUCT ;
                }
            }
            // If received syntax error as std::logic_error,
            // convert to runtime_error not to terminate application.
            catch(const std::logic_error& e) {
                throw std::runtime_error(e.what()) ;
            }

            return SystemCall::NOTHING ;
        }


        // unmap
        SyscmdUnmap::SyscmdUnmap()
        : BindedFuncFlex("system_command_unmap")
        {}

        SystemCall SyscmdUnmap::sprocess(
                core::Mode mode,
                const std::string& args) {
            if(args.empty()) {
                // does not have argument is empty
                opt::VCmdLine::print(opt::ErrorMessage("E: Invalid argument")) ;
            }
            else {
                auto arg = core::extract_single_arg(args) ;
                if(arg.empty()) {
                    opt::VCmdLine::print(opt::ErrorMessage("E: Invalid argument")) ;
                }
                else {
                    auto& maptable = core::MapTable::get_instance() ;
                    maptable.remove(arg, mode) ;
                }
            }
            return SystemCall::NOTHING ;
        }
        SystemCall SyscmdUnmap::sprocess(core::NTypeLogger&) {
            return SystemCall::NOTHING ;
        }
        SystemCall SyscmdUnmap::sprocess(const core::CharLogger& parent_lgr) {
            auto str = parent_lgr.to_str() ;
            if(str.empty()) {
                throw RUNTIME_EXCEPT("Empty command") ;
            }

            auto [cmd, args] = core::divide_cmd_and_args(str) ;
            auto [prefix, _] = core::divide_prefix_and_cmd(cmd, "u") ;

            auto mode = core::parse_mode_prefix(prefix) ;
            if(mode == core::Mode::UNDEFINED) {
                PRINT_ERROR(str + " is Invalid mode prefix.") ;
                opt::VCmdLine::print(opt::ErrorMessage("E: Unsupported mode prefix")) ;
                return SystemCall::NOTHING ;
            }

            sprocess(mode, args) ;
            return SystemCall::RECONSTRUCT ;
        }

        // mapclear
        SyscmdMapclear::SyscmdMapclear()
        : BindedFuncFlex("system_command_mapclear")
        {}

        SystemCall SyscmdMapclear::sprocess(core::Mode mode) {
            auto& maptable = core::MapTable::get_instance() ;
            maptable.clear(mode) ;
            return SystemCall::NOTHING ;
        }
        SystemCall SyscmdMapclear::sprocess(core::NTypeLogger&) {
            return SystemCall::NOTHING ;
        }
        SystemCall SyscmdMapclear::sprocess(const core::CharLogger& parent_lgr) {
            auto str = parent_lgr.to_str() ;
            if(str.empty()) {
                throw RUNTIME_EXCEPT("Empty command") ;
            }

            auto [cmd, args] = core::divide_cmd_and_args(str) ;
            if(!args.empty()) {
                opt::VCmdLine::print(opt::ErrorMessage("E: Invalid argument")) ;
            }
            else {
                auto [prefix, _] = core::divide_prefix_and_cmd(cmd, "m") ;
                auto mode = core::parse_mode_prefix(prefix) ;
                if(mode == core::Mode::UNDEFINED) {
                    PRINT_ERROR(str + " is Invalid mode prefix.") ;
                    opt::VCmdLine::print(opt::ErrorMessage("E: Unsupported mode prefix")) ;
                }
                else {
                    sprocess(mode) ;
                    return SystemCall::RECONSTRUCT ;
                }
            }

            return SystemCall::NOTHING ;
        }
    }
}
