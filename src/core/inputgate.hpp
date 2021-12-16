#ifndef _INPUTGATE_HPP
#define _INPUTGATE_HPP

#include "defs.hpp"
#include "key_log.hpp"
#include "keycode_def.hpp"
#include "mode.hpp"

#include "util/def.hpp"

#include <windows.h>

#include <initializer_list>
#include <memory>
#include <vector>

namespace vind
{
    namespace core
    {
        template <typename T>
        auto extended_key_flag(T key) noexcept {
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

        class InputGate {
        private:
            struct Impl ;
            std::unique_ptr<Impl> pimpl ;

            explicit InputGate() ;
            virtual ~InputGate() noexcept ;

            static LRESULT CALLBACK hook_proc(
                    int n_code,
                    WPARAM w_param,
                    LPARAM l_param) ;

        public:
            static InputGate& get_instance() ;

            void install_hook() ;

            void uninstall_hook() noexcept ;

            void reconstruct() ;

            void refresh_toggle_state() ;

            bool is_pressed(KeyCode keycode) noexcept ;
            template <typename... Args>
            inline bool is_pressed(
                    KeyCode keycode,
                    Args&&... keys) noexcept {
                return is_pressed(keycode) && is_pressed(std::forward<Args>(keys)...) ;
            }

            bool is_really_pressed(KeyCode keycode) noexcept ;
            template <typename... Args>
            inline bool is_really_pressed(
                    KeyCode keycode,
                    Args&&... keys) noexcept {
                return is_really_pressed(keycode) && is_really_pressed(std::forward<Args>(keys)...) ;
            }

            KeyLog pressed_list() ;

            bool is_absorbed() noexcept ;
            void absorb() noexcept ;
            void unabsorb() noexcept ;

            void close_some_ports(
                    std::initializer_list<KeyCode>&& keys) noexcept ;
            void close_some_ports(
                    std::initializer_list<KeyCode>::const_iterator begin,
                    std::initializer_list<KeyCode>::const_iterator end) noexcept ;

            void close_some_ports(
                    std::vector<KeyCode>&& keys) noexcept ;
            void close_some_ports(
                    KeySet::const_iterator begin,
                    KeySet::const_iterator end) noexcept ;

            void close_some_ports(const KeyLog::Data& key) noexcept ;
            void close_port(KeyCode key) noexcept ;

            void close_all_ports() noexcept ;
            void close_all_ports_with_refresh() ;

            void open_some_ports(
                    std::initializer_list<KeyCode>&& keys) noexcept ;
            void open_some_ports(
                    std::initializer_list<KeyCode>::const_iterator begin,
                    std::initializer_list<KeyCode>::const_iterator end) noexcept ;

            void open_some_ports(std::vector<KeyCode>&& keys) noexcept ;
            void open_some_ports(
                    KeySet::const_iterator begin,
                    KeySet::const_iterator end) noexcept ;

            void open_some_ports(const KeyLog::Data& key) noexcept ;
            void open_port(KeyCode key) noexcept ;

            //These functions is existed in order to fool KeyLogger as no-changing.
            //For example, MoveCaretLeft...
            void release_virtually(KeyCode key) noexcept ;
            void press_virtually(KeyCode key) noexcept ;

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

                        release_virtually(*begin) ;
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

                        release_virtually(*begin) ;
                        release_virtually(*(begin + 1)) ;
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

                        release_virtually(*begin) ;
                        release_virtually(*(begin + 1)) ;
                        release_virtually(*(begin + 2)) ;
                        break ;

                    default:
                        std::unique_ptr<INPUT[]> dynamic_ins(new INPUT[size]) ;
                        for(std::size_t i = 0 ; i < size ; i ++) {
                            dynamic_ins[i].ki.wVk         = static_cast<WORD>(*(begin + i)) ;
                            dynamic_ins[i].ki.wScan       = static_cast<WORD>(MapVirtualKeyA(*(begin + i), MAPVK_VK_TO_VSC)) ;
                            dynamic_ins[i].ki.dwFlags     = KEYEVENTF_KEYUP | extended_key_flag(*(begin + i)) ;
                            dynamic_ins[i].ki.dwExtraInfo = GetMessageExtraInfo() ;

                            release_virtually(*(begin + 1)) ;
                        }

                        if(!SendInput(static_cast<UINT>(size), dynamic_ins.get(), sizeof(INPUT))) {
                            throw RUNTIME_EXCEPT("failed sending keyboard event") ;
                        }

                        break ;
                }
            }

            void release_keystate(
                    std::initializer_list<KeyCode>&& initlist) {
                release_keystate(initlist.begin(), initlist.end()) ;
            }

            void release_keystate(KeyCode key) {
                release_keystate({key}) ;
            }

            void release_keystate(KeyCode key1, KeyCode key2) {
                release_keystate({key1, key2}) ;
            }

            template <typename... Ts>
            void release_keystate(
                    KeyCode key1,
                    KeyCode key2,
                    Ts&&... keys) {
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

                        press_virtually(*begin) ;
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

                        press_virtually(*begin) ;
                        press_virtually(*(begin + 1)) ;
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

                        press_virtually(*begin) ;
                        press_virtually(*(begin + 1)) ;
                        press_virtually(*(begin + 2)) ;
                        break ;

                    default:
                        std::unique_ptr<INPUT[]> dynamic_ins(new INPUT[size]) ;
                        for(std::size_t i = 0 ; i < size ; i ++) {
                            auto key = *(begin + 1) ;
                            dynamic_ins[i].ki.wVk         = static_cast<WORD>(key) ;
                            dynamic_ins[i].ki.wScan       = static_cast<WORD>(MapVirtualKeyA(key, MAPVK_VK_TO_VSC)) ;
                            dynamic_ins[i].ki.dwFlags     = extended_key_flag(key) ;
                            dynamic_ins[i].ki.dwExtraInfo = GetMessageExtraInfo() ;

                            press_virtually(key) ;
                        }

                        if(!SendInput(static_cast<UINT>(size), dynamic_ins.get(), sizeof(INPUT))) {
                            throw RUNTIME_EXCEPT("failed sending keyboard event") ;
                        }

                        break ;
                }
            }

            void press_keystate(
                    std::initializer_list<KeyCode>&& initlist) {
                press_keystate(initlist.begin(), initlist.end()) ;
            }

            void press_keystate(KeyCode key) {
                press_keystate({key}) ;
            }

            void press_keystate(KeyCode key1, KeyCode key2) {
                press_keystate({key1, key2}) ;
            }

            template <typename... Ts>
            void press_keystate(
                    bool vhook,
                    KeyCode key1,
                    KeyCode key2,
                    Ts&&... keys) {
                press_keystate({key1, key2, static_cast<KeyCode>(keys)...}) ;
            }


            template <typename Iterator>
            void pushup(Iterator begin, Iterator end) {
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

                const auto pre_state = pressed_list() ;

                auto size = static_cast<std::size_t>(end - begin) ;

                //optimizing for 1
                switch(size) {
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
                        std::unique_ptr<INPUT[]> dynamic_ins(new INPUT[size * 2]) ;
                        for(std::size_t i = 0 ; i < size ; i ++) {
                            auto key = *(begin + 1) ;
                            dynamic_ins[i].ki.wVk         = static_cast<WORD>(key) ;
                            dynamic_ins[i].ki.wScan       = static_cast<WORD>(MapVirtualKeyA(key, MAPVK_VK_TO_VSC)) ;
                            dynamic_ins[i].ki.dwFlags     = extended_key_flag(key) ;
                            dynamic_ins[i].ki.dwExtraInfo = GetMessageExtraInfo() ;

                            dynamic_ins[size - i - 1].ki.wVk         = static_cast<WORD>(key) ;
                            dynamic_ins[size - i - 1].ki.wScan       = static_cast<WORD>(MapVirtualKeyA(key, MAPVK_VK_TO_VSC)) ;
                            dynamic_ins[size - i - 1].ki.dwFlags     = KEYEVENTF_KEYUP | extended_key_flag(key) ;
                            dynamic_ins[size - i - 1].ki.dwExtraInfo = GetMessageExtraInfo() ;
                        }

                        if(!SendInput(static_cast<UINT>(size), dynamic_ins.get(), sizeof(INPUT))) {
                            throw RUNTIME_EXCEPT("failed sending keyboard event") ;
                        }

                        break ;
                }

                close_all_ports() ;

                for(auto key : (pre_state - pressed_list())) {
                    press_keystate(key) ;
                }
            }

            void pushup(std::initializer_list<KeyCode>&& initlist) {
                pushup(initlist.begin(), initlist.end()) ;
            }

            void pushup(KeyCode key) {
                pushup({key}) ;
            }

            void pushup(KeyCode key1, KeyCode key2) {
                pushup({key1, key2}) ;
            }

            template <typename... Ts>
            void pushup(
                    KeyCode key1,
                    KeyCode key2,
                    Ts&&... keys) {
                pushup({key1, key2, static_cast<KeyCode>(keys)...}) ;
            }


            /**
             * NOTE: This function samples a log from the log pool based on the key mapping
             * instead of getting the key status obtained from the hook.
             */
            KeyLog pop_log(Mode mode=get_global_mode()) ;


            /**
             * NOTE: map_logger is a mapping from NTypeLogger to NTypeLogger.
             * What it means is that it is excited by a command, and return the log pool.
             * it would have been in if the corresponding command had been entered.
             *
             * In the case of noremap, the key message is not actually generated,
             * but in the case of map, the key message is generated in a state that is passed to Windows.
             */
            std::vector<KeyLog> map_logger(
                    const KeyLog& log,
                    Mode mode=get_global_mode()) ;

            /**
             * A gate uses to synchronize the state of a key at low-level
             * with mapped to the hook_key. It return ture if the map was done,
             * false if the map does not exist.
             * It works like the following, with the keys connected by bars.
             *
             *     _____________________________________
             *   _|_                        _|_        _|_
             *  /   \                      /   \      /   \
             * /_____\                    /_____\    /_____\
             * hook_key        keyset = {   key1  ,    key2   }
             *
             */
            bool map_syncstate(
                    KeyCode hook_key,
                    bool press_sync_state,
                    Mode mode=get_global_mode()) ;
        } ;


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


        class InstantKeyAbsorber {
        private:
            bool flag_ ;

        public:
            explicit InstantKeyAbsorber() ;
            virtual ~InstantKeyAbsorber() noexcept ;

            InstantKeyAbsorber(const InstantKeyAbsorber&) = delete ;
            InstantKeyAbsorber& operator=(const InstantKeyAbsorber&) = delete ;
            InstantKeyAbsorber(InstantKeyAbsorber&&) = delete ;
            InstantKeyAbsorber& operator=(InstantKeyAbsorber&&) = delete ;
        } ;
    }
}

#endif
