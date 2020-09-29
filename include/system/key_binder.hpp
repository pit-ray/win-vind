#ifndef _KEY_BINDER_HPP
#define _KEY_BINDER_HPP

#include "command.hpp"
#include "key_binding.hpp"

namespace KeyBinder
{
    void init() ;

    enum InvalidPolicy {
        None,
        AllSystemKey,
        UnbindedSystemKey
    } ;

    bool is_invalid_log(KeyLogger& logger, const InvalidPolicy ip=InvalidPolicy::AllSystemKey) noexcept ;
    const kbg::shp_t find_keybinds(const KeyLogger& logger) noexcept ;

    void load_config() noexcept ;
    void call_matched_funcs() noexcept ;
}

#endif
