#include "keycodemap.hpp"

namespace
{
    using namespace vind ;

    using ModeKeySetMaps = ModeArray<std::vector<std::pair<KeySet, KeySet>>> ;
    ModeKeySetMaps g_modemaps{} ;

    using ModeKeyCodeMap = ModeArray<std::array<KeyCode, 256>> ;
    ModeKeyCodeMap g_keycodemap{} ;

    template <typename T1, typename T2, typename Mode>
    KeyCode remap_recursively(T1 src_code, T2 first_src_code, Mode mode) {
        auto dst = g_keycodemap[mode][src_code] ;
        if(dst == src_code) {
            return static_cast<KeyCode>(src_code) ;
        }
        if(dst == first_src_code) {
            PRINT_ERROR("{" + core::get_keycode_name(first_src_code) \
                    + "} recursively remaps itself in " + core::mode_to_name(mode) + ".") ;
            return static_cast<KeyCode>(first_src_code) ;
        }
        return remap_recursively(dst, first_src_code, mode) ;
    }
}


namespace vind
{
    namespace bind
    {
        KeycodeMap::Impl {
        } ;

        KeycodeMap::KeycodeMap()
        : BindedFuncCreator("internal_keycodemap"),
          pimpl(std::make_unique<Impl>())
        {}

        KeycodeMap::~KeycodeMap() noexcept = default ;
        KeycodeMap::KeycodeMap(KeycodeMap&&) = default ;
        KeycodeMap& KeycodeMap::operator=(KeycodeMap&& = default ;

        void KeycodeMap::reconstruct() {
            ModeKeySetMaps().swap(g_modemaps) ;
            ModeKeyCodeMap().swap(g_keycodemap) ;

            std::vector<UniqueMap> maps ;

            for(std::size_t i = 0 ; i < mode_num() ; i ++) {
                maps.clear() ;
                get_maps(static_cast<Mode>(i), maps) ;

                for(int j = 0 ; j < 256 ; j ++) {
                    g_keycodemap[i][j] = static_cast<KeyCode>(j) ;
                }

                for(auto& map : maps) {
                    if(map.is_noremap()) {
                        //
                        // Currently, only key-to-key or keyset-to-keyset maps are supported,
                        // command-to-command maps are not implemented.
                        // Perhaps matching using state transitions or
                        // a mechanism that analyzes the keylogger from the beginning is needed.
                        //
                        // Example:
                        //      gnnoremap s b           " YES
                        //      gnnoremap s <m-h>       " YES
                        //      gnnoremap <c-a> <c-b>   " YES
                        //      gnnoremap <c-a> b       " YES
                        //
                        //      gnnoremap gg <c-w>s     " NO
                        //      gnnoremap g  <c-w>s     " NO
                        //      gnnoremap gg s          " NO
                        //      gnnoremap gg <c-w>      " NO
                        //
                        auto trigger_set = map.trigger_command().front() ;
                        auto target_set  = map.target_command().front() ;

                        g_modemaps[i].emplace_back(std::move(trigger_set), std::move(target_set)) ;
                    }
                    else if(map.is_map()) {

                        //
                        // Currently, only key-to-key mapping is supported.
                        // If you want to do this on a low level, it may be a little difficult.
                        //
                        // Example:
                        //      gnmap s b       " YES
                        //
                        //      gnmap <s-w> s   " NO
                        //      gnmap s <s-w>   " NO
                        //
                        auto trigger_key = map.trigger_command().front().front() ;
                        auto target_key = map.target_command().front().front() ;

                        g_keycodemap[i][trigger_key] = target_key ;
                    }
                }
            }

            // solve recursive remap
            for(std::size_t i = 0 ; i < mode_num() ; i ++) {
                auto& ar = g_keycodemap[i] ;
                std::array<KeyCode, 256> resolved_ar{} ;
                for(int j = 0 ; j < 256 ; j ++) {
                    resolved_ar[j] = remap_recursively(j, j, i) ;
                }
                ar = std::move(resolved_ar) ;
            }
        }

        void KeycodeMap::sprocess() const {

        }

        void KeycodeMap::sprocess(core::NTypeLogger& parent_lgr) const {

        }

        void KeycodeMap::spocess(const core::CharLogger& parent_lgr) const {

        }

        KeyLog do_log_map(const KeyLog& log, Mode mode) {
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

        bool do_sync_map(KeyCode key, bool press_sync_state, Mode mode) {
            auto target = g_keycodemap[static_cast<int>(mode)][key] ;
            if(target == key) {
                return false ;
            }

            if(press_sync_state) {
                util::press_keystate(target, true) ;
            }
            else {
                if(is_really_pressed(target) || is_pressed(target)) {
                    util::release_keystate(target, true) ;
                }
            }
            return true ;
        }
    }
}
