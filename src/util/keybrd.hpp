#ifndef _KEYBRD_HPP
#define _KEYBRD_HPP

#include <memory>
#include <stack>
#include <type_traits>

#include "core/key_absorber.hpp"
#include "core/keycode_def.hpp"

#if defined(DEBUG)
#include <iostream>
#endif

namespace vind
{
    class KeyLog ;

    namespace util {
        bool is_pressed_actually(KeyCode key) noexcept ;

        class ScopedKey {
        private:
            struct Impl ;
            std::unique_ptr<Impl> pimpl ;

            void send_event(bool pressed) ;

        public:
            explicit ScopedKey(KeyCode key) ;
            virtual ~ScopedKey() noexcept ;

            void press() ;
            void release() ;

            ScopedKey(ScopedKey&&) ;
            ScopedKey& operator=(ScopedKey&&) ;
            ScopedKey(const ScopedKey&)            = delete ;
            ScopedKey& operator=(const ScopedKey&) = delete ;
        } ;


        //change key state without input
        void release_keystate(
                KeyCode key,
                bool enable_vhook=true) ;

        //change key state without input
        void press_keystate(
                KeyCode key,
                bool enable_vhook=true) ;

        void pushup_core(std::initializer_list<KeyCode>&& initlist) ;

        //perfect forwarding
        template <typename... Ts>
        inline void pushup(Ts&&... keys) {
            return pushup_core({static_cast<KeyCode>(keys)...}) ;
        }
    }
}

#endif
