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
    using namespace vind::core ;
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

    struct InputQueue {
        std::vector<CmdUnit::SPtr> keys{} ;
        std::vector<std::uint16_t> cnts{} ;
        std::size_t idx{0} ;

        void clear() {
            idx = 0 ;
            keys.clear() ;
            cnts.clear() ;
        }

        bool empty() {
            return keys.empty() ;
        }
    } ;
}

namespace vind
{
    namespace core
    {
        struct InputHub::Impl {
            ModeArray<std::shared_ptr<MapSolver>> solvers_{} ;
            TypingEmulator typeemu_{} ;
            std::string count_{} ;
            InputQueue q_ ;
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

        bool InputHub::get_typed_input(
                CmdUnit::SPtr& fetched_input,
                std::uint16_t& count,
                Mode mode,
                bool parse_count) {
            auto raw_input = InputGate::get_instance().pressed_list() ;
            auto input = pimpl->typeemu_.lowlevel_to_typing(raw_input) ;
            if(!input) {
                return false ;
            }

            auto solver = get_solver(mode) ;
            if(parse_count && !solver->is_matching_any()) {
                // parses the sequence of head counts as the string
                // and converts them into a number.
                auto new_count = extract_numbers(
                        *input, pimpl->count_.size() > 0) ;
                if(!new_count.empty()) {
                    pimpl->count_ += new_count ;
                    return false ;
                }
            }

            if(!pimpl->count_.empty()) {
                count = util::extract_num<std::uint16_t>(pimpl->count_) ;
                pimpl->count_.clear() ;
            }
            else {
                count = 1 ;
            }

            fetched_input = input ;
            return true ;
        }


        bool InputHub::is_empty_queue() const noexcept {
            return pimpl->q_.empty() ;
        }

        bool InputHub::fetch_input(
                CmdUnit::SPtr& fetched_input,
                std::uint16_t& fetched_count,
                Mode mode,
                bool parse_count) {
            if(is_empty_queue()) {
                get_typed_input(
                    fetched_input, fetched_count, mode, parse_count) ;
            }

            fetched_input = pimpl->q_.keys[pimpl->q_.idx] ;
            fetched_count = pimpl->q_.cnts[pimpl->q_.idx] ;
            pimpl->q_.idx ++ ;
            if(pimpl->q_.idx >= pimpl->q_.keys.size()) {
                pimpl->q_.clear() ;
            }
            return true ;
        }

        bool InputHub::fetch_all_inputs(
                std::vector<CmdUnit::SPtr>& fetched_inputs,
                std::vector<std::uint16_t>& fetched_counts,
                Mode mode,
                bool parse_count) {
            if(is_empty_queue()) {
                CmdUnit::SPtr input ;
                std::uint16_t count ;
                if(!get_typed_input(
                        input, count, mode, parse_count)) {
                    return false ;
                }
                fetched_inputs.push_back(input) ;
                fetched_counts.push_back(count) ;
                return true ;
            }

            for(std::size_t i = pimpl->q_.idx ;
                    i < pimpl->q_.keys.size() ; i ++) {
                fetched_inputs.push_back(pimpl->q_.keys[i]) ;
                fetched_counts.push_back(pimpl->q_.cnts[i]) ;
            }
            pimpl->q_.clear() ;
            return true ;
        }

        bool InputHub::enqueue_mapped(
                const CmdUnit::SPtr& input,
                std::uint16_t prefix_count,
                Mode mode) {
            auto solver = get_solver(mode) ;
            auto map_cmd = solver->map_command_from(*input) ;
            if(map_cmd.empty()) {
                return false ;
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
                pimpl->q_.keys.push_back(map_cmd.front()) ;
                pimpl->q_.cnts.push_back(prefix_count) ;
                return true ;
            }

            // For commands consisting of the multiple unit,
            // assume the count as one and add multiple commands with a count of one.
            std::vector<std::uint16_t> counts(map_cmd.size(), 1) ;
            for(std::uint16_t i = 0 ; i < prefix_count ; i ++) {
                pimpl->q_.keys.insert(
                    pimpl->q_.keys.end(),
                    map_cmd.begin(), map_cmd.end()) ;

                pimpl->q_.cnts.insert(
                    pimpl->q_.cnts.end(),
                    counts.begin(), counts.end()) ;
            }
            return true ;
        }

        bool InputHub::pull_input(
                CmdUnit::SPtr& fetched_input,
                std::uint16_t& fetched_count,
                Mode mode,
                bool parse_count) {
            if(!fetch_input(
                    fetched_input, fetched_count, mode, parse_count)) {
                return false ;
            }

            enqueue_mapped(fetched_input, fetched_count, mode) ;
            return true ;
        }

        bool InputHub::pull_all_inputs(
                std::vector<CmdUnit::SPtr>& fetched_inputs,
                std::vector<std::uint16_t>& fetched_counts,
                Mode mode,
                bool parse_count) {
            if(!fetch_all_inputs(
                    fetched_inputs, fetched_counts, mode, parse_count)) {
                return false ;
            }

            for(std::size_t i = 0 ; i < fetched_inputs.size() ; i ++) {
                enqueue_mapped(
                    fetched_inputs[i], fetched_counts[i], mode) ;
            }
            return true ;
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
                for(std::size_t m = 0 ; m < pimpl->solvers_.size() ; m ++) {
                    pimpl->solvers_[m]->deploy(static_cast<Mode>(m)) ;
                }
            }
            else {
                get_solver(mode)->deploy(mode) ;
            }
        }
    }
}
