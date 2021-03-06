#ifndef _LOG_MAP_HPP
#define _LOG_MAP_HPP

#include "key_log.hpp"
#include "mode.hpp"

namespace vind
{
    namespace logmap {
        void load_config() ;

        KeyLog do_noremap(
                const KeyLog& log,
                mode::Mode mode=mode::get_global_mode()) ;

        bool do_keycode_map(
                KeyCode key,
                bool press_sync_state=true,
                mode::Mode mode=mode::get_global_mode()) ;

    }
}

#endif
