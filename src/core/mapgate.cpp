#include "mapgate.hpp"

#include "key_logger_base.hpp"
#include "mapdefs.hpp"
#include "mode.hpp"

#include <queue>


namespace vind
{
    namespace core
    {
        struct MapGate::Impl {
            std::queue<KeyLog> logpool_ ;
            MapArray<std::array<Command, 256>> synctable_ ;
        } ;

        MapGate::MapGate()
        : pimpl(std::make_unique<Impl>())
        {}

        MapGate::~MapGate() noexcept = default ;

        MapGate& MapGate::get_instance() {
            static MapGate instance{} ;
            return instance ;
        }

        void MapGate::reconstruct() {
        }

        void MapGate::add_to_pool(const KeyLog& log) {
            pimpl->logpool_.push(log) ;
        }

        KeyLog MapGate::do_map_overlog(
                const KeyLog& log,
                Mode mode) {
            KeyLog::Data mapped{} ;
            KeyLog::Data converted{} ;

            for(const auto& [src, dst] : g_modemaps[static_cast<int>(mode)]) {
                auto itr = src.cbegin() ;
                while(itr != src.cend()) {
                    if(!log.is_containing(*itr)) {
                        break ;
                    }
                    itr ++ ;
                }

                // matched
                if(itr == src.cend()) {
                    for(auto key : src) mapped.insert(key) ;
                    for(auto key : dst) converted.insert(key) ;
                }
            }

            for(auto key : (log - mapped)) {
                converted.insert(key) ;
            }

            return KeyLog(std::move(converted)) ;
        }

        void MapGate::do_map_syncstate(
                KeyLog key,
                bool state,
                Mode mode) {

        }

    }
}
