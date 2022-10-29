#include "inputhub.hpp"

#include "bind/mapdefault.hpp"
#include "cmdunit.hpp"
#include "inputgate.hpp"
#include "mapsolver.hpp"
#include "mode.hpp"
#include "syscalldef.hpp"
#include "typeemu.hpp"

#include "util/def.hpp"
#include "util/string.hpp"
#include "util/type_traits.hpp"

#include <algorithm>
#include <memory>


namespace
{
    using namespace vind ;
    std::string extract_numbers(
            const core::CmdUnit& input_unit,
            bool allow_zero=true) {
        std::vector<char> nums{} ;
        for(const auto& keycode : input_unit) {
            auto uni = core::keycode_to_unicode(keycode, input_unit) ;
            if(uni.empty()) {
                continue ;
            }
            auto ascii = uni.front() ;
            if(!allow_zero && ascii == '0') {
                continue ;
            }
            if('0' <= ascii && ascii <= '9') {
                nums.push_back(ascii) ;
            }
        }
        std::sort(nums.begin(), nums.end()) ;
        return std::string(nums.begin(), nums.end()) ;
    }
}

namespace vind
{
    namespace core
    {
        struct InputHub::Impl {
            ModeArray<std::shared_ptr<MapSolver>> solvers_{} ;
            TypingEmulator typeemu_{} ;
            std::string count_{} ;
        } ;

        InputHub::InputHub()
        : pimpl(std::make_unique<Impl>())
        {
            for(std::size_t m = 0 ; m < pimpl->solvers_.size() ; m ++) {
                auto solver = std::make_shared<MapSolver>(false) ;
                for(const auto& [trigger, target] : bind::get_default_map(m)) {
                    solver->add_default(trigger, target) ;
                }
                solver->deploy_default(true) ;
                solver->deploy(static_cast<Mode>(m)) ;
                pimpl->solvers_[m] = std::move(solver) ;
            }
        }

        InputHub::~InputHub() noexcept = default ;

        InputHub& InputHub::get_instance() {
            static InputHub instance{} ;
            return instance ;
        }

        std::shared_ptr<MapSolver> InputHub::get_solver(Mode mode) {
            return pimpl->solvers_[static_cast<int>(mode)] ;
        }

        void InputHub::fetch_inputs(
                CmdUnit::SPtr& fetched_input,
                std::vector<CmdUnit::SPtr>& fetched_outputs,
                std::vector<std::uint16_t>& fetched_counts,
                Mode mode,
                bool parse_count) {
            auto raw_input = InputGate::get_instance().pressed_list() ;
            auto input = pimpl->typeemu_.lowlevel_to_typing(raw_input) ;
            if(!input) {
                return ;
            }

            auto solver = get_solver(mode) ;

            if(parse_count && !solver->is_matching_any()) {
                // parses the sequence of head counts as the string
                // and converts them into a number.
                auto new_count = extract_numbers(
                        *input, pimpl->count_.size() > 0) ;
                if(!new_count.empty()) {
                    pimpl->count_ += new_count ;
                    return ;
                }
            }

            std::uint16_t count = 1 ;
            if(!pimpl->count_.empty()) {
                count = static_cast<std::uint16_t>(
                        util::extract_num(pimpl->count_)) ;
                pimpl->count_.clear() ;
            }

            fetched_input = input ;

            auto map_cmd = solver->map_command_from(*input) ;
            if(map_cmd.empty()) {
                return ;
            }

            // Update states of a solver using the solved mapping.
            for(const auto& unit : map_cmd) {
                if(!unit->empty()) {
                    // To make solver state transition.
                    solver->map_command_from(*unit) ;
                }
            }

            // For commands consisting of single command unit,
            // it is more efficient to repeat using an internal counter.
            if(map_cmd.size() == 1) {
                fetched_outputs.push_back(map_cmd.front()) ;
                fetched_counts.push_back(count) ;
                return ;
            }

            // For commands consisting of the multiple unit,
            // assume the count as one and add multiple commands with a count of one.
            std::vector<std::uint16_t> counts(map_cmd.size(), 1) ;
            for(std::uint16_t i = 0 ; i < count ; i ++) {
                fetched_outputs.insert(
                    fetched_outputs.end(),
                    map_cmd.begin(), map_cmd.end()) ;

                fetched_counts.insert(
                    fetched_counts.end(),
                    counts.begin(), counts.end()) ;
            }
        }

        SystemCall InputHub::pull_inputs(Mode mode) {
            CmdUnit::SPtr input ;
            std::vector<CmdUnit::SPtr> outputs ;
            std::vector<std::uint16_t> counts ;
            fetch_inputs(input, outputs, counts, mode) ;

            auto res = SystemCall::SUCCEEDED ;
            for(int i = 0 ; i < outputs.size() ; i ++) {
                res = util::enum_or(res, outputs[i]->execute(counts[i])) ;
            }
            return res ;
        }

        void InputHub::forget_previous_input(Mode mode) {
            get_solver(mode)->backward_state(1) ;
        }

        void InputHub::add_map(
            const std::string& trigger_cmd,
            const std::string& target_cmd,
            Mode mode) {
            get_solver(mode)->add_map(trigger_cmd, target_cmd) ;
        }

        void InputHub::add_noremap(
            const std::string& trigger_cmd,
            const std::string& target_cmd,
            Mode mode) {
            get_solver(mode)->add_noremap(trigger_cmd, target_cmd) ;
        }

        bool InputHub::remove_mapping(
            const std::string& trigger_cmd,
            Mode mode) {
            return get_solver(mode)->remove(trigger_cmd) ;
        }

        void InputHub::clear_mapping(Mode mode) {
            if(mode == Mode::UNDEFINED) {
                for(auto& solver : pimpl->solvers_) {
                    solver->clear() ;
                }
            }
            else {
                get_solver(mode)->clear() ;
            }
        }

        void InputHub::apply_mapping(Mode mode) {
            if(mode == Mode::UNDEFINED) {
                for(int m = 0 ; m < pimpl->solvers_.size() ; m ++) {
                    pimpl->solvers_[m]->deploy(static_cast<Mode>(m)) ;
                }
            }
            else {
                get_solver(mode)->deploy(mode) ;
            }
        }
    }
}
