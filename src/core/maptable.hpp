#ifndef _MAPTABLE_HPP
#define _MAPTABLE_HPP

#include "mapsolver.hpp"
#include "mode.hpp"

#include <memory>

namespace vind
{
    namespace core
    {
        class MapTable {
        private:
            struct Impl ;
            std::unique_ptr<Impl> pimpl ;

            explicit MapTable() ;
            virtual ~MapTable() noexcept ;

        public:
            static MapTable& get_instance() ;
            const std::shared_ptr<MapSolver>& get_solver(
                Mode mode=get_global_mode()) ;

            void add_map(
                const std::string& trigger_cmd,
                const std::string& target_cmd,
                Mode mode) ;

            void add_noremap(
                const std::string& trigger_cmd,
                const std::string& target_cmd,
                Mode mode) ;

            bool remove(
                const std::string& trigger_cmd,
                Mode mode) ;

            void clear(Mode mode) ;

            void solve_remapping(Mode mode) ;
            void solve_remapping() ;
        } ;
    }
}

#endif
