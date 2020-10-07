#ifndef _KEY_BINDER_HPP
#define _KEY_BINDER_HPP

#include "binded_func.hpp"

namespace KeyBinder
{
    void init() ;

    enum InvalidPolicy {
        None,
        AllSystemKey,
        UnbindedSystemKey
    } ;

    bool is_invalid_log(
            KeyLogger& logger,
            const InvalidPolicy ip=InvalidPolicy::AllSystemKey) noexcept ;

    const BindedFunc::shp_t find_keybinds(
            const KeyLogger& logger,
            const BindedFunc::shp_t& current_func=nullptr,
            ModeManager::Mode mode=ModeManager::get_mode()) ;

    void load_config() ;
    void call_matched_funcs() ;
}

#endif
