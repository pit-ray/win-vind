#include "map.hpp"

#include "bind/bindedfunc.hpp"
#include "bind/bindinglist.hpp"
#include "core/charlogger.hpp"
#include "core/entry.hpp"
#include "core/errlogger.hpp"
#include "core/maptable.hpp"
#include "core/mode.hpp"
#include "core/rcparser.hpp"
#include "opt/vcmdline.hpp"
#include "util/debug.hpp"
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
                std::uint16_t count, const std::string& args) {
            auto mode = core::Mode::GUI_NORMAL ;

            std::string arg1, arg2 ;
            if(parse_argument_as_map(args, arg1, arg2)) {
                core::MapTable::get_instance().add_map(arg1, arg2, mode) ;
            }

            return SystemCall::NOTHING ;
        }

        // noremap
        SyscmdNoremap::SyscmdNoremap()
        : BindedFuncFlex("system_command_noremap")
        {}
        SystemCall SyscmdNoremap::sprocess(
                std::uint16_t count, const std::string& args) {
            auto mode = core::Mode::GUI_NORMAL ;

            std::string arg1, arg2 ;
            if(parse_argument_as_map(args, arg1, arg2)) {
                /*
                if(map.target_command().size() > 6 && \
                        !bind::ref_global_funcs_bynames(map.target_command_string())) {
                    core::Logger::get_instance().warning(
                            "{" + \
                            mode_to_prefix(mode) + "noremap " + \
                            map.trigger_command_string() + " " + map.target_command_string() + \
                            "} is not a function ID, so considered as a keystroke to be generated.") ;
                }
                */

                auto& mtable = core::MapTable::get_instance() ;
                mtable.add_noremap(arg1, arg2, mode) ;
            }
            return SystemCall::NOTHING ;
        }

        // unmap
        SyscmdUnmap::SyscmdUnmap()
        : BindedFuncFlex("system_command_unmap")
        {}

        SystemCall SyscmdUnmap::sprocess(
                std::uint16_t count, const std::string& args) {
            auto mode = core::Mode::GUI_NORMAL ;

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
                    core::MapTable::get_instance().remove(arg, mode) ;
                }
            }
            return SystemCall::NOTHING ;
        }

        // mapclear
        SyscmdMapclear::SyscmdMapclear()
        : BindedFuncFlex("system_command_mapclear")
        {}

        SystemCall SyscmdMapclear::sprocess(
                std::uint16_t count, const std::string& args) {
            auto mode = core::Mode::GUI_NORMAL ;
            core::MapTable::get_instance().clear(mode) ;
            return SystemCall::NOTHING ;
        }
    }
}
