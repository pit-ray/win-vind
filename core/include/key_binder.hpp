#ifndef _KEY_BINDER_HPP
#define _KEY_BINDER_HPP

#include "binded_func.hpp"
#include "key_logger.hpp"
#include "mode_manager.hpp"

namespace KeyBinder
{
    void init() ;

    enum InvalidPolicy {
        None,
        AllSystemKey,
        UnbindedSystemKey
    } ;

    bool is_invalid_log(
            const KeyLog& log,
            const InvalidPolicy ip=InvalidPolicy::AllSystemKey) ;

    const BindedFunc::shp_t find_func(
            const KeyLogger& logger,
            const BindedFunc::shp_t& running_func=nullptr,
            const bool full_scan=false, //The cost for computing is high.
            ModeManager::Mode mode=ModeManager::get_mode()) ;

    const BindedFunc::shp_t find_func_byname(const std::string& name) ;

    void load_config() ;
    void call_matched_funcs() ;
}

#endif
