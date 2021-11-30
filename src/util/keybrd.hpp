#ifndef _KEYBRD_HPP
#define _KEYBRD_HPP

#include <windows.h>

#include "core/key_absorber.hpp"
#include "core/keycode_def.hpp"
#include "def.hpp"

#include <memory>
#include <stack>
#include <type_traits>

#if defined(DEBUG)
#include <iostream>
#endif

namespace vind
{
    namespace core
    {
        class KeyLog ;
    }

    namespace util {
        template <typename T>
        inline auto extended_key_flag(T key) noexcept {
            switch(key) {
                case KEYCODE_UP:
                case KEYCODE_DOWN:
                case KEYCODE_LEFT:
                case KEYCODE_RIGHT:
                case KEYCODE_HOME:
                case KEYCODE_END:
                case KEYCODE_PAGEUP:
                case KEYCODE_PAGEDOWN:
                case KEYCODE_INSERT:
                case KEYCODE_DELETE:
                    return KEYEVENTF_EXTENDEDKEY ;
                default:
                    return 0 ;
            }
        }

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

        template <typename Iterator>
        void pushup(Iterator begin, Iterator end) {
            using core::close_all_ports ;
            using core::open_port ;
            using core::open_some_ports ;

#if defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif
            static INPUT ins[6] = {
                {INPUT_KEYBOARD},
                {INPUT_KEYBOARD},
                {INPUT_KEYBOARD},
                {INPUT_KEYBOARD},
                {INPUT_KEYBOARD},
                {INPUT_KEYBOARD}
            } ;
#if defined(__gnuc__)
#pragma gcc diagnostic warning "-wmissing-field-initializers"
#endif

            const auto pre_state = core::get_pressed_list() ;

            //optimizing for 1
            switch(static_cast<std::size_t>(end - begin)) {
                case 1:
                    ins[0].ki.wVk         = static_cast<WORD>(*begin) ;
                    ins[0].ki.dwFlags     = extended_key_flag(*begin) ;
                    ins[0].ki.dwExtraInfo = GetMessageExtraInfo() ;

                    ins[1].ki.wVk         = static_cast<WORD>(*begin) ;
                    ins[1].ki.dwFlags     = KEYEVENTF_KEYUP | extended_key_flag(*begin) ;
                    ins[1].ki.dwExtraInfo = GetMessageExtraInfo() ;

                    open_port(*begin) ;

                    if(!SendInput(2, ins, sizeof(INPUT))) {
                        throw RUNTIME_EXCEPT("failed sending keyboard event") ;
                    }
                    break ;

                case 2:
                    ins[0].ki.wVk         = static_cast<WORD>(*begin) ;
                    ins[0].ki.dwFlags     = extended_key_flag(*begin) ;
                    ins[0].ki.dwExtraInfo = GetMessageExtraInfo() ;

                    ins[1].ki.wVk         = static_cast<WORD>(*(begin + 1)) ;
                    ins[1].ki.dwFlags     = extended_key_flag(*(begin + 1)) ;
                    ins[1].ki.dwExtraInfo = GetMessageExtraInfo() ;

                    ins[2].ki.wVk         = static_cast<WORD>(*(begin + 1)) ;
                    ins[2].ki.dwFlags     = KEYEVENTF_KEYUP | extended_key_flag(*(begin + 1));
                    ins[2].ki.dwExtraInfo = GetMessageExtraInfo() ;

                    ins[3].ki.wVk         = static_cast<WORD>(*begin) ;
                    ins[3].ki.dwFlags     = KEYEVENTF_KEYUP | extended_key_flag(*begin) ;
                    ins[3].ki.dwExtraInfo = GetMessageExtraInfo() ;

                    open_some_ports(begin, end) ;
                    if(!SendInput(4, ins, sizeof(INPUT))) {
                        throw RUNTIME_EXCEPT("failed sending keyboard event") ;
                    }
                    break ;

                case 3:
                    ins[0].ki.wVk         = static_cast<WORD>(*begin) ;
                    ins[0].ki.dwFlags     = extended_key_flag(*begin) ;
                    ins[0].ki.dwExtraInfo = GetMessageExtraInfo() ;

                    ins[1].ki.wVk         = static_cast<WORD>(*(begin + 1)) ;
                    ins[1].ki.dwFlags     = extended_key_flag(*(begin + 1)) ;
                    ins[1].ki.dwExtraInfo = GetMessageExtraInfo() ;

                    ins[2].ki.wVk         = static_cast<WORD>(*(begin + 2)) ;
                    ins[2].ki.dwFlags     = extended_key_flag(*(begin + 2)) ;
                    ins[2].ki.dwExtraInfo = GetMessageExtraInfo() ;

                    ins[3].ki.wVk         = static_cast<WORD>(*(begin + 2)) ;
                    ins[3].ki.dwFlags     = KEYEVENTF_KEYUP | extended_key_flag(*(begin + 2)) ;
                    ins[3].ki.dwExtraInfo = GetMessageExtraInfo() ;

                    ins[4].ki.wVk         = static_cast<WORD>(*(begin + 1)) ;
                    ins[4].ki.dwFlags     = KEYEVENTF_KEYUP | extended_key_flag(*(begin + 1)) ;
                    ins[4].ki.dwExtraInfo = GetMessageExtraInfo() ;

                    ins[5].ki.wVk         = static_cast<WORD>(*begin) ;
                    ins[5].ki.dwFlags     = KEYEVENTF_KEYUP | extended_key_flag(*begin);
                    ins[5].ki.dwExtraInfo = GetMessageExtraInfo() ;

                    open_some_ports(begin, end) ;
                    if(!SendInput(6, ins, sizeof(INPUT))) {
                        throw RUNTIME_EXCEPT("failed sending keyboard event") ;
                    }
                    break ;

                default: 
                    //>=4
                    using ScopedKeyStack = std::stack<ScopedKey, std::vector<ScopedKey>> ;
                    static ScopedKeyStack st ;

                    for(auto itr = begin ; itr != end ; itr ++) {
                        st.emplace(static_cast<KeyCode>(*itr)) ;
                        st.top().press() ;
                    }

                    ScopedKeyStack().swap(st) ; //clear
                    break ;
            }

            close_all_ports() ;

            for(auto key : (pre_state - core::get_pressed_list())) {
                press_keystate(key) ;
            }
        }

        inline void pushup(std::initializer_list<KeyCode>&& initlist) {
            pushup(initlist.begin(), initlist.end()) ;
        }

        inline void pushup(KeyCode key) {
            pushup({key}) ;
        }

        inline void pushup(KeyCode key1, KeyCode key2) {
            pushup({key1, key2}) ;
        }

        template <typename... Ts>
        inline void pushup(KeyCode key1, KeyCode key2, Ts&&... keys) {
            pushup({key1, key2, static_cast<KeyCode>(keys)...}) ;
        }
    }
}

#endif
