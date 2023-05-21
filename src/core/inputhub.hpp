#ifndef _INPUTHUB_HPP
#define _INPUTHUB_HPP

#include "cmdunit.hpp"
#include "mode.hpp"
#include "syscalldef.hpp"

#include <memory>
#include <string>
#include <vector>


namespace vind
{
    namespace core
    {
        class MapSolver ;

        class InputHub {
        private:
            struct Impl ;
            std::unique_ptr<Impl> pimpl ;

            explicit InputHub() ;
            virtual ~InputHub() noexcept ;

        public:
            static InputHub& get_instance() ;

            std::shared_ptr<MapSolver> get_solver(Mode mode) ;

            /*
             * Emulate text input and retrieve input at the appropriate time.
             * @param[out] (fetched_input) Retrieved input
             * @param[out] (fetched_count) Retrived prefix count of a command
             * @param[in] (mode) The mode of solver to analyze the prefix count.
             * @param[in] (parse_count) If false, treats the prefix number as the normal.
             */
            bool get_typed_input(
                CmdUnit::SPtr& fetched_input,
                std::uint16_t& fetched_count,
                Mode mode=get_global_mode(),
                bool parse_count=true) ;

            /*
             * Enqueue the input directly to the input euque.
             *
             * @param[in] (input) A raw input or a function unit.
             */
            bool do_typing(const CmdUnit::SPtr& input) ;

            /*
             * Check if the input queue is empty.
             */
            bool is_empty_queue() const noexcept ;

            /*
             * Dequeue all inputs from the input queue without receving them.
             */
            void clear_queue() ;

            /*
             * Dequeue an input from the input queue. If the input queue is empty,
             * get an input from the current states of a keyboard like typing.
             *
             * @param[out] (fetched_input) Retrieved input
             * @param[out] (fetched_count) Retrived prefix count of a command
             * @param[in] (mode) The mode of solver to analyze the prefix count.
             * @param[in] (parse_count) If false, treats the prefix number as the normal.
             */
            bool fetch_input(
                CmdUnit::SPtr& fetched_input,
                std::uint16_t& fetched_count,
                Mode mode=get_global_mode(),
                bool parse_count=true) ;

            /*
             * Dequeue all inputs from the input queue. If the input queue is empty,
             * get an input from the current states of a keyboard like typing.
             *
             * @param[out] (fetched_inputs) Retrieved all inputs
             * @param[out] (fetched_counts) Retrived prefix counts of each command
             * @param[in] (mode) The mode of solver to analyze the prefix count.
             * @param[in] (parse_count) If false, treats the prefix number as the normal.
             */
            bool fetch_all_inputs(
                std::vector<CmdUnit::SPtr>& fetched_inputs,
                std::vector<std::uint16_t>& fetched_counts,
                Mode mode=get_global_mode(),
                bool parse_count=true) ;

            /*
             * Maps input to other commands and adds the result to the input waiting
             * queue. The mapped commands are assumed to be input at the same time
             * and the solver automatically transitions between states.
             *
             * @param[in] (input) The trigger input of map
             * @param[in] (count) The prefix number of input command.
             * @param[in] (mode) The mode of solver to analyze the prefix number.
             */
            bool enqueue_mapped(
                const CmdUnit::SPtr& input,
                std::uint16_t prefix_count,
                Mode mode=get_global_mode()) ;

            /*
             * Takes input from the waiting input queue and returns it as the
             * fetched_input variable. It then further maps that input and adds
             * the mapped command to the queue. (fetch_input + enqueue_mapped)
             *
             * @param[out] (fetched_input) Retrieved input
             * @param[out] (fetched_count) Retrived prefix count of a command
             * @param[in] (mode) The mode of solver to analyze the prefix count.
             * @param[in] (parse_count) If false, treats the prefix number as the normal.
             */
            bool pull_input(
                CmdUnit::SPtr& fetched_input,
                std::uint16_t& fetched_count,
                Mode mode=get_global_mode(),
                bool parse_count=true) ;

            /*
             *
             * Takes all inputs from the waiting input queue and returns it as the
             * fetched_inputs variable. It then further maps that inputs and adds
             * the mapped command to the queue. (fetch_inputs + enqueue_mapped)
             *
             * @param[out] (fetched_inputs) Retrieved all inputs
             * @param[out] (fetched_counts) Retrived prefix counts of each command
             * @param[in] (mode) The mode of solver to analyze the prefix count.
             * @param[in] (parse_count) If false, treats the prefix number as the normal.
             */
            bool pull_all_inputs(
                std::vector<CmdUnit::SPtr>& fetched_inputs,
                std::vector<std::uint16_t>& fetched_counts,
                Mode mode=get_global_mode(),
                bool parse_count=true) ;

            void add_map(
                const std::string& trigger_cmd,
                const std::string& target_cmd,
                Mode mode) ;

            void add_noremap(
                const std::string& trigger_cmd,
                const std::string& target_cmd,
                Mode mode) ;

            bool remove_mapping(
                const std::string& trigger_cmd,
                Mode mode) ;

            void clear_mapping(Mode mode=Mode::UNDEFINED) ;
            void apply_mapping(Mode mode=Mode::UNDEFINED) ;
        } ;
    }
}

#endif
