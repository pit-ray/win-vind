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

        bool is_send_event(const bool pushed) noexcept ;

    public:
        explicit SmartKey(const unsigned char key) ;
        ~SmartKey() ;

        bool is_push() noexcept ;
        bool is_release() noexcept ;

        SmartKey(SmartKey&&) noexcept ;
        SmartKey& operator=(SmartKey&&) noexcept ;

        SmartKey(const SmartKey&) = delete ;
        SmartKey& operator=(const SmartKey&) = delete ;
    } ;


    //free function

    //change key state without input
    bool is_release_keystate(const unsigned char key) noexcept ;

    //change key state without input
    bool is_push_keystate(const unsigned char key) noexcept ;

    bool _is_pushup_core(std::initializer_list<unsigned char>&& initlist) ;

    //perfect forwarding
    template <typename... Ts>
    inline bool is_pushup(Ts&&... keys) {
        using namespace std ;
        auto initl = {std::forward<Ts>(keys)...} ;
        return _is_pushup_core(std::move(initl)) ;
    }
}

#endif