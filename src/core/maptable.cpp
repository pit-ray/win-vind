#include "maptable.hpp"

#include "cmdparser_old.hpp"
#include "mapdefault.hpp"

#include <array>
#include <mutex>
#include <unordered_map>


namespace vind
{
    namespace core
    {
        struct MapTable::Impl {
            ModeArray<std::shared_ptr<MapSolver>> solvers_{} ;
        } ;

        MapTable::MapTable()
        : pimpl(std::make_unique<Impl>())
        {
            for(std::size_t m = 0 ; m < pimpl->solvers_.size() ; m ++) {
                auto solver = std::make_shared<MapSolver>() ;
                for(const auto& [trigger, target] : get_default_map(m)) {
                    solver->add_default(trigger, target) ;
                }
                solver->deploy_default(true) ;
                solver->deploy() ;
                pimpl->solvers_[m] = std::move(solver) ;
            }
        }

        MapTable::~MapTable() noexcept = default ;

        MapTable& MapTable::get_instance() {
            static MapTable instance{} ;
            return instance ;
        }

        const std::shared_ptr<MapSolver>& MapTable::get_solver(Mode mode) {
            return pimpl->solvers_[static_cast<int>(mode)] ;
        }

        void MapTable::add_map(
            const std::string& trigger_cmd,
            const std::string& target_cmd,
            Mode mode) {
            get_solver(mode)->add_map(trigger_cmd, target_cmd) ;
        }

        void MapTable::add_noremap(
            const std::string& trigger_cmd,
            const std::string& target_cmd,
            Mode mode) {
            get_solver(mode)->add_noremap(trigger_cmd, target_cmd) ;
        }

        bool MapTable::remove(
            const std::string& trigger_cmd,
            Mode mode) {
            return get_solver(mode)->remove(trigger_cmd) ;
        }

        void MapTable::clear(Mode mode) {
            get_solver(mode)->clear() ;
        }

        void MapTable::clear() {
            for(auto& solver : pimpl->solvers_) {
                solver->clear() ;
            }
        }

        void MapTable::apply_mapping(Mode mode) {
            get_solver(mode)->deploy() ;
        }

        void MapTable::apply_mapping() {
            for(auto& solver : pimpl->solvers_) {
                solver->deploy() ;
            }
        }
    }
} // namespace vind
