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

            void add_map(
                const std::string& trigger_strcmd,
                const std::string& target_strcmd) ;

            void add_noremap(
                const std::string& trigger_strcmd,
                const std::string& target_strcmd) ;

            void remove(const std::string& trigger_strcmd) ;

            void solve() ;

            void clear() ;

            void backward_state(std::size_t n) ;

            void reset_state() ;

            // If the trigger command is matched, it return true.
            bool map_command_to(
                const CmdUnit& raw_cmdunit,
                std::vector<CmdUnit::SPtr>& target_cmd,
                unsigned int& count) ;

            /**
             * A gate uses to synchronize the state of a key at low-level
             * with mapped to the hook_key. It return ture if the map was done,
             * false if the map does not exist.
             * It works like the following, with the keys connected by bars.
             *
             *     _____________________________________
             *   _|_                        _|_        _|_
             *  /   \                      /   \      /   \
             * /_____\                    /_____\    /_____\
             * hook_key        keyset = {   key1  ,    key2   }
             *
             */
            bool map_syncstate(KeyCode hook_key, bool press_sync_state) ;
        } ;
    }
}

#endif
