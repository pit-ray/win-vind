#include "autocmd.hpp"

#include "core/autocmd.hpp"
#include "core/errlogger.hpp"
#include "core/rcparser.hpp"
#include "opt/vcmdline.hpp"
#include "util/debug.hpp"
#include "util/def.hpp"
#include "util/string.hpp"

#include <sstream>


namespace vind
{
    namespace bind
    {
        AutoCmdAdd::AutoCmdAdd()
        : BindedFuncVoid("autocmd_add")
        {}

        void AutoCmdAdd::sprocess(
                std::uint16_t UNUSED(count),
                const std::string& args) {
            auto [_, pargs] = core::divide_cmd_and_args(args) ;

            auto [event_str, patcmd] = core::extract_double_args(pargs) ;
            auto [pattern, cmd] = core::extract_double_args(patcmd) ;

            std::vector<core::AutoCmdEvent> events ;
            for(const auto& event_str_part : util::split(event_str, ",")) {
                auto event = core::get_autocmd_event(event_str_part) ;
                if(event == core::AutoCmdEvent::UNDEFINED) {
                    opt::VCmdLine::print(
                        opt::ErrorMessage("E: Not supported event name")) ;

                    core::Logger::get_instance().error(
                        event_str + " is an unsupported event name.") ;
                    return ;
                }

                events.push_back(event) ;
            }

            if(pattern.empty()) {
                opt::VCmdLine::print(
                    opt::ErrorMessage("E: Empty autocmd pattern")) ;

                core::Logger::get_instance().error(
                    "The autocmd pattern must be specified.") ;

                return ;
            }
            auto patterns = util::split(pattern, ",") ;

            if(cmd.empty()) {
                opt::VCmdLine::print(
                    opt::ErrorMessage("E: Empty autocmd command")) ;

                core::Logger::get_instance().error(
                    "autocmd must specify the command to be executed.") ;

                return ;
            }

            for(const auto event : events) {
                for(const auto& aupat : patterns) {
                    core::AutoCmd::get_instance().add(event, aupat, cmd) ;
                }
            }
        }

        AutoCmdRemove::AutoCmdRemove()
        : BindedFuncVoid("autocmd_del")
        {}

        void AutoCmdRemove::sprocess(
                std::uint16_t UNUSED(count),
                const std::string& args) {
            auto& ac = core::AutoCmd::get_instance() ;

            auto [_, pargs] = core::divide_cmd_and_args(args) ;

            auto [event_str, patcmd] = core::extract_double_args(pargs) ;
            auto [pattern, cmd] = core::extract_double_args(patcmd) ;
            auto patterns = util::split(pattern, ",") ;

            if(event_str == "*") {
                if(patterns.empty()) {
                    opt::VCmdLine::print(
                        opt::ErrorMessage("E: Empty autocmd pattern")) ;
                    core::Logger::get_instance().error(
                        "autocmd patterns are required for events containing wildcards, such as *.") ;
                    return ;
                }

                for(const auto& aupat : patterns) {
                    ac.remove(aupat) ;
                }
                return ;
            }

            std::vector<core::AutoCmdEvent> events ;
            for(const auto& event_str_part : util::split(event_str, ",")) {
                auto event = core::get_autocmd_event(event_str_part) ;
                if(event == core::AutoCmdEvent::UNDEFINED) {
                    opt::VCmdLine::print(
                        opt::ErrorMessage("E: Not supported event name")) ;

                    core::Logger::get_instance().error(
                        event_str + " is an unsupported event name.") ;
                    return ;
                }
                events.push_back(event) ;
            }

            if(patterns.empty()) {
                for(const auto event : events) {
                    ac.remove(event) ;
                }
                return ;
            }
            if(cmd.empty()) {
                for(const auto event : events) {
                    for(const auto& aupat : patterns) {
                        ac.remove(event, aupat) ;
                    }
                }
                return ;
            }

            for(const auto event : events) {
                for(const auto& aupat : patterns) {
                    ac.remove(event, aupat) ;
                }
            }
            for(const auto event : events) {
                for(const auto& aupat : patterns) {
                    ac.add(event, aupat, cmd) ;
                }
            }
        }
    }
}
