#ifndef _KEY_ABSORBER_HPP
#define _KEY_ABSORBER_HPP

#include <vector>
#include "key_log.hpp"
#include "keycode_def.hpp"
namespace vind
{
    namespace keyabsorber {
        void install_hook() ;

        void refresh_toggle_state() ;

        bool is_pressed(KeyCode keycode) noexcept ;
        bool is_really_pressed(KeyCode keycode) noexcept ;
        KeyLog get_pressed_list() ;

        bool is_absorbed() noexcept ;
        void absorb() noexcept ;
        void unabsorb() noexcept ;

        void close_all_ports() noexcept ;
        void close_all_ports_with_refresh() ;

        void open_some_ports(const KeyLog::Data& key) noexcept ;
        void open_port(KeyCode key) noexcept ;

        //These functions is existed in order to fool KeyLogger as no-changing.
        //For example, EdiMoveCaretLeft...
        void release_virtually(KeyCode key) noexcept ;
        void press_virtually(KeyCode key) noexcept ;

        class InstantKeyAbsorber {
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
}

#endif
