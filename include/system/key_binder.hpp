#ifndef _KEY_BINDER_HPP
#define _KEY_BINDER_HPP

#include "binded_func.hpp"
#include "key_logger.hpp"

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
            const InvalidPolicy ip=InvalidPolicy::AllSystemKey) ;

    const BindedFunc::shp_t find_func(
            const KeyLogger& logger,
            const BindedFunc::shp_t& running_func=nullptr,
            ModeManager::Mode mode=ModeManager::get_mode()) ;

    void load_config() ;
    void call_matched_funcs() ;
}

#endif
