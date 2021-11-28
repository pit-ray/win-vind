#ifndef _MAPGATE_HPP
#define _MAPGATE_HPP

#include "mode.hpp"


namespace vind
{
    namespace core
    {
        class KeyLog ;

        class MapGate {
        private:
            explicit MapGate() ;
            virtual ~MapGate() noexcept ;

            struct Impl ;
            std::unique_ptr<Impl> pimpl ;

        public:
            static MapGate& get_instance() ;

            //
            //
            //
            //
            //
            void reconstruct() ;

            //
            //
            //
            //
            //
            void get_binding_hook() ;

            //
            //
            //
            //
            //
            void add_to_pool(const KeyLog& log) ;

            //
            //
            //
            //
            //
            //
            KeyLog do_map_overlog(
                    const KeyLog& log,
                    Mode mode=get_global_mode()) ;

            //
            //
            //
            //
            //
            //
            void do_map_syncstate(
                    KeyCode key,
                    bool state,
                    Mode mode=get_global_mode()) ;

            MapGate(MapGate&&)                 = delete ;
            MapGate& operator=(MapGate&&)      = delete ;
            MapGate(const MapGate&)            = delete ;
            MapGate& operator=(const MapGate&) = delete ;
        } ;
    }
}

#endif
