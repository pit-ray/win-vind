#ifndef _KEY_ABSORBER_HPP
#define _KEY_ABSORBER_HPP

#include <vector>
#include "key_log.hpp"
#include "virtual_key_fwd.hpp"

namespace KeyAbsorber
{
    void install_hook() ;
    bool is_pressed(const unsigned char keycode) noexcept ;
    bool is_really_pressed(const unsigned char keycode) noexcept ;
    const KeyLog get_pressed_list() ;

    bool is_absorbed() noexcept ;
    void absorb() noexcept ;
    void unabsorb() noexcept ;

    void close_all_ports() noexcept ;
    void close_all_ports_with_refresh() ;

    void open_all_ports() noexcept ;
    void open_some_ports(const KeyLog::data_t& key) noexcept ;
    void open_port(const unsigned char key) noexcept ;

    //These functions is existed in order to fool KeyLogger as no-changing.
    //For example, EdiMoveCaretLeft...
    void release_virtually(const unsigned char key) noexcept ;
    void press_virtually(const unsigned char key) noexcept ;

    class InstantKeyAbsorber
    {
    private:
        struct Impl ;
        std::unique_ptr<Impl> pimpl ;

    public:
        explicit InstantKeyAbsorber() ;
        virtual ~InstantKeyAbsorber() noexcept ;

        InstantKeyAbsorber(const InstantKeyAbsorber&)            = delete ;
        InstantKeyAbsorber& operator=(const InstantKeyAbsorber&) = delete ;
        InstantKeyAbsorber(InstantKeyAbsorber&&)                 = delete ;
        InstantKeyAbsorber& operator=(InstantKeyAbsorber&&)         = delete ;
    } ;
}


#endif
