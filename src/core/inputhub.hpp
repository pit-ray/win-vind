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
             * @param[out] (fetched_input)
             * @param[out] (fetched_count)
             * @param[in] (mode)
             * @param[in] (parse_count)
             *
             */
            bool get_typed_input(
                CmdUnit::SPtr& fetched_input,
                std::uint16_t& fetched_count,
                Mode mode=get_global_mode(),
                bool parse_count=true) ;

            /*
             *
             *
             */
            bool is_empty_queue() const noexcept ;

            /*
             *
             * @param[out] (fetched_input)
             * @param[out] (fetched_count)
             * @param[in] (mode)
             * @param[in] (parse_count)
             *
             */
            bool fetch_input(
                CmdUnit::SPtr& fetched_input,
                std::uint16_t& fetched_count,
                Mode mode=get_global_mode(),
                bool parse_count=true) ;

            /*
             *
             * @param[out] (fetched_inputs)
             * @param[out] (fetched_counts)
             * @param[in] (mode)
             * @param[in] (parse_count)
             *
             */
            bool fetch_all_inputs(
                std::vector<CmdUnit::SPtr>& fetched_inputs,
                std::vector<std::uint16_t>& fetched_counts,
                Mode mode=get_global_mode(),
                bool parse_count=true) ;

            /*
             *
             * @param[in] (input)
             * @param[in] (count)
             * @param[in] (mode)
             *
             */
            bool enqueue_mapped(
                const CmdUnit::SPtr& input,
                std::uint16_t prefix_count,
                Mode mode=get_global_mode()) ;

            /*
             *
             * @param[out] (fetched_input)
             * @param[out] (fetched_count)
             * @param[in] (mode)
             * @param[in] (parse_count)
             *
             */
            bool pull_input(
                CmdUnit::SPtr& fetched_input,
                std::uint16_t& fetched_count,
                Mode mode=get_global_mode(),
                bool parse_count=true) ;

            /*
             *
             * @param[out] (fetched_inputs)
             * @param[out] (fetched_counts)
             * @param[in] (mode)
             * @param[in] (parse_count)
             *
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
