#ifndef _LOG_MAP_HPP
#define _LOG_MAP_HPP

#include "key_log.hpp"
#include "mode.hpp"

namespace vind
{
    namespace core {
        void load_keycodemap_config() ;

        KeyLog do_keycode_noremap(
                const KeyLog& log,
                Mode mode=get_global_mode()) ;

        bool do_keycode_map(
                KeyCode key,
                bool press_sync_state=true,
                Mode mode=get_global_mode()) ;

    }
}

#endif
