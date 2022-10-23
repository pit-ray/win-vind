#include "inputhub.hpp"

#include "cmdunit.hpp"
#include "inputgate.hpp"
#include "mapdefault.hpp"
#include "mapsolver.hpp"
#include "typeemu.hpp"

#include "util/def.hpp"
#include "util/string.hpp"

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

            template <typename T>
            std::shared_ptr<MapSolver> get_solver(T mode) {
                return solvers_[static_cast<int>(mode)] ;
            }
        } ;

        InputHub::InputHub()
        : pimpl(std::make_unique<Impl>())
        {
            for(std::size_t m = 0 ; m < pimpl->solvers_.size() ; m ++) {
                auto solver = std::make_shared<MapSolver>(false) ;
                for(const auto& [trigger, target] : get_default_map(m)) {
                    solver->add_default(trigger, target) ;
                }
                solver->deploy_default(true) ;
                solver->deploy() ;
                pimpl->solvers_[m] = std::move(solver) ;
            }
        }

        InputHub::~InputHub() noexcept = default ;

        InputHub& InputHub::get_instance() {
            static InputHub instance{} ;
            return instance ;
        }

        bool InputHub::fetch_inputs(
                std::vector<CmdUnit::SPtr>& fetched_inputs,
                std::vector<std::uint16_t>& fetched_counts,
                Mode mode) {
            auto raw_inputs = InputGate::get_instance().pressed_list() ;

            auto inputs = pimpl->typeemu_.lowlevel_to_typing(raw_inputs) ;
            if(!inputs) {
                return false ;
            }

            auto solver = pimpl->get_solver(mode) ;

            if(!solver->is_matching_any()) {
                // parses the sequence of head counts as the string
                // and converts them into a number.
                auto new_count = extract_numbers(*inputs, pimpl->count_.size() > 0) ;
                if(!new_count.empty()) {
                    pimpl->count_ += new_count ;
                    return false ;
                }
            }

            std::uint16_t count = 1 ;
            if(!pimpl->count_.empty()) {
                count = static_cast<std::uint16_t>(util::extract_num(pimpl->count_)) ;
                pimpl->count_.clear() ;
            }

            auto map_cmd = solver->map_command_from(*inputs) ;
            if(map_cmd.empty()) {
                return false ;
            }

            // Update states of a solver using the solved mapping.
            for(const auto& unit : map_cmd) {
                if(unit->empty()) {
                    continue ;
                }

                // To make solver state transition.
                auto cmd = solver->map_command_from(*unit) ;
                if(!cmd.empty()) {
                    throw RUNTIME_EXCEPT("Mapping has not been resolved.") ;
                }
            }

            // For commands consisting of single command unit,
            // it is more efficient to repeat using an internal counter.
            if(map_cmd.size() == 1) {
                fetched_inputs.push_back(map_cmd.front()) ;
                fetched_counts.push_back(count) ;
                return true ;
            }

            // For commands consisting of the multiple unit,
            // assume the count as one and add multiple commands with a count of one.
            std::vector<std::uint16_t> counts(map_cmd.size(), 1) ;
            for(std::uint16_t i = 0 ; i < count ; i ++) {
                fetched_inputs.insert(
                    fetched_inputs.end(),
                    map_cmd.begin(), map_cmd.end()) ;

                fetched_counts.insert(
                    fetched_counts.end(),
                    counts.begin(), counts.end()) ;
            }
            return true ;
        }

        void InputHub::pull_inputs(Mode mode) {
            std::vector<CmdUnit::SPtr> inputs{} ;
            std::vector<std::uint16_t> counts{} ;
            if(!fetch_inputs(inputs, counts, mode)) {
                return ;
            }

            for(int i = 0 ; i < inputs.size() ; i ++) {
                inputs[i]->execute(counts[i]) ;
            }
        }

        void InputHub::forget_previous_input(Mode mode) {
            pimpl->get_solver(mode)->backward_state(1) ;
        }

        void InputHub::add_map(
            const std::string& trigger_cmd,
            const std::string& target_cmd,
            Mode mode) {
            pimpl->get_solver(mode)->add_map(trigger_cmd, target_cmd) ;
        }

        void InputHub::add_noremap(
            const std::string& trigger_cmd,
            const std::string& target_cmd,
            Mode mode) {
            pimpl->get_solver(mode)->add_noremap(trigger_cmd, target_cmd) ;
        }

        bool InputHub::remove_mapping(
            const std::string& trigger_cmd,
            Mode mode) {
            return pimpl->get_solver(mode)->remove(trigger_cmd) ;
        }

        void InputHub::clear_mapping(Mode mode) {
            if(mode == Mode::UNDEFINED) {
                for(auto& solver : pimpl->solvers_) {
                    solver->clear() ;
                }
            }
            else {
                pimpl->get_solver(mode)->clear() ;
            }
        }

        void InputHub::apply_mapping(Mode mode) {
            if(mode == Mode::UNDEFINED) {
                for(auto& solver : pimpl->solvers_) {
                    solver->clear() ;
                }
            }
            else {
                pimpl->get_solver(mode)->deploy() ;
            }
        }
    }
}
