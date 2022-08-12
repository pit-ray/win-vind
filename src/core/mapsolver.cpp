#ifdef SOLVE

#include "cmdmatcher.hpp"
#include "cmdunit.hpp"
#include "mapsolver.hpp"
#include "mode.hpp"


namespace
{
    struct Map {
        CmdMatcher trigger_matcher ;
        std::shared_ptr<CmdUnit> target_cmdunit ;
    } ;
}


namespace vind
{
    namespace core
    {
        struct MapSolver::Impl {
            ModeArray<std::vector<Map>> deployed_ ;
            ModeArray<std::vector<Map>> default_noremap_ ;
            ModeArray<std::vector<Map>> registered_noremap_ ;
            ModeArray<std::vector<Map>> registered_map_ ;
        } ;

        MapSolver::MapSolver()
        : pimpl(std::make_unique<Impl>())
        {

        }

        MapSolver::~MapSolver() noexcept = default ;

        MapSolver::MapSolver(MapSolver&&) = default ;
        MapSolver& MapSolver::operator=(MapSolver&&) = default ;

        void MapSolver::add_map(
            const std::string& trigger_cmd,
            const std::string& target_cmd,
            Mode mode) {
            pimpl->registered_map_[static_cast<int>(mode)].push_back(

        }

        void MapSolver::add_noremap(
            const std::string& trigger_cmd,
            const std::string& target_cmd,
            Mode mode) {


        }

        void MapSolver::solve(Mode mode) {
        }

        void MapSolver::add_solved_map(
            const std::string& trigger_cmd,
            const std::string& target_cmd,
            Mode mode) {
            add_map(trigger_cmd, target_cmd, mode) ;
            solve(mode) ;
        }

        void add_solved_noremap(
            const std::string& trigger_cmd,
            const std::string& target_cmd,
            Mode mode) {
            add_noremap(trigger_cmd, target_cmd, mode) ;
            solve(mode) ;
        }

        void MapSolver::remove(
            const std::string& trigger_cmd,
            Mode mode) {

        }

        void MapSolver::clear(Mode mode) {

        }

        void MapSolver::clear_all() {

        }

        void MapSolver::backward_state(std::size_t) {

        }

        void MapSolver::reset_state() {

        }

        // If the trigger command is matched, it return true.
        bool MapSolver::map_command_to(
            const std::shared_ptr<CmdUnit>& trigger_cmdunit,
            std::vector<std::shared_ptr<CmdUnit>>& target_cmd,
            unsigned int& count) {

        }

        bool MapSolver::map_syncstate(
            KeyCode hook_key,
            bool press_sync_state,
            Mode mode) {
            auto midx = static_cast<int>(mode) ;
            auto target = pimpl->mapgate_[midx].syncmap_[hook_key.to_code()] ;
            if(target.empty()) {
                return false ;
            }

            if(press_sync_state) {
                open_some_ports(target.begin(), target.end()) ;

                press_keystate(target.begin(), target.end()) ;
                close_some_ports(target.begin(), target.end()) ;
            }
            else {
                open_some_ports(target.begin(), target.end()) ;
                release_keystate(target.begin(), target.end()) ;
                close_some_ports(target.begin(), target.end()) ;
            }
            return true ;
        }
    }
}
#endif
