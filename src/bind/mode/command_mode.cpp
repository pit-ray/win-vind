#include "command_mode.hpp"

#include <algorithm>
#include <memory>
#include <sstream>
#include <windows.h>

#include <vector>

#include "bind/bindedfunc.hpp"
#include "core/background.hpp"
#include "core/errlogger.hpp"
#include "core/inputgate.hpp"
#include "core/inputhub.hpp"
#include "core/keycodedef.hpp"
#include "core/mapsolver.hpp"
#include "core/mode.hpp"
#include "core/settable.hpp"
#include "core/typeemu.hpp"
#include "opt/dedicate_to_window.hpp"
#include "opt/optionlist.hpp"
#include "opt/suppress_for_vim.hpp"
#include "opt/uiacachebuild.hpp"
#include "opt/vcmdline.hpp"
#include "util/container.hpp"
#include "util/debug.hpp"
#include "util/def.hpp"


namespace
{
    using namespace vind ;

    struct CmdPoint
    {
        std::vector<std::string> print_logs_{} ;
        std::vector<std::vector<core::CmdUnit::SPtr>> fetched_logs_{} ;

        void backward() {
            util::remove_from_back(print_logs_, 1) ;
            util::remove_from_back(fetched_logs_, 1) ;
        }

        std::string to_string() const {
            std::stringstream ss ;
            for(auto& cmd : print_logs_) {
                ss << cmd ;
            }
            return ss.str() ;
        }

        bool empty() const noexcept {
            return fetched_logs_.empty() ;
        }

        std::size_t size() const noexcept {
            return fetched_logs_.size() ;
        }
    } ;

    template <typename CmdType>
    std::string encode_to_unicode(CmdType&& cmd) {
        std::stringstream ss ;
        for(auto& unit : cmd) {
            for(auto& key : *unit) {
                auto uni = core::keycode_to_unicode(key, *unit) ;
                if(!uni.empty()) {
                    ss << uni ;
                }
            }
        }
        return ss.str() ;
    }
}

namespace vind
{
    namespace bind
    {
        struct ToCommand::Impl {
            std::vector<std::shared_ptr<CmdPoint>> hists_ ;
            core::Background bg_ ;
            core::TypingEmulator typeemu_ ;

            Impl()
            : hists_(),
              bg_(opt::ref_global_options_bynames(
                    opt::AsyncUIACacheBuilder().name(),
                    opt::Dedicate2Window().name(),
                    opt::SuppressForVim().name(),
                    opt::VCmdLine().name()
              )),
              typeemu_()
            {}

            template <typename UnitType, typename CmdType>
            std::vector<core::CmdUnit::SPtr> get_printables(
                    UnitType&& input, CmdType&& outputs) {
                std::vector<core::CmdUnit::SPtr> printables ;
                for(auto& unit : outputs) {
                    if(!unit->empty()) {
                        printables.push_back(unit) ;
                    }
                }
                if(printables.empty()) {
                    printables.push_back(input) ;
                }
                return printables ;
            }
        } ;

        ToCommand::ToCommand()
        : BindedFuncFlex("to_command"),
          pimpl(std::make_unique<Impl>())
        {}

        ToCommand::~ToCommand() noexcept = default ;
        ToCommand::ToCommand(ToCommand&&) = default ;
        ToCommand& ToCommand::operator=(ToCommand&&) = default ;

        SystemCall ToCommand::sprocess(
                std::uint16_t UNUSED(count),
                const std::string& UNUSED(args)) {
            using core::Mode ;

            auto& igate = core::InputGate::get_instance() ;
            auto solver = core::InputHub::get_instance().get_solver(Mode::COMMAND) ;

            auto return_mode = [] (core::Mode* m) {
                // If the mode is changed, then do nothing.
                if(core::get_global_mode() == core::Mode::COMMAND) {
                    core::set_global_mode(*m) ;
                }
            } ;
            std::unique_ptr<core::Mode, decltype(return_mode)>
                mode_preserver(new core::Mode(core::get_global_mode()), return_mode) ;

            core::set_global_mode(core::Mode::COMMAND) ;
            core::InstantKeyAbsorber ika ;

            opt::VCmdLine::reset() ;
            opt::VCmdLine::print(opt::StaticMessage(":")) ;

            auto hist = std::make_shared<CmdPoint>() ; 
            pimpl->hists_.push_back(hist) ;
            auto max_hist_idx = static_cast<int>(pimpl->hists_.size() - 1) ;
            auto hist_idx = max_hist_idx ;

            auto result = SystemCall::SUCCEEDED ;
            while(true) {
                pimpl->bg_.update() ;

                auto raw_input = igate.pressed_list() ;
                auto input = pimpl->typeemu_.lowlevel_to_typing(raw_input) ;
                if(!input) {
                    continue ;
                }

                auto outputs = solver->map_command_from(*input) ;
                auto contains = [&outputs, &input](auto key) {
                    for(auto& unit : outputs) {
                        if(unit->is_containing(key)) {
                            return true ;
                        }
                    }
                    return input->is_containing(key) ;
                } ;

                if(contains(KEYCODE_ESC)) {
                    opt::VCmdLine::reset() ;
                    util::remove_from_back(pimpl->hists_, 1) ;
                    break ;
                }
                if(contains(KEYCODE_BKSPACE)) {
                    if(hist->empty()) {
                        opt::VCmdLine::reset() ;
                        util::remove_from_back(pimpl->hists_, 1) ;
                        break ;
                    }
                    hist->backward() ;
                    opt::VCmdLine::print(opt::StaticMessage(":" + hist->to_string())) ;
                    continue ;
                }
                if(contains(KEYCODE_UP)) {
                    hist_idx = (std::max)(hist_idx - 1, 0) ;
                    *pimpl->hists_.back() = *pimpl->hists_[hist_idx] ;
                    opt::VCmdLine::print(opt::StaticMessage(":" + hist->to_string())) ;
                    continue ;
                }
                if(contains(KEYCODE_DOWN)) {
                    hist_idx = (std::min)(hist_idx + 1, max_hist_idx) ;
                    *pimpl->hists_.back() = *pimpl->hists_[hist_idx] ;
                    opt::VCmdLine::print(opt::StaticMessage(":" + hist->to_string())) ;
                    continue ;
                }
                if(contains(KEYCODE_ENTER)) {
                    opt::VCmdLine::reset() ;

                    solver->reset_state() ;
                    std::vector<core::CmdUnit::SPtr> command ;
                    std::string args ;
                    for(std::size_t i = 0 ; i < hist->size() ; i ++) {
                        args += hist->print_logs_[i] ;
                        for(auto& unit : hist->fetched_logs_[i]) {
                            auto tmp = solver->map_command_from(*unit, false) ;
                            if(!tmp.empty()) {
                                command = tmp ;
                            }
                        }
                    }

                    bool called = false ;
                    for(auto& unit : command) {
                        if(unit->empty()) {
                            called = true ;
                            result = unit->execute(1, args) ;
                        }
                    }
                    if(!called) {
                        opt::VCmdLine::print(opt::ErrorMessage("E: Not a command")) ;
                    }
                    break ;
                }

                auto printables = pimpl->get_printables(input, outputs) ;
                auto printable_ustring = encode_to_unicode(printables) ;
                // Ignore unprintable inputs like <shift> or <alt>.
                if(!printable_ustring.empty()) {
                    hist->print_logs_.push_back(printable_ustring) ;
                    hist->fetched_logs_.push_back(printables) ;

                    opt::VCmdLine::print(opt::StaticMessage(":" + hist->to_string())) ;
                }
            }
            igate.release_virtually(KEYCODE_ESC) ;
            igate.release_virtually(KEYCODE_ENTER) ;

            return result ;
        }
    }
}
