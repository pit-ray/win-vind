#ifndef _KEY_ABSORBER_HPP
#define _KEY_ABSORBER_HPP

#include <vector>
#include "key_log.hpp"
#include "virtual_key_fwd.hpp"

namespace KeyAbsorber
{
    bool install_hook() noexcept ;
    bool is_pressed(const unsigned char keycode) noexcept ;
    const KeyLog get_pressed_list() noexcept ;

    bool is_closed() noexcept ;
    void close() noexcept ;
    bool close_with_refresh() noexcept ;
    void open() noexcept ;
    void open_keys(const KeyLog::data_t& key) noexcept ;
    void open_key(const unsigned char key) noexcept ;

    //These functions is existed in order to fool KeyLogger as no-changing.
    //For example, EdiMoveCaretLeft...
    void release_vertually(const unsigned char key) noexcept ;
    void press_vertually(const unsigned char key) noexcept ;
}


#endif