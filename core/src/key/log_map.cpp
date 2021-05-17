#include "key/log_map.hpp"

#include "g_maps.hpp"
#include "io/keybrd.hpp"
#include "key/keycodecvt.hpp"
#include "mapdefs.hpp"
#include "mode.hpp"

#include <stdexcept>
#include <unordered_set>

namespace
{
    using namespace vind ;

    using ModeKeySetMaps = ModeArray<std::vector<std::pair<KeySet, KeySet>>> ;
    ModeKeySetMaps g_modemaps{} ;

    using KeyCodeMap = ModeArray<std::unordered_map<KeyCode, KeyCode>> ;
    KeyCodeMap g_keycodemap{} ;
}


namespace vind
{
    namespace logmap {

        void load_config() {
            ModeKeySetMaps().swap(g_modemaps) ;
            KeyCodeMap().swap(g_keycodemap) ;

            std::vector<gmaps::UniqueMap> maps ;

            for(std::size_t i = 0 ; i < mode::mode_num() ; i ++) {
                maps.clear() ;

                gmaps::get_maps(static_cast<mode::Mode>(i), maps) ;

                for(auto& map : maps) {
                    if(map.is_noremap()) {
                        //
                        // Currently, only key-to-key or keyset-to-keyset maps are supported,
                        // command-to-command maps are not implemented.
                        // Perhaps matching using state transitions or
                        // a mechanism that analyzes the keylogger from the beginning is needed.
                        //
                        // Example:
                        //      gnnoremap s b           " OK
                        //      gnnoremap s <m-h>       " OK
                        //      gnnoremap <c-a> <c-b>   " OK
                        //      gnnoremap <c-a> b       " OK
                        //
                        //      gnnoremap gg <c-w>s     " NOT
                        //      gnnoremap g  <c-w>s     " NOT
                        //      gnnoremap gg s          " NOT
                        //      gnnoremap gg <c-w>      " NOT
                        //
                        auto trigger_set = map.trigger_command().front() ;
                        auto target_set  = map.create_target_command().front() ;

                        g_modemaps[i].emplace_back(std::move(trigger_set), std::move(target_set)) ;
                    }
                    else if(map.is_map()) {

                        //
                        // Currently, only key-to-key mapping is supported.
                        // If you want to do this on a low level, it may be a little difficult.
                        //
                        // Example:
                        //      gnmap s b       " OK
                        //
                        //      gnmap <s-w> s   " NOT
                        //      gnmap s <s-w>   " NOT
                        //
                        auto trigger_key = map.trigger_command().front().front() ;
                        auto target_key = map.create_target_command().front().front() ;

                        g_keycodemap[i][trigger_key] = target_key ;
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

        bool do_keycode_map(
                KeyCode key,
                bool press_sync_state,
                mode::Mode mode) {
            try {
                auto target = g_keycodemap[static_cast<int>(mode)].at(key) ;
                if(press_sync_state) {
                    keybrd::press_keystate(target) ;
                }
                else {
                    keybrd::release_keystate(target) ;
                }
                return true ;
            }
            catch(const std::out_of_range&) {
                return false ;
            }
        }
    }
}
