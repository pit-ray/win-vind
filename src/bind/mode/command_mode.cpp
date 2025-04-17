#include "command_mode.hpp"

#include "bind/bindedfunc.hpp"
#include "core/autocmd.hpp"
#include "core/background.hpp"
#include "core/cmdunit.hpp"
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


#include <algorithm>
#include <memory>
#include <set>
#include <sstream>
#include <windows.h>

#include <vector>


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

            std::vector<std::vector<core::CmdUnit::SPtr>> candidates_ ;
            std::vector<std::vector<std::string>> print_candidates_ ;
            int candidate_idx_ ;

            Impl()
            : hists_(),
              bg_(opt::search_options(
                    opt::AsyncUIACacheBuilder().name(),
                    opt::VCmdLine().name()
              )),
              hist_idx_(0),
              candidates_(),
              print_candidates_(),
              candidate_idx_(0)
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
                            command = std::move(tmp) ;
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

            void complement_command() {
                if(candidates_.empty()) { // find candidates
                    auto& ihub = core::InputHub::get_instance() ;
                    auto solver = ihub.get_solver(core::Mode::COMMAND) ;

                    // Load the current command
                    auto& latest = hists_.back() ;

                    // Match with matchers
                    solver->reset_state() ;
                    for(std::size_t i = 0 ; i < latest->size() ; i ++) {
                        for(auto& unit : latest->fetched_logs_[i]) {
                            solver->map_command_from(*unit, false) ;
                        }
                    }

                    std::set<std::string> candidate_set ;
                    auto matchers = solver->get_trigger_matchers() ;
                    for(auto matcher : matchers) {
                        if(!matcher->is_rejected()) {
                            const auto& trigger_cmd = matcher->get_command() ;
                            std::vector<core::CmdUnit::SPtr> cmd_no_args ;
                            std::vector<std::string> print_cmd_no_args ;

                            for(const auto& unit : trigger_cmd) {
                                bool break_flag = false ;
                                std::vector<core::CmdUnit::SPtr> cmd ;
                                std::stringstream ss ;
                                for(const auto& key : *unit) {
                                    // For simplicity, ignore a command with arguments such as `:! <any>`.
                                    // It just considers the pure command like `:!`.
                                    auto uni = core::keycode_to_unicode(key, *unit) ;
                                    if(uni.empty() || uni == " ") {
                                        break_flag = true ;
                                        break ;
                                    }
                                    ss << uni ;
                                }
                                if(break_flag) {
                                    break ;
                                }
                                cmd_no_args.push_back(unit) ;
                                print_cmd_no_args.push_back(ss.str()) ;
                            }

                            std::string cmd_str ;
                            for(const auto& s : print_cmd_no_args) {
                                cmd_str += s ;
                            }

                            if(candidate_set.find(cmd_str) == candidate_set.end()) {  // Avoid the duplicated candidates.
                                candidates_.push_back(std::move(cmd_no_args)) ;
                                print_candidates_.push_back(std::move(print_cmd_no_args)) ;
                                candidate_set.insert(std::move(cmd_str)) ;
                            }
                        }
                    }

                    // Show the candidates in the virtual command line.
                    if(!candidates_.empty()) {
                        std::stringstream ss ;
                        for(const auto& s : candidate_set) {
                            ss << ":" << s << " " ;
                        }
                        opt::VCmdLine::print(opt::StaticMessage(ss.str())) ;
                    }
                    candidate_idx_ = -1 ;

                    return ;  // At the first time, show a list of candidates without selection.
                }

                // Rotate the candidates
                candidate_idx_ = (candidate_idx_ + 1) % candidates_.size() ;

                // Overwrite the latest command line with the candidate.
                auto& latest = hists_.back() ;
                latest->fetched_logs_.clear() ;
                for(const auto& unit : candidates_[candidate_idx_]) {
                    latest->fetched_logs_.push_back({unit}) ;
                }
                latest->print_logs_ = print_candidates_[candidate_idx_] ;
                update_cmdline_display() ;
            }

            void clear_candidates() {
                candidates_.clear() ;
                print_candidates_.clear() ;
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
                    latest->print_logs_.push_back(std::move(printable_ustring)) ;
                    latest->fetched_logs_.push_back(std::move(printables)) ;
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
            auto& ac = core::AutoCmd::get_instance() ;

            auto return_mode = [] (core::Mode* m) {
                // If the mode is changed, then do nothing.
                if(core::get_global_mode() == core::Mode::COMMAND) {
                    core::set_global_mode(*m) ;
                    core::AutoCmd::get_instance().apply(
                        core::get_leave_event(core::Mode::COMMAND)) ;
                }
            } ;
            auto m = core::get_global_mode() ;
            ac.apply(core::get_leave_event(m)) ;

            std::unique_ptr<core::Mode, decltype(return_mode)>
                mode_preserver(new core::Mode(m), return_mode) ;

            core::set_global_mode(core::Mode::COMMAND) ;
            core::InstantKeyAbsorber ika ;

            opt::VCmdLine::reset() ;
            opt::VCmdLine::print(opt::StaticMessage(":")) ;

            // Reset the index of histories to refer the inputted logs.
            pimpl->hists_.push_back(std::make_shared<CmdPoint>()) ;
            pimpl->hist_idx_ = static_cast<int>(pimpl->hists_.size() - 1) ;

            auto result = SystemCall::SUCCEEDED ;
            ac.apply(core::get_enter_event(core::Mode::COMMAND)) ;
            while(true) {
                if(!pimpl->bg_.update()) {
                    continue ;
                }

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
                    if(input->is_containing(KEYCODE_TAB)) {
                        pimpl->complement_command() ;
                        continue ;
                    }
                    if(input->is_containing(KEYCODE_ENTER)) {
                        result = pimpl->decide() ;
                        break_flag = true ;
                        break ;
                    }

                    if(!pimpl->candidates_.empty()) {
                        pimpl->clear_candidates() ;
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
