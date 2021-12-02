#ifndef _MAPGATE_HPP
#define _MAPGATE_HPP

#include "keycode_def.hpp"
#include "mode.hpp"

#include <memory>
#include <queue>


namespace vind
{
    namespace core
    {
        class NTypeLogger ;
        class KeyLog ;

        class MapGate {
        private:
            explicit MapGate() ;
            virtual ~MapGate() noexcept ;

            struct Impl ;
            std::unique_ptr<Impl> pimpl ;

        public:
            static MapGate& get_instance() ;

            void reconstruct(Mode mode) ;
            void reconstruct() ;

            //
            // map_logger is a mapping from NTypeLogger to NTypeLogger.
            // What it means is that it is excited by a command, and return the log pool.
            // it would have been in if the corresponding command had been entered.
            //
            // In the case of noremap, the key message is not actually generated,
            // but in the case of map, the key message is generated in a state that is passed to Windows.
            //
            //
            using KeyLogPool = std::queue<KeyLog> ;
            KeyLogPool map_logger(
                    const NTypeLogger& lgr,
                    Mode mode=get_global_mode()) ;

            //
            // A gate uses to synchronize the state of a key at low-level
            // with mapped to the hook_key. It return ture if the map was done,
            // false if the map does not exist.
            //
            // It works like the following, with the keys connected by bars.
            /*
                 _____________________________________
               _|_                        _|_        _|_
              /   \                      /   \      /   \
             /_____\                    /_____\    /_____\
             hook_key        keyset = {   key1  ,    key2   }

            */
            bool map_syncstate(
                    KeyCode hook_key,
                    bool press_sync_state,
                    Mode mode=get_global_mode()) ;

            MapGate(MapGate&&)                 = delete ;
            MapGate& operator=(MapGate&&)      = delete ;
            MapGate(const MapGate&)            = delete ;
            MapGate& operator=(const MapGate&) = delete ;
        } ;
    }
}

#endif
