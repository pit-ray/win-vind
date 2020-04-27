#ifndef _KEY_ABSORBER_HPP
#define _KEY_ABSORBER_HPP

#include <vector>
#include "virtual_key_fwd.hpp"

class KeyLog ;

namespace KeyAbsorber
{
    using key_log_t = std::vector<unsigned char> ;

    bool is_install_hook() noexcept ;
    bool is_down(const unsigned char keycode) noexcept ;
    const KeyLog get_downed_list() noexcept ;

    bool is_closed() noexcept ;
    void close() noexcept ;
    void open() noexcept ;
    void noword_activate() noexcept ;
    void open_keys(const key_log_t& key) noexcept ;
    void open_key(const unsigned char key) noexcept ;
}


#endif