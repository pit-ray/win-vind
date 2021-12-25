#ifndef _INPUTGATE_HPP
#define _INPUTGATE_HPP

#include "key_log.hpp"
#include "keycode.hpp"
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
            switch(static_cast<unsigned char>(key)) {
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
                const auto size = static_cast<std::size_t>(end - begin) ;
                switch(size) {
                    case 1: {
#if defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif
                        INPUT in = {INPUT_KEYBOARD} ;
#if defined(__GNUC__)
#pragma gcc diagnostic warning "-wmissing-field-initializers"
#endif
                        in.ki.wVk         = static_cast<WORD>(begin->to_code()) ;
                        in.ki.wScan       = static_cast<WORD>(MapVirtualKeyA(in.ki.wVk, MAPVK_VK_TO_VSC)) ;
                        in.ki.dwFlags     = KEYEVENTF_KEYUP | extended_key_flag(in.ki.wVk) ;
                        in.ki.dwExtraInfo = GetMessageExtraInfo() ;

                        if(!SendInput(1, &in, sizeof(INPUT))) {
                            throw RUNTIME_EXCEPT("failed sending keyboard event") ;
                        }

                        release_virtually(*begin) ;
                        break ;
                    }
                    case 2: {
#if defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif
                        INPUT ins[] = {
                            {INPUT_KEYBOARD},
                            {INPUT_KEYBOARD}
                        } ;
#if defined(__GNUC__)
#pragma gcc diagnostic warning "-wmissing-field-initializers"
#endif
                        ins[0].ki.wVk         = static_cast<WORD>(begin->to_code()) ;
                        ins[0].ki.wScan       = static_cast<WORD>(MapVirtualKeyA(ins[0].ki.wVk, MAPVK_VK_TO_VSC)) ;
                        ins[0].ki.dwFlags     = KEYEVENTF_KEYUP | extended_key_flag(ins[0].ki.wVk) ;
                        ins[0].ki.dwExtraInfo = GetMessageExtraInfo() ;

                        ins[1].ki.wVk         = static_cast<WORD>((begin + 1)->to_code()) ;
                        ins[1].ki.wScan       = static_cast<WORD>(MapVirtualKeyA(ins[1].ki.wVk, MAPVK_VK_TO_VSC)) ;
                        ins[1].ki.dwFlags     = KEYEVENTF_KEYUP | extended_key_flag(ins[1].ki.wVk) ;
                        ins[1].ki.dwExtraInfo = GetMessageExtraInfo() ;

                        if(!SendInput(2, ins, sizeof(INPUT))) {
                            throw RUNTIME_EXCEPT("failed sending keyboard event") ;
                        }

                        release_virtually(*begin) ;
                        release_virtually(*(begin + 1)) ;
                        break ;
                    }

                    case 3: {
#if defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif
                        INPUT ins[] = {
                            {INPUT_KEYBOARD},
                            {INPUT_KEYBOARD},
                            {INPUT_KEYBOARD}
                        } ;
#if defined(__GNUC__)
#pragma gcc diagnostic warning "-wmissing-field-initializers"
#endif
                        ins[0].ki.wVk         = static_cast<WORD>(begin->to_code()) ;
                        ins[0].ki.wScan       = static_cast<WORD>(MapVirtualKeyA(ins[0].ki.wVk, MAPVK_VK_TO_VSC)) ;
                        ins[0].ki.dwFlags     = KEYEVENTF_KEYUP | extended_key_flag(ins[0].ki.wVk) ;
                        ins[0].ki.dwExtraInfo = GetMessageExtraInfo() ;

                        ins[1].ki.wVk         = static_cast<WORD>((begin + 1)->to_code()) ;
                        ins[1].ki.wScan       = static_cast<WORD>(MapVirtualKeyA(ins[1].ki.wVk, MAPVK_VK_TO_VSC)) ;
                        ins[1].ki.dwFlags     = KEYEVENTF_KEYUP | extended_key_flag(ins[1].ki.wVk) ;
                        ins[1].ki.dwExtraInfo = GetMessageExtraInfo() ;

                        ins[2].ki.wVk         = static_cast<WORD>((begin + 2)->to_code()) ;
                        ins[2].ki.wScan       = static_cast<WORD>(MapVirtualKeyA(ins[2].ki.wVk, MAPVK_VK_TO_VSC)) ;
                        ins[2].ki.dwFlags     = KEYEVENTF_KEYUP | extended_key_flag(ins[2].ki.wVk) ;
                        ins[2].ki.dwExtraInfo = GetMessageExtraInfo() ;

                        if(!SendInput(3, ins, sizeof(INPUT))) {
                            throw RUNTIME_EXCEPT("failed sending keyboard event") ;
                        }

                        release_virtually(*begin) ;
                        release_virtually(*(begin + 1)) ;
                        release_virtually(*(begin + 2)) ;
                        break ;
                    }

                    default: {
                        std::unique_ptr<INPUT[]> dins(new INPUT[size]) ;
                        for(std::size_t i = 0 ; i < size ; i ++) {
                            dins[i].ki.wVk         = static_cast<WORD>((begin + i)->to_code()) ;
                            dins[i].ki.wScan       = static_cast<WORD>(MapVirtualKeyA(dins[i].ki.wVk, MAPVK_VK_TO_VSC)) ;
                            dins[i].ki.dwFlags     = KEYEVENTF_KEYUP | extended_key_flag(dins[i].ki.wVk) ;
                            dins[i].ki.dwExtraInfo = GetMessageExtraInfo() ;

                            release_virtually(*(begin + 1)) ;
                        }

                        if(!SendInput(static_cast<UINT>(size), dins.get(), sizeof(INPUT))) {
                            throw RUNTIME_EXCEPT("failed sending keyboard event") ;
                        }

                        break ;
                    }
                }
            }

            void release_keystate(
                    std::initializer_list<KeyCode>&& initlist) {
                release_keystate(initlist.begin(), initlist.end()) ;
            }

            void release_keystate(KeyCode key) {
                release_keystate({key}) ;
            }

            void release_keystate(unsigned char key1, unsigned char key2) {
                release_keystate({KeyCode(key1), KeyCode(key2)}) ;
            }

            void release_keystate(unsigned char key1, KeyCode key2) {
                release_keystate({KeyCode(key1), key2}) ;
            }

            void release_keystate(KeyCode key1, unsigned char key2) {
                release_keystate({key1, KeyCode(key2)}) ;
            }

            void release_keystate(KeyCode key1, KeyCode key2) {
                release_keystate({key1, key2}) ;
            }

            template <typename... Ts>
            void release_keystate(
                    KeyCode key1,
                    KeyCode key2,
                    Ts&&... keys) {
                release_keystate({key1, key2, KeyCode(keys)...}) ;
            }


            //change key state without input
            template <typename Iterator>
            void press_keystate(Iterator begin, Iterator end) {
                //optimizing for 1
                const auto size = static_cast<std::size_t>(end - begin) ;
                switch(size) {
                    case 1: {
#if defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif
                        INPUT in = {INPUT_KEYBOARD} ;
#if defined(__GNUC__)
#pragma gcc diagnostic warning "-wmissing-field-initializers"
#endif
                        in.ki.wVk         = static_cast<WORD>(begin->to_code()) ;
                        in.ki.wScan       = static_cast<WORD>(MapVirtualKeyA(in.ki.wVk, MAPVK_VK_TO_VSC)) ;
                        in.ki.dwFlags     = extended_key_flag(in.ki.wVk) ;
                        in.ki.dwExtraInfo = GetMessageExtraInfo() ;

                        if(!SendInput(1, &in, sizeof(INPUT))) {
                            throw RUNTIME_EXCEPT("failed sending keyboard event") ;
                        }

                        press_virtually(*begin) ;
                        break ;
                    }
                    case 2: {
#if defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif
                        INPUT ins[] = {
                            {INPUT_KEYBOARD},
                            {INPUT_KEYBOARD}
                        } ;
#if defined(__GNUC__)
#pragma gcc diagnostic warning "-wmissing-field-initializers"
#endif

                        ins[0].ki.wVk         = static_cast<WORD>(begin->to_code()) ;
                        ins[0].ki.wScan       = static_cast<WORD>(MapVirtualKeyA(ins[0].ki.wVk, MAPVK_VK_TO_VSC)) ;
                        ins[0].ki.dwFlags     = extended_key_flag(ins[0].ki.wVk) ;
                        ins[0].ki.dwExtraInfo = GetMessageExtraInfo() ;

                        ins[1].ki.wVk         = static_cast<WORD>((begin + 1)->to_code()) ;
                        ins[1].ki.wScan       = static_cast<WORD>(MapVirtualKeyA(ins[1].ki.wVk, MAPVK_VK_TO_VSC)) ;
                        ins[1].ki.dwFlags     = extended_key_flag(ins[1].ki.wVk) ;
                        ins[1].ki.dwExtraInfo = GetMessageExtraInfo() ;

                        if(!SendInput(2, ins, sizeof(INPUT))) {
                            throw RUNTIME_EXCEPT("failed sending keyboard event") ;
                        }

                        press_virtually(*begin) ;
                        press_virtually(*(begin + 1)) ;
                        break ;
                    }
                    case 3: {
#if defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif
                        INPUT ins[] = {
                            {INPUT_KEYBOARD},
                            {INPUT_KEYBOARD},
                            {INPUT_KEYBOARD}
                        } ;
#if defined(__GNUC__)
#pragma gcc diagnostic warning "-wmissing-field-initializers"
#endif

                        ins[0].ki.wVk         = static_cast<WORD>(begin->to_code()) ;
                        ins[0].ki.wScan       = static_cast<WORD>(MapVirtualKeyA(ins[0].ki.wVk, MAPVK_VK_TO_VSC)) ;
                        ins[0].ki.dwFlags     = extended_key_flag(*begin) ;
                        ins[0].ki.dwExtraInfo = GetMessageExtraInfo() ;

                        ins[1].ki.wVk         = static_cast<WORD>((begin + 1)->to_code()) ;
                        ins[1].ki.wScan       = static_cast<WORD>(MapVirtualKeyA(ins[1].ki.wVk, MAPVK_VK_TO_VSC)) ;
                        ins[1].ki.dwFlags     = extended_key_flag(ins[1].ki.wVk) ;
                        ins[1].ki.dwExtraInfo = GetMessageExtraInfo() ;

                        ins[2].ki.wVk         = static_cast<WORD>((begin + 2)->to_code()) ;
                        ins[2].ki.wScan       = static_cast<WORD>(MapVirtualKeyA(ins[2].ki.wVk, MAPVK_VK_TO_VSC)) ;
                        ins[2].ki.dwFlags     = extended_key_flag(ins[2].ki.wVk) ;
                        ins[2].ki.dwExtraInfo = GetMessageExtraInfo() ;

                        if(!SendInput(3, ins, sizeof(INPUT))) {
                            throw RUNTIME_EXCEPT("failed sending keyboard event") ;
                        }

                        press_virtually(*begin) ;
                        press_virtually(*(begin + 1)) ;
                        press_virtually(*(begin + 2)) ;
                        break ;
                    }
                    default: {
                        std::unique_ptr<INPUT[]> dins(new INPUT[size]) ;
                        for(std::size_t i = 0 ; i < size ; i ++) {
                            dins[i].ki.wVk         = static_cast<WORD>((begin + i)->to_code()) ;
                            dins[i].ki.wScan       = static_cast<WORD>(MapVirtualKeyA(dins[i].ki.wVk, MAPVK_VK_TO_VSC)) ;
                            dins[i].ki.dwFlags     = extended_key_flag(dins[i].ki.wVk) ;
                            dins[i].ki.dwExtraInfo = GetMessageExtraInfo() ;

                            press_virtually(*(begin + i)) ;
                        }

                        if(!SendInput(static_cast<UINT>(size), dins.get(), sizeof(INPUT))) {
                            throw RUNTIME_EXCEPT("failed sending keyboard event") ;
                        }

                        break ;
                     }
                }
            }

            void press_keystate(
                    std::initializer_list<KeyCode>&& initlist) {
                press_keystate(initlist.begin(), initlist.end()) ;
            }

            void press_keystate(KeyCode key) {
                press_keystate({key}) ;
            }

            void press_keystate(unsigned char key1, unsigned char key2) {
                press_keystate({KeyCode(key1), KeyCode(key2)}) ;
            }

            void press_keystate(unsigned char key1, KeyCode key2) {
                press_keystate({KeyCode(key1), key2}) ;
            }

            void press_keystate(KeyCode key1, unsigned char key2) {
                press_keystate({key1, KeyCode(key2)}) ;
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
                press_keystate({key1, key2, KeyCode(keys)...}) ;
            }


            template <typename Iterator>
            void pushup(Iterator begin, Iterator end) {
                const auto pre_state = pressed_list() ;

                auto size = static_cast<std::size_t>(end - begin) ;

                //optimizing for 1
                switch(size) {
                    case 1: {
#if defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif
                        INPUT ins[] = {
                            {INPUT_KEYBOARD},
                            {INPUT_KEYBOARD}
                        } ;
#if defined(__GNUC__)
#pragma gcc diagnostic warning "-wmissing-field-initializers"
#endif

                        ins[0].ki.wVk         = static_cast<WORD>(begin->to_code()) ;
                        ins[0].ki.wScan       = static_cast<WORD>(MapVirtualKeyA(ins[0].ki.wVk, MAPVK_VK_TO_VSC)) ;
                        ins[0].ki.dwFlags     = extended_key_flag(ins[0].ki.wVk) ;
                        ins[0].ki.dwExtraInfo = GetMessageExtraInfo() ;

                        ins[1].ki.wVk         = ins[0].ki.wVk ;
                        ins[1].ki.wScan       = ins[0].ki.wScan ;
                        ins[1].ki.dwFlags     = ins[0].ki.dwFlags | KEYEVENTF_KEYUP ;
                        ins[1].ki.dwExtraInfo = GetMessageExtraInfo() ;

                        open_port(*begin) ;

                        if(!SendInput(2, ins, sizeof(INPUT))) {
                            throw RUNTIME_EXCEPT("failed sending keyboard event") ;
                        }
                        break ;
                    }
                    case 2: {
#if defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif
                        INPUT ins[] = {
                            {INPUT_KEYBOARD},
                            {INPUT_KEYBOARD},
                            {INPUT_KEYBOARD},
                            {INPUT_KEYBOARD}
                        } ;
#if defined(__GNUC__)
#pragma gcc diagnostic warning "-wmissing-field-initializers"
#endif

                        ins[0].ki.wVk         = static_cast<WORD>(begin->to_code()) ;
                        ins[0].ki.wScan       = static_cast<WORD>(MapVirtualKeyA(ins[0].ki.wVk, MAPVK_VK_TO_VSC)) ;
                        ins[0].ki.dwFlags     = extended_key_flag(ins[0].ki.wVk) ;
                        ins[0].ki.dwExtraInfo = GetMessageExtraInfo() ;

                        ins[1].ki.wVk         = static_cast<WORD>((begin + 1)->to_code()) ;
                        ins[1].ki.wScan       = static_cast<WORD>(MapVirtualKeyA(ins[1].ki.wVk, MAPVK_VK_TO_VSC)) ;
                        ins[1].ki.dwFlags     = extended_key_flag(ins[1].ki.wVk) ;
                        ins[1].ki.dwExtraInfo = GetMessageExtraInfo() ;

                        ins[2].ki.wVk         = ins[1].ki.wVk ;
                        ins[2].ki.wScan       = ins[1].ki.wScan ;
                        ins[2].ki.dwFlags     = ins[1].ki.dwFlags | KEYEVENTF_KEYUP ;
                        ins[2].ki.dwExtraInfo = GetMessageExtraInfo() ;

                        ins[3].ki.wVk         = ins[0].ki.wVk ;
                        ins[3].ki.wScan       = ins[0].ki.wScan ;
                        ins[3].ki.dwFlags     = ins[0].ki.dwFlags | KEYEVENTF_KEYUP ;
                        ins[3].ki.dwExtraInfo = GetMessageExtraInfo() ;

                        open_some_ports(begin, end) ;
                        if(!SendInput(4, ins, sizeof(INPUT))) {
                            throw RUNTIME_EXCEPT("failed sending keyboard event") ;
                        }
                        break ;
                    }
                    case 3: {
#if defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif
                        INPUT ins[] = {
                            {INPUT_KEYBOARD},
                            {INPUT_KEYBOARD},
                            {INPUT_KEYBOARD},
                            {INPUT_KEYBOARD},
                            {INPUT_KEYBOARD},
                            {INPUT_KEYBOARD}
                        } ;
#if defined(__GNUC__)
#pragma gcc diagnostic warning "-wmissing-field-initializers"
#endif

                        ins[0].ki.wVk         = static_cast<WORD>(begin->to_code()) ;
                        ins[0].ki.wScan       = static_cast<WORD>(MapVirtualKeyA(ins[0].ki.wVk, MAPVK_VK_TO_VSC)) ;
                        ins[0].ki.dwFlags     = extended_key_flag(ins[0].ki.wVk) ;
                        ins[0].ki.dwExtraInfo = GetMessageExtraInfo() ;

                        ins[1].ki.wVk         = static_cast<WORD>((begin + 1)->to_code()) ;
                        ins[1].ki.wScan       = static_cast<WORD>(MapVirtualKeyA(ins[1].ki.wVk, MAPVK_VK_TO_VSC)) ;
                        ins[1].ki.dwFlags     = extended_key_flag(ins[1].ki.dwFlags) ;
                        ins[1].ki.dwExtraInfo = GetMessageExtraInfo() ;

                        ins[2].ki.wVk         = static_cast<WORD>((begin + 2)->to_code()) ;
                        ins[2].ki.wScan       = static_cast<WORD>(MapVirtualKeyA(ins[2].ki.wVk, MAPVK_VK_TO_VSC)) ;
                        ins[2].ki.dwFlags     = extended_key_flag(ins[2].ki.wVk) ;
                        ins[2].ki.dwExtraInfo = GetMessageExtraInfo() ;

                        ins[3].ki.wVk         = ins[2].ki.wVk ;
                        ins[3].ki.wScan       = ins[2].ki.wScan ;
                        ins[3].ki.dwFlags     = ins[2].ki.dwFlags | KEYEVENTF_KEYUP ;
                        ins[3].ki.dwExtraInfo = GetMessageExtraInfo() ;

                        ins[4].ki.wVk         = ins[1].ki.wVk ;
                        ins[4].ki.wScan       = ins[1].ki.wScan ;
                        ins[4].ki.dwFlags     = ins[1].ki.dwFlags | KEYEVENTF_KEYUP ;
                        ins[4].ki.dwExtraInfo = GetMessageExtraInfo() ;

                        ins[5].ki.wVk         = ins[0].ki.wVk ;
                        ins[5].ki.wScan       = ins[0].ki.wScan ;
                        ins[5].ki.dwFlags     = ins[0].ki.dwFlags | KEYEVENTF_KEYUP ;
                        ins[5].ki.dwExtraInfo = GetMessageExtraInfo() ;

                        open_some_ports(begin, end) ;
                        if(!SendInput(6, ins, sizeof(INPUT))) {
                            throw RUNTIME_EXCEPT("failed sending keyboard event") ;
                        }
                        break ;
                    }
                    default: {
                        //>=4
                        std::unique_ptr<INPUT[]> dins(new INPUT[size * 2]) ;
                        for(std::size_t i = 0 ; i < size ; i ++) {
                            dins[i].ki.wVk         = static_cast<WORD>((begin + i)->to_code()) ;
                            dins[i].ki.wScan       = static_cast<WORD>(MapVirtualKeyA(dins[i].ki.wVk, MAPVK_VK_TO_VSC)) ;
                            dins[i].ki.dwFlags     = extended_key_flag(dins[i].ki.wVk) ;
                            dins[i].ki.dwExtraInfo = GetMessageExtraInfo() ;

                            dins[size - i - 1].ki.wVk         = dins[i].ki.wVk ;
                            dins[size - i - 1].ki.wScan       = dins[i].ki.wScan ;
                            dins[size - i - 1].ki.dwFlags     = dins[i].ki.dwFlags | KEYEVENTF_KEYUP ;
                            dins[size - i - 1].ki.dwExtraInfo = GetMessageExtraInfo() ;
                        }

                        if(!SendInput(static_cast<UINT>(size), dins.get(), sizeof(INPUT))) {
                            throw RUNTIME_EXCEPT("failed sending keyboard event") ;
                        }

                        break ;
                    }
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

            void pushup(unsigned char key1, unsigned char key2) {
                pushup({KeyCode(key1), KeyCode(key2)}) ;
            }

            void pushup(unsigned char key1, KeyCode key2) {
                pushup({KeyCode(key1), key2}) ;
            }

            void pushup(KeyCode key1, unsigned char key2) {
                pushup({key1, KeyCode(key2)}) ;
            }

            void pushup(KeyCode key1, KeyCode key2) {
                pushup({key1, key2}) ;
            }

            template <typename... Ts>
            void pushup(
                    KeyCode key1,
                    KeyCode key2,
                    Ts&&... keys) {
                pushup({key1, key2, KeyCode(keys)...}) ;
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
