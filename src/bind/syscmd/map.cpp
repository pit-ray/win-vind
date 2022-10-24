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

    std::vector<core::Mode> prefix_to_modes(const std::string& prefix) {
        using core::Mode ;
        if(prefix.empty()) {
            return {
                Mode::GUI_NORMAL,
                Mode::GUI_VISUAL,
                Mode::EDI_NORMAL,
                Mode::EDI_VISUAL,
            } ;
        }
        if(prefix == "g") {
            return {
                Mode::GUI_NORMAL,
                Mode::GUI_VISUAL,
            } ;
        }
        if(prefix == "e") {
            return {
                Mode::EDI_NORMAL,
                Mode::EDI_VISUAL,
            } ;
        }
        if(prefix == "n") {
            return {
                Mode::GUI_NORMAL,
                Mode::EDI_NORMAL,
            } ;
        }

        if(prefix == "v") {
            return {
                Mode::GUI_VISUAL,
                Mode::EDI_VISUAL,
            } ;
        }

        auto mode = core::parse_mode_prefix(prefix) ;
        if(mode == core::Mode::UNDEFINED) {
            PRINT_ERROR(prefix + " is invalid mode prefix.") ;
            opt::VCmdLine::print(
                opt::ErrorMessage("E: Unsupported mode prefix")) ;
            return {} ;
        }
        return {mode} ;
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
                std::uint16_t UNUSED(count),
                const std::string& args,
                const std::string& prefix) {
            if(args.empty()) {
                opt::VCmdLine::print(
                    opt::ErrorMessage("E: Not support list of map yet")) ;
                return SystemCall::NOTHING ;
            }

            auto [a1, a2] = core::extract_double_args(args) ;
            if(a1.empty()) {
                opt::VCmdLine::print(
                    opt::ErrorMessage("E: Not support list of map yet")) ;
                return SystemCall::NOTHING ;
            }
            if(a2.empty()) {
                opt::VCmdLine::print(
                    opt::ErrorMessage("E: Not support reference of map yet")) ;
                return SystemCall::NOTHING ;
            }

            auto& mtable = core::MapTable::get_instance() ;
            auto modes = prefix_to_modes(prefix) ;
            for(auto mode : modes) {
                mtable.add_map(a1, a2, mode) ;
            }
            return SystemCall::RECONSTRUCT ;
        }

        // noremap
        SyscmdNoremap::SyscmdNoremap()
        : BindedFuncFlex("system_command_noremap")
        {}
        SystemCall SyscmdNoremap::sprocess(
                std::uint16_t UNUSED(count),
                const std::string& args,
                const std::string& prefix) {
            if(args.empty()) {
                opt::VCmdLine::print(
                    opt::ErrorMessage("E: Not support list of map yet")) ;
                return SystemCall::NOTHING ;
            }

            auto [a1, a2] = core::extract_double_args(args) ;
            if(a1.empty()) {
                opt::VCmdLine::print(
                    opt::ErrorMessage("E: Not support list of map yet")) ;
                return SystemCall::NOTHING ;
            }
            if(a2.empty()) {
                opt::VCmdLine::print(
                    opt::ErrorMessage("E: Not support reference of map yet")) ;
                return SystemCall::NOTHING ;
            }

            auto& mtable = core::MapTable::get_instance() ;
            auto modes = prefix_to_modes(prefix) ;
            for(auto mode : modes) {
                mtable.add_noremap(a1, a2, mode) ;
            }
            return SystemCall::RECONSTRUCT ;
        }

        // unmap
        SyscmdUnmap::SyscmdUnmap()
        : BindedFuncFlex("system_command_unmap")
        {}
        SystemCall SyscmdUnmap::sprocess(
                std::uint16_t UNUSED(count),
                const std::string& args,
                const std::string& prefix) {
            if(args.empty()) {
                // does not have argument is empty
                opt::VCmdLine::print(opt::ErrorMessage("E: Invalid argument")) ;
                return SystemCall::NOTHING ;
            }

            auto arg = core::extract_single_arg(args) ;
            if(arg.empty()) {
                opt::VCmdLine::print(opt::ErrorMessage("E: Invalid argument")) ;
                return SystemCall::NOTHING ;
            }

            auto modes = prefix_to_modes(prefix) ;
            for(auto mode : modes) {
                core::MapTable::get_instance().remove(arg, mode) ;
            }
            return SystemCall::RECONSTRUCT ;
        }

        // mapclear
        SyscmdMapclear::SyscmdMapclear()
        : BindedFuncFlex("system_command_mapclear")
        {}
        SystemCall SyscmdMapclear::sprocess(
                std::uint16_t UNUSED(count),
                const std::string& UNUSED(args),
                const std::string& prefix) {
            auto modes = prefix_to_modes(prefix) ;
            for(auto mode : modes) {
                core::MapTable::get_instance().clear(mode) ;
            }
            return SystemCall::RECONSTRUCT ;
        }
    }
}
