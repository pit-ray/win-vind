#include "key/log_map.hpp"

#include "g_maps.hpp"
#include "mapdefs.hpp"
#include "mode.hpp"

namespace
{
    using namespace vind ;

    using ModeKeySetMaps = ModeArray<std::vector<std::pair<KeySet, KeySet>>> ;
    ModeKeySetMaps g_modemaps{} ;
}


namespace vind
{
    namespace logmap {

        void load_config() {
            ModeKeySetMaps().swap(g_modemaps) ;

            std::vector<gmaps::UniqueMap> maps ;

            for(std::size_t i = 0 ; i < mode::mode_num() ; i ++) {
                maps.clear() ;

                gmaps::get_maps(static_cast<mode::Mode>(i), maps) ;

                for(auto& map : maps) {
                    if(!map.is_function()) {

                        auto trigger_set = map.trigger_command().front() ;
                        auto target_set = map.create_target_command().front() ;

                        g_modemaps[i].emplace_back(std::move(trigger_set), std::move(target_set)) ;
                    }
                }
            }
        }

        KeyLog do_noremap(
                const KeyLog& log,
                mode::Mode mode) {

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
    }
}
