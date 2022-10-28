#include "map.hpp"

#include "bind/bindedfunc.hpp"
#include "bind/bindinglist.hpp"
#include "core/charlogger.hpp"
#include "core/entry.hpp"
#include "core/errlogger.hpp"
#include "core/inputhub.hpp"
#include "core/mode.hpp"
#include "core/rcparser.hpp"
#include "opt/vcmdline.hpp"
#include "util/debug.hpp"
#include "util/def.hpp"

#include <string>


namespace vind
{
    namespace bind
    {
        std::unordered_set<core::Mode> prefix_to_modes(const std::string& prefix) {
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

        SystemCall do_map(
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

            auto& ihub = core::InputHub::get_instance() ;
            for(auto mode : prefix_to_modes(prefix)) {
                ihub.add_map(a1, a2, mode) ;
            }
            return SystemCall::RECONSTRUCT ;
        }

        SystemCall do_noremap(
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

            auto& ihub = core::InputHub::get_instance() ;
            for(auto mode : prefix_to_modes(prefix)) {
                ihub.add_noremap(a1, a2, mode) ;
            }
            return SystemCall::RECONSTRUCT ;
        }

        SystemCall do_unmap(
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

            for(auto mode : prefix_to_modes(prefix)) {
                core::InputHub::get_instance().remove_mapping(arg, mode) ;
            }
            return SystemCall::RECONSTRUCT ;
        }

        SystemCall do_mapclear(const std::string& prefix) {
            for(auto mode : prefix_to_modes(prefix)) {
                core::InputHub::get_instance().clear_mapping(mode) ;
            }
            return SystemCall::RECONSTRUCT ;
        }
    }
}
