#ifndef _KEYBRD_HPP
#define _KEYBRD_HPP

#include <iostream>
#include <memory>
#include <stack>
#include <type_traits>

#include "key/key_absorber.hpp"
#include "key/keycode_def.hpp"

namespace vind
{
    class KeyLog ;

    namespace keybrd {
        bool is_pressed_actually(KeyCode key) noexcept ;

        class SmartKey {
        private:
            struct Impl ;
            std::unique_ptr<Impl> pimpl ;

            void send_event(bool pressed) ;

        public:
            explicit SmartKey(KeyCode key) ;
            virtual ~SmartKey() noexcept ;

            void press() ;
            void release() ;

            SmartKey(SmartKey&&) ;
            SmartKey& operator=(SmartKey&&) ;
            SmartKey(const SmartKey&)            = delete ;
            SmartKey& operator=(const SmartKey&) = delete ;
        } ;


        //change key state without input
        void release_keystate(KeyCode key) ;

        //change key state without input
        void press_keystate(KeyCode key) ;

        void pushup_core(std::initializer_list<KeyCode>&& initlist) ;

        //perfect forwarding
        template <typename... Ts>
        inline void pushup(Ts&&... keys) {
            auto initl = {std::forward<Ts>(keys)...} ;
            return pushup_core(std::move(initl)) ;
        }
    }
}

#endif
