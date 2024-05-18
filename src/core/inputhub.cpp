#include "inputhub.hpp"

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

    class InputQueue {
    private:
        std::vector<CmdUnit::SPtr> keys ;
        std::vector<std::uint16_t> cnts ;
        std::size_t idx ;

    public:
        explicit InputQueue()
        : keys(),
          cnts(),
          idx(0)
        {}

        void clear() {
            idx = 0 ;
            keys.clear() ;
            cnts.clear() ;
        }

        bool empty() {
            return keys.empty() ;
        }

        bool dequeue(CmdUnit::SPtr& key, std::uint16_t& count) {
            if(empty()) {
                return false ;
            }
            key = keys[idx] ;
            count = cnts[idx] ;
            idx ++ ;
            if(idx >= keys.size()) {
                clear() ;
            }
            return true ;
        }

        bool enqueue(const CmdUnit::SPtr& key, std::uint16_t count) {
            keys.push_back(key) ;
            cnts.push_back(count) ;
            return true ;
        }

        void dequeue_full(
                std::vector<CmdUnit::SPtr>& dump_keys,
                std::vector<std::uint16_t>& dump_cnts) {
            dump_keys.insert(
                dump_keys.end(), keys.begin() + idx, keys.end()) ;
            dump_cnts.insert(
                dump_cnts.end(), cnts.begin() + idx, cnts.end()) ;
            clear() ;
        }

        void enqueue_full(
                const std::vector<CmdUnit::SPtr>& push_keys,
                const std::vector<std::uint16_t>& push_cnts) {
            keys.insert(keys.end(), push_keys.begin(), push_keys.end()) ;
            cnts.insert(cnts.end(), push_cnts.begin(), push_cnts.end()) ;
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
            InputQueue typing_queue_{} ;
            InputQueue waiting_queue_{} ;
        } ;

        InputHub::InputHub()
        : pimpl(std::make_unique<Impl>())
        {
            for(std::size_t m = 0 ; m < pimpl->solvers_.size() ; m ++) {
                pimpl->solvers_[m] = std::make_shared<MapSolver>(false) ;
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
            if(parse_count && solver->max_history_size() == 0) {
                // parses the sequence of head counts as the string
                // and converts them into a number.
                auto new_count = extract_numbers(
                        *input, pimpl->count_.size() > 0) ;
                if(!new_count.empty()) {
                    pimpl->count_ += new_count ;

                    // Store the count number, but return as false.
                    count = util::extract_num<std::uint16_t>(pimpl->count_) ;
                    return false ;
                }
            }

            if(!pimpl->count_.empty()) {
                if(pimpl->count_.back() != ';') {
                    pimpl->count_ += ';' ;  // Final symbol of counts
                }
                count = util::extract_num<std::uint16_t>(pimpl->count_) ;
            }
            else {
                count = 1 ;
            }
            fetched_input = input ;
            return true ;
        }

        bool InputHub::do_typing(const CmdUnit::SPtr& input) {
            if(input->empty()) {
                return false ;
            }
            return pimpl->typing_queue_.enqueue(input, 1) ;
        }

        bool InputHub::is_empty_queue() const noexcept {
            return pimpl->typing_queue_.empty() \
                && pimpl->waiting_queue_.empty() ;
        }

        void InputHub::clear_queue() {
            pimpl->waiting_queue_.clear() ;
            pimpl->typing_queue_.clear() ;
        }

        bool InputHub::fetch_input(
                CmdUnit::SPtr& fetched_input,
                std::uint16_t& fetched_count,
                Mode mode,
                bool parse_count) {
            if(!pimpl->waiting_queue_.empty()) {
                return pimpl->waiting_queue_.dequeue(fetched_input, fetched_count) ;
            }

            if(!pimpl->typing_queue_.empty()) {
                return pimpl->typing_queue_.dequeue(fetched_input, fetched_count) ;
            }

            return get_typed_input(
                fetched_input, fetched_count, mode, parse_count) ;
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

            pimpl->waiting_queue_.dequeue_full(fetched_inputs, fetched_counts) ;
            pimpl->typing_queue_.dequeue_full(fetched_inputs, fetched_counts) ;
            return true ;
        }

        bool InputHub::enqueue(
            const CmdUnit::SPtr& unit,
            std::uint16_t prefix_count) {
            pimpl->waiting_queue_.enqueue(unit, prefix_count) ;
            return true ;
        }

        bool InputHub::enqueue_mapped(
                const CmdUnit::SPtr& input,
                std::uint16_t prefix_count,
                Mode mode) {
            auto solver = get_solver(mode) ;
            auto map_cmd = solver->map_command_from(*input, false) ;
            if(solver->is_accepted_any() || solver->is_rejected_all()) {
                if(!pimpl->count_.empty() && pimpl->count_.back() == ';') {
                    pimpl->count_.clear() ;  // Erase previous counts.
                }
                solver->reset_state() ;
            }
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

            if(map_cmd.size() == 1) {
                // For commands consisting of single command unit,
                // it is more efficient to repeat using an internal counter.
                pimpl->waiting_queue_.enqueue(map_cmd.front(), prefix_count) ;
                return true ;
            }

            // For commands consisting of the multiple unit,
            // assume the count as one and add multiple commands with a count of one.
            std::vector<std::uint16_t> counts(map_cmd.size(), 1) ;
            for(std::uint16_t i = 0 ; i < prefix_count ; i ++) {
                pimpl->waiting_queue_.enqueue_full(map_cmd, counts) ;
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
            Mode mode,
            bool as_default) {

            auto solver = get_solver(mode) ;

            if(as_default) {
                solver->add_default(trigger_cmd, target_cmd) ;
            }
            else {
                solver->add_map(trigger_cmd, target_cmd) ;
            }
        }

        void InputHub::add_noremap(
            const std::string& trigger_cmd,
            const std::string& target_cmd,
            Mode mode,
            bool as_default) {

            auto solver = get_solver(mode) ;

            if(as_default) {
                solver->add_default(trigger_cmd, target_cmd) ;
            }
            else {
                solver->add_noremap(trigger_cmd, target_cmd) ;
            }
        }

        bool InputHub::remove_mapping(
            const std::string& trigger_cmd,
            Mode mode,
            bool as_default) {

            auto solver = get_solver(mode) ;

            if(as_default) {
                return solver->remove_default(trigger_cmd) ;
            }
            return solver->remove(trigger_cmd) ;
        }

        void InputHub::clear_mapping(Mode mode, bool as_default) {
            if(mode == Mode::UNDEFINED) {
                for(auto& solver : pimpl->solvers_) {
                    if(as_default) {
                        solver->clear_default() ;
                    }
                    else {
                        solver->clear() ;
                    }
                }
            }
            else {
                auto solver = get_solver(mode) ;
                if(as_default) {
                    solver->clear_default() ;
                }
                else {
                    solver->clear() ;
                }
            }
        }

        void InputHub::apply_mapping(Mode mode, bool as_default) {
            if(mode == Mode::UNDEFINED) {
                for(std::size_t m = 0 ; m < pimpl->solvers_.size() ; m ++) {
                    auto& solver = pimpl->solvers_[m] ;
                    if(as_default) {
                        solver->deploy_default(true) ;
                    }
                    solver->deploy(static_cast<Mode>(m)) ;
                }
            }
            else {
                auto solver = get_solver(mode) ;
                if(as_default) {
                    solver->deploy_default(true) ;
                }
                solver->deploy(mode) ;
            }
        }
    }
}
