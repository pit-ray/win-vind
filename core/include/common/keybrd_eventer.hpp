#ifndef _KEYBRD_EVENTER_HPP
#define _KEYBRD_EVENTER_HPP

#include <memory>
#include <type_traits>
#include <stack>
#include <iostream>

#include "virtual_key_fwd.hpp"
#include "key_absorber.hpp"

class KeyLog ;

namespace KeybrdEventer
{
    class SmartKey {
    private:
        struct Impl ;
        std::unique_ptr<Impl> pimpl ;

        void send_event(const bool pressed) ;

    public:
        explicit SmartKey(const unsigned char key) ;
        virtual ~SmartKey() noexcept ;

        void press() ;
        void release() ;

        SmartKey(SmartKey&&) ;
        SmartKey& operator=(SmartKey&&) ;
        SmartKey(const SmartKey&)            = delete ;
        SmartKey& operator=(const SmartKey&) = delete ;
    } ;


    //free function

    //change key state without input
    void release_keystate(const unsigned char key) ;

    //change key state without input
    void press_keystate(const unsigned char key) ;

    void _pushup_core(std::initializer_list<unsigned char>&& initlist) ;

    //perfect forwarding
    template <typename... Ts>
    inline void pushup(Ts&&... keys) {
        auto initl = {std::forward<Ts>(keys)...} ;
        return _pushup_core(std::move(initl)) ;
    }
}

#endif
