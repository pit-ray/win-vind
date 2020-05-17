#ifndef _KEYBRD_EVENTER_HPP
#define _KEYBRD_EVENTER_HPP

#include <memory>
#include <type_traits>
#include <stack>
#include "virtual_key_fwd.hpp"
#include <iostream>

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

        SmartKey(SmartKey&&) ;
        SmartKey& operator=(SmartKey&&) ;

        SmartKey(const SmartKey&) = delete ;
        SmartKey& operator=(const SmartKey&) = delete ;
    } ;


    //free function

    //perfect forwarding
    template <typename... Ts>
    inline bool is_pushup(Ts&&... keys) {
        using namespace std ;
        const auto initl = {std::forward<Ts>(keys)...} ;

        if(initl.size() == 1) {
            return std::make_unique<SmartKey>(
                static_cast<unsigned char>(*initl.begin()) //initialize
            )->is_push() ;
        }

        std::stack<std::unique_ptr<SmartKey>> st ;
        const auto clear_stack = [&st] {
            while(!st.empty()) {
                st.pop() ;
            }
        } ;

        for(auto iter = initl.begin() ; iter != initl.end() ; iter ++) {
            const auto key = static_cast<unsigned char>(*iter) ;
            st.push(std::make_unique<SmartKey>(key)) ;
            if(!st.top()->is_push()) {
                clear_stack() ;
                return false ;
            }
        }

        clear_stack() ;
        return true ;
    }

    //change key state without input
    bool is_release_keystate(const unsigned char key) noexcept ;

    //change key state without input
    bool is_push_keystate(const unsigned char key) noexcept ;


    bool is_sync_push(const unsigned char bindkey, const unsigned char funckey) ;

    void update_sync_push() noexcept ;
}

#endif