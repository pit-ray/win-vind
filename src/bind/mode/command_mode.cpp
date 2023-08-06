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
#include "opt/optionlist.hpp"
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

        void clear() {
            print_logs_.clear() ;
            fetched_logs_.clear() ;
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
            int hist_idx_ ;

            Impl()
            : hists_(),
              bg_(opt::ref_global_options_bynames(
                    opt::AsyncUIACacheBuilder().name(),
                    opt::VCmdLine().name()
              )),
              hist_idx_(0)
            {}

            void update_cmdline_display() {
                auto& latest = hists_.back() ;
               opt::VCmdLine::print(opt::StaticMessage(":" + latest->to_string())) ;
            }

            void quit_loop() {
                opt::VCmdLine::reset() ;
                // Remove the latest history
                util::remove_from_back(hists_, 1) ;
            }

            bool backspace() {
                auto& latest = hists_.back() ;
                if(latest->empty()) {
                    // When the command line is `:` only and call it,
                    // exit the command mode.
                    quit_loop() ;
                    return false ;
                }
                latest->backward() ;
                update_cmdline_display() ;
                return true ;
            }

            void forward_history() {
                auto& latest = hists_.back() ;
                auto latest_idx = static_cast<int>(hists_.size() - 1) ;
                auto tmp = hist_idx_ + 1 ;
                if(tmp > latest_idx) {
                    return ;
                }

                hist_idx_ = tmp ;
                if(tmp == latest_idx) {
                    latest->clear() ;
                    update_cmdline_display() ;
                }
                else {
                    *latest = *hists_[hist_idx_] ;
                    update_cmdline_display() ;
                }
            }

            void backward_history() {
                if(hist_idx_ - 1 < 0) {
                    return ;
                }
                auto& latest = hists_.back() ;
                *latest = *hists_[--hist_idx_] ;
                update_cmdline_display() ;
            }

            auto decide() {
                opt::VCmdLine::reset() ;

                auto& ihub = core::InputHub::get_instance() ;
                auto solver = ihub.get_solver(core::Mode::COMMAND) ;
                solver->reset_state() ;

                auto& latest = hists_.back() ;

                std::string args ;
                std::vector<core::CmdUnit::SPtr> command ;
                for(std::size_t i = 0 ; i < latest->size() ; i ++) {
                    args += latest->print_logs_[i] ;
                    for(auto& unit : latest->fetched_logs_[i]) {
                        auto tmp = solver->map_command_from(*unit, false) ;
                        if(!tmp.empty()) {
                            // To get the last matched command
                            command = tmp ;
                        }
                    }
                }

                auto result = SystemCall::SUCCEEDED ;
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
                return result ;
            }

            void write_as_printable(const core::CmdUnit::SPtr& input) {
                // Aggregate the printable keys into this vector from input and outputs.
                std::vector<core::CmdUnit::SPtr> printables ;

                auto& ihub = core::InputHub::get_instance() ;
                auto solver = ihub.get_solver(core::Mode::COMMAND) ;
                auto outputs = solver->map_command_from(*input, true) ;
                for(auto& unit : outputs) {
                    if(!unit->empty()) {
                        printables.push_back(unit) ;
                    }
                }

                // If the input is mapped to another printable keys,
                // then the original input is ignored.
                if(printables.empty()) {
                    printables.push_back(input) ;
                }

                // Ignore unprintable inputs like <shift> or <alt>.
                auto printable_ustring = encode_to_unicode(printables) ;
                if(!printable_ustring.empty()) {
                    auto& latest = hists_.back() ;
                    latest->print_logs_.push_back(printable_ustring) ;
                    latest->fetched_logs_.push_back(printables) ;
                    update_cmdline_display() ;
                }
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
            auto& igate = core::InputGate::get_instance() ;
            auto& ihub = core::InputHub::get_instance() ;

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

            // Reset the index of histories to refer the inputted logs.
            pimpl->hists_.push_back(std::make_shared<CmdPoint>()) ;
            pimpl->hist_idx_ = static_cast<int>(pimpl->hists_.size() - 1) ;

            auto result = SystemCall::SUCCEEDED ;
            while(true) {
                pimpl->bg_.update() ;

                bool break_flag = false ;
                do {
                    core::CmdUnit::SPtr input ;
                    std::uint16_t count ;
                    if(!ihub.fetch_input(input, count, core::Mode::COMMAND, false)) {
                        continue ;
                    }

                    if(input->is_containing(KEYCODE_ESC)) {
                        pimpl->quit_loop() ;
                        break_flag = true ;
                        break ;
                    }
                    if(input->is_containing(KEYCODE_BKSPACE)) {
                        if(!pimpl->backspace()) {
                            break_flag = true ;
                        }
                        break ;
                    }
                    if(input->is_containing(KEYCODE_UP)) {
                        pimpl->backward_history() ;
                        continue ;
                    }
                    if(input->is_containing(KEYCODE_DOWN)) {
                        pimpl->forward_history() ;
                        continue ;
                    }
                    if(input->is_containing(KEYCODE_ENTER)) {
                        result = pimpl->decide() ;
                        break_flag = true ;
                        break ;
                    }
                    pimpl->write_as_printable(input) ;
                } while(!ihub.is_empty_queue()) ;

                if(break_flag) {
                    break ;
                }
            }
            igate.release_virtually(KEYCODE_ESC) ;
            igate.release_virtually(KEYCODE_ENTER) ;

            return result ;
        }
    }
}
