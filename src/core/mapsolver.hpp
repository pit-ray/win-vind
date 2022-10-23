#ifndef _MAPSOLVER_HPP
#define _MAPSOLVER_HPP

#include <memory>
#include <string>
#include <vector>

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
            void deploy() ;
            void deploy_default(bool solve=true) ;

            void clear() ;
            void clear_default() ;

            void backward_state(int n) ;

            void reset_state() ;

            /*
             * @param[in] (raw_cmdunit) 
             *
             * @return target command
             */
            std::vector<CmdUnit::SPtr> map_command_from(
                    const CmdUnit& raw_cmdunit) ;

            std::vector<std::vector<CmdUnit::SPtr>> get_trigger_commands() const ;
            std::vector<std::vector<CmdUnit::SPtr>> get_target_commands() const ;

            void copy_deployment_from(const MapSolver& rhs) ;

            bool is_matching_any() const noexcept ;
            bool is_accepted_any() const noexcept ;
            bool is_rejected_any() const noexcept ;
            bool is_rejected_all() const noexcept ;
        } ;
    }
}

#endif
