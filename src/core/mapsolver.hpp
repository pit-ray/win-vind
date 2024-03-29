#ifndef _MAPSOLVER_HPP
#define _MAPSOLVER_HPP

#include <memory>
#include <string>
#include <vector>

#include "cmdmatcher.hpp"
#include "cmdunit.hpp"
#include "mode.hpp"


namespace vind
{
    namespace core
    {
        class MapSolver {
        private:
            struct Impl ;
            std::unique_ptr<Impl> pimpl ;

        public:
            explicit MapSolver(bool enable_typing_emulator=true) ;
            virtual ~MapSolver() noexcept ;

            MapSolver(const MapSolver&) = delete ;
            MapSolver& operator=(const MapSolver&) = delete ;

            MapSolver(MapSolver&&) ;
            MapSolver& operator=(MapSolver&&) ;

            void add_default(
                const std::string& trigger_strcmd,
                const std::string& target_strcmd) ;

            void add_map(
                const std::string& trigger_strcmd,
                const std::string& target_strcmd) ;

            void add_noremap(
                const std::string& trigger_strcmd,
                const std::string& target_strcmd) ;

            // Delete user-defined mappings.
            bool remove(const std::string& trigger_strcmd) ;

            bool remove_default(const std::string& trigger_strcmd) ;

            // Resolve the remapping of the added maps and deploy
            // to the mapping used in the matching.
            // The mode is used to register the low-level key mapping
            // into InputGate for specific mode. If the mode is UNDEFINED,
            // the low-level mapping is treated the same as the general
            // mappings without low-level.
            void deploy(Mode mode=Mode::UNDEFINED) ;
            void deploy_default(bool solve=true) ;

            void clear() ;
            void clear_default() ;

            void backward_state(int n) ;

            void reset_state() ;

            /*
             * @param[in] (raw_cmdunit) 
             *
             * @param[in] (auto_reset)
             *     If true, the states of each matcher are reset
             *     after any are accepted or rejected automatically.
             *
             * @return target command
             */
            std::vector<CmdUnit::SPtr> map_command_from(
                const CmdUnit& raw_cmdunit,
                bool auto_reset=true) ;

            std::vector<std::vector<CmdUnit::SPtr>> get_trigger_commands() const ;
            std::vector<std::vector<CmdUnit::SPtr>> get_target_commands() const ;

            std::vector<std::shared_ptr<CmdMatcher>> get_trigger_matchers() const ;

            void copy_deployment_from(const MapSolver& rhs) ;

            bool is_matching_any() const noexcept ;
            bool is_accepted_any() const noexcept ;
            bool is_rejected_any() const noexcept ;
            bool is_rejected_all() const noexcept ;

            // Return the maximum history size.
            std::size_t max_history_size() const noexcept ;
        } ;
    }
}

#endif
