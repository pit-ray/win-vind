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
        template <typename Iterator>
        void release_keystate(Iterator begin, Iterator end) {
#if defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif
            static INPUT ins[6] = {
                {INPUT_KEYBOARD},
                {INPUT_KEYBOARD},
                {INPUT_KEYBOARD},
            } ;
#if defined(__gnuc__)
#pragma gcc diagnostic warning "-wmissing-field-initializers"
#endif

            //optimizing for 1
            const auto size = static_cast<std::size_t>(end - begin) ;
            switch(size) {
                case 1:
                    ins[0].ki.wVk         = static_cast<WORD>(*begin) ;
                    ins[0].ki.wScan       = static_cast<WORD>(MapVirtualKeyA(*begin, MAPVK_VK_TO_VSC)) ;
                    ins[0].ki.dwFlags     = KEYEVENTF_KEYUP | extended_key_flag(*begin) ;
                    ins[0].ki.dwExtraInfo = GetMessageExtraInfo() ;

                    if(!SendInput(1, ins, sizeof(INPUT))) {
                        throw RUNTIME_EXCEPT("failed sending keyboard event") ;
                    }

                    core::release_virtually(*begin) ;
                    break ;

                case 2:
                    ins[0].ki.wVk         = static_cast<WORD>(*begin) ;
                    ins[0].ki.wScan       = static_cast<WORD>(MapVirtualKeyA(*begin, MAPVK_VK_TO_VSC)) ;
                    ins[0].ki.dwFlags     = KEYEVENTF_KEYUP | extended_key_flag(*begin) ;
                    ins[0].ki.dwExtraInfo = GetMessageExtraInfo() ;

                    ins[1].ki.wVk         = static_cast<WORD>(*(begin + 1)) ;
                    ins[1].ki.wScan       = static_cast<WORD>(MapVirtualKeyA(*(begin + 1), MAPVK_VK_TO_VSC)) ;
                    ins[1].ki.dwFlags     = KEYEVENTF_KEYUP | extended_key_flag(*(begin + 1)) ;
                    ins[1].ki.dwExtraInfo = GetMessageExtraInfo() ;

                    if(!SendInput(2, ins, sizeof(INPUT))) {
                        throw RUNTIME_EXCEPT("failed sending keyboard event") ;
                    }

                    core::release_virtually(*begin) ;
                    core::release_virtually(*(begin + 1)) ;
                    break ;

                case 3:
                    ins[0].ki.wVk         = static_cast<WORD>(*begin) ;
                    ins[0].ki.wScan       = static_cast<WORD>(MapVirtualKeyA(*begin, MAPVK_VK_TO_VSC)) ;
                    ins[0].ki.dwFlags     = KEYEVENTF_KEYUP | extended_key_flag(*begin) ;
                    ins[0].ki.dwExtraInfo = GetMessageExtraInfo() ;

                    ins[1].ki.wVk         = static_cast<WORD>(*(begin + 1)) ;
                    ins[1].ki.wScan       = static_cast<WORD>(MapVirtualKeyA(*(begin + 1), MAPVK_VK_TO_VSC)) ;
                    ins[1].ki.dwFlags     = KEYEVENTF_KEYUP | extended_key_flag(*(begin + 1)) ;
                    ins[1].ki.dwExtraInfo = GetMessageExtraInfo() ;

                    ins[2].ki.wVk         = static_cast<WORD>(*(begin + 2)) ;
                    ins[2].ki.wScan       = static_cast<WORD>(MapVirtualKeyA(*(begin + 2), MAPVK_VK_TO_VSC)) ;
                    ins[2].ki.dwFlags     = KEYEVENTF_KEYUP | extended_key_flag(*(begin + 2)) ;
                    ins[2].ki.dwExtraInfo = GetMessageExtraInfo() ;

                    if(!SendInput(3, ins, sizeof(INPUT))) {
                        throw RUNTIME_EXCEPT("failed sending keyboard event") ;
                    }

                    core::release_virtually(*begin) ;
                    core::release_virtually(*(begin + 1)) ;
                    core::release_virtually(*(begin + 2)) ;
                    break ;

                default:
                    std::unique_ptr<INPUT[]> dynamic_ins(new INPUT[size]) ;
                    for(std::size_t i = 0 ; i < size ; i ++) {
                        dynamic_ins[i].ki.wVk         = static_cast<WORD>(*(begin + i)) ;
                        dynamic_ins[i].ki.wScan       = static_cast<WORD>(MapVirtualKeyA(*(begin + i), MAPVK_VK_TO_VSC)) ;
                        dynamic_ins[i].ki.dwFlags     = KEYEVENTF_KEYUP | extended_key_flag(*(begin + i)) ;
                        dynamic_ins[i].ki.dwExtraInfo = GetMessageExtraInfo() ;

                        core::release_virtually(*(begin + 1)) ;
                    }

                    if(!SendInput(size, dynamic_ins.get(), sizeof(INPUT))) {
                        throw RUNTIME_EXCEPT("failed sending keyboard event") ;
                    }

                    break ;
            }
        }

        inline void release_keystate(std::initializer_list<KeyCode>&& initlist) {
            release_keystate(initlist.begin(), initlist.end()) ;
        }

        inline void release_keystate(KeyCode key) {
            release_keystate({key}) ;
        }

        inline void release_keystate(KeyCode key1, KeyCode key2) {
            release_keystate({key1, key2}) ;
        }

        template <typename... Ts>
        inline void release_keystate(KeyCode key1, KeyCode key2, Ts&&... keys) {
            release_keystate({key1, key2, static_cast<KeyCode>(keys)...}) ;
        }


        //change key state without input
        template <typename Iterator>
        void press_keystate(Iterator begin, Iterator end) {
#if defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif
            static INPUT ins[6] = {
                {INPUT_KEYBOARD},
                {INPUT_KEYBOARD},
                {INPUT_KEYBOARD},
            } ;
#if defined(__gnuc__)
#pragma gcc diagnostic warning "-wmissing-field-initializers"
#endif

            //optimizing for 1
            const auto size = static_cast<std::size_t>(end - begin) ;
            switch(size) {
                case 1:
                    ins[0].ki.wVk         = static_cast<WORD>(*begin) ;
                    ins[0].ki.wScan       = static_cast<WORD>(MapVirtualKeyA(*begin, MAPVK_VK_TO_VSC)) ;
                    ins[0].ki.dwFlags     = extended_key_flag(*begin) ;
                    ins[0].ki.dwExtraInfo = GetMessageExtraInfo() ;

                    if(!SendInput(1, ins, sizeof(INPUT))) {
                        throw RUNTIME_EXCEPT("failed sending keyboard event") ;
                    }

                    core::press_virtually(*begin) ;
                    break ;

                case 2:
                    ins[0].ki.wVk         = static_cast<WORD>(*begin) ;
                    ins[0].ki.wScan       = static_cast<WORD>(MapVirtualKeyA(*begin, MAPVK_VK_TO_VSC)) ;
                    ins[0].ki.dwFlags     = extended_key_flag(*begin) ;
                    ins[0].ki.dwExtraInfo = GetMessageExtraInfo() ;

                    ins[1].ki.wVk         = static_cast<WORD>(*(begin + 1)) ;
                    ins[1].ki.wScan       = static_cast<WORD>(MapVirtualKeyA(*(begin + 1), MAPVK_VK_TO_VSC)) ;
                    ins[1].ki.dwFlags     = extended_key_flag(*(begin + 1)) ;
                    ins[1].ki.dwExtraInfo = GetMessageExtraInfo() ;

                    if(!SendInput(2, ins, sizeof(INPUT))) {
                        throw RUNTIME_EXCEPT("failed sending keyboard event") ;
                    }

                    core::press_virtually(*begin) ;
                    core::press_virtually(*(begin + 1)) ;
                    break ;

                case 3:
                    ins[0].ki.wVk         = static_cast<WORD>(*begin) ;
                    ins[0].ki.wScan       = static_cast<WORD>(MapVirtualKeyA(*begin, MAPVK_VK_TO_VSC)) ;
                    ins[0].ki.dwFlags     = extended_key_flag(*begin) ;
                    ins[0].ki.dwExtraInfo = GetMessageExtraInfo() ;

                    ins[1].ki.wVk         = static_cast<WORD>(*(begin + 1)) ;
                    ins[1].ki.wScan       = static_cast<WORD>(MapVirtualKeyA(*(begin + 1), MAPVK_VK_TO_VSC)) ;
                    ins[1].ki.dwFlags     = extended_key_flag(*(begin + 1)) ;
                    ins[1].ki.dwExtraInfo = GetMessageExtraInfo() ;

                    ins[2].ki.wVk         = static_cast<WORD>(*(begin + 2)) ;
                    ins[2].ki.wScan       = static_cast<WORD>(MapVirtualKeyA(*(begin + 2), MAPVK_VK_TO_VSC)) ;
                    ins[2].ki.dwFlags     = extended_key_flag(*(begin + 2)) ;
                    ins[2].ki.dwExtraInfo = GetMessageExtraInfo() ;

                    if(!SendInput(3, ins, sizeof(INPUT))) {
                        throw RUNTIME_EXCEPT("failed sending keyboard event") ;
                    }

                    core::press_virtually(*begin) ;
                    core::press_virtually(*(begin + 1)) ;
                    core::press_virtually(*(begin + 2)) ;
                    break ;

                default:
                    std::unique_ptr<INPUT[]> dynamic_ins(new INPUT[size]) ;
                    for(std::size_t i = 0 ; i < size ; i ++) {
                        auto key = *(begin + 1) ;
                        dynamic_ins[i].ki.wVk         = static_cast<WORD>(key) ;
                        dynamic_ins[i].ki.wScan       = static_cast<WORD>(MapVirtualKeyA(key, MAPVK_VK_TO_VSC)) ;
                        dynamic_ins[i].ki.dwFlags     = extended_key_flag(key) ;
                        dynamic_ins[i].ki.dwExtraInfo = GetMessageExtraInfo() ;

                        core::press_virtually(key) ;
                    }

                    if(!SendInput(size, dynamic_ins.get(), sizeof(INPUT))) {
                        throw RUNTIME_EXCEPT("failed sending keyboard event") ;
                    }

                    break ;
            }
        }

        inline void press_keystate(std::initializer_list<KeyCode>&& initlist) {
            press_keystate(initlist.begin(), initlist.end()) ;
        }

        inline void press_keystate(KeyCode key) {
            press_keystate({key}) ;
        }

        inline void press_keystate(KeyCode key1, KeyCode key2) {
            press_keystate({key1, key2}) ;
        }

        template <typename... Ts>
        inline void press_keystate(bool vhook, KeyCode key1, KeyCode key2, Ts&&... keys) {
            press_keystate({key1, key2, static_cast<KeyCode>(keys)...}) ;
        }


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
