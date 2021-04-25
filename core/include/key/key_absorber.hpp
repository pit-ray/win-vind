#ifndef _KEY_ABSORBER_HPP
#define _KEY_ABSORBER_HPP

#include <vector>
#include "key_log.hpp"
#include "keycode_def.hpp"

namespace vind
{
    namespace keyabsorber {
        void install_hook() ;
        bool is_pressed(unsigned char keycode) noexcept ;
        bool is_really_pressed(unsigned char keycode) noexcept ;
        KeyLog get_pressed_list() ;

        bool is_absorbed() noexcept ;
        void absorb() noexcept ;
        void unabsorb() noexcept ;

        void close_all_ports() noexcept ;
        void close_all_ports_with_refresh() ;

        void open_some_ports(const KeyLog::Data& key) noexcept ;
        void open_port(unsigned char key) noexcept ;

        //These functions is existed in order to fool KeyLogger as no-changing.
        //For example, EdiMoveCaretLeft...
        void release_virtually(unsigned char key) noexcept ;
        void press_virtually(unsigned char key) noexcept ;

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
