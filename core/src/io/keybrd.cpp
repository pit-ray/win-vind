#include "io/keybrd.hpp"

#include <algorithm>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>

#if defined(DEBUG)
#include <iostream>
#endif

#include <windows.h>

#include "err_logger.hpp"
#include "key/key_absorber.hpp"
#include "key/key_log.hpp"
#include "key/keycode_def.hpp"
#include "key/keycodecvt.hpp"
#include "util/def.hpp"

namespace
{
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
}

namespace vind
{
    namespace keybrd {

        bool is_pressed_actually(KeyCode key) noexcept {
            return GetAsyncKeyState(key) & 0x8000 ;
        }

        struct ScopedKey::Impl {
            INPUT in ;
            KeyCode key ;

            explicit Impl(KeyCode keycode)
            : in(),
              key(keycode)
            {
                ZeroMemory(&in, sizeof(INPUT)) ;

                in.type     = INPUT_KEYBOARD ;
                in.ki.wVk   = static_cast<WORD>(key) ;
                in.ki.wScan = static_cast<WORD>(MapVirtualKeyA(key, MAPVK_VK_TO_VSC)) ;
            }
        } ;

        ScopedKey::ScopedKey(KeyCode key)
        : pimpl(std::make_unique<Impl>(key))
        {}

        ScopedKey::~ScopedKey() noexcept {
            try {release() ;}
            catch(const std::exception& e) {
                PRINT_ERROR(e.what()) ;
            }
        }

        ScopedKey::ScopedKey(ScopedKey&&)            = default ;
        ScopedKey& ScopedKey::operator=(ScopedKey&&) = default ;

        void ScopedKey::send_event(bool pressed) {
            pimpl->in.ki.dwFlags = (pressed ? 0 : KEYEVENTF_KEYUP) | extended_key_flag(pimpl->key) ;
            if(!SendInput(1, &pimpl->in, sizeof(INPUT))) {
                throw RUNTIME_EXCEPT("failed sending keyboard event") ;
            }
        }

        void ScopedKey::press() {
            keyabsorber::open_port(pimpl->key) ;
            send_event(true) ;
            keyabsorber::close_all_ports() ;
            if(!is_pressed_actually(pimpl->key)) {
                throw RUNTIME_EXCEPT("You sent a key pressing event successfully, but the state of its key was not changed.") ;
            }
        }

        void ScopedKey::release() {
            keyabsorber::open_port(pimpl->key) ;
            send_event(false) ;
            keyabsorber::close_all_ports() ;
            if(is_pressed_actually(pimpl->key)) {
                throw RUNTIME_EXCEPT("You sent a key releasing event successfully, but the state of its key was not changed.") ;
            }
        }


        //change key state without input
        void release_keystate(KeyCode key, bool enable_vhook) {
            static INPUT in = {INPUT_KEYBOARD} ;
            in.ki.wVk         = static_cast<WORD>(key) ;
            in.ki.wScan       = static_cast<WORD>(MapVirtualKeyA(key, MAPVK_VK_TO_VSC)) ;
            in.ki.dwFlags     = KEYEVENTF_KEYUP | extended_key_flag(key) ;
            in.ki.dwExtraInfo = GetMessageExtraInfo() ;

            if(!SendInput(1, &in, sizeof(INPUT))) {
                throw RUNTIME_EXCEPT("failed sending keyboard event") ;
            }

            if(enable_vhook) {
                keyabsorber::release_virtually(key) ;
            }
        }

        //change key state without input
        void press_keystate(KeyCode key, bool enable_vhook) {
            static INPUT in = {INPUT_KEYBOARD} ;
            in.ki.wVk         = static_cast<WORD>(key) ;
            in.ki.wScan       = static_cast<WORD>(MapVirtualKeyA(key, MAPVK_VK_TO_VSC)) ;
            in.ki.dwFlags     = extended_key_flag(key) ;
            in.ki.dwExtraInfo = GetMessageExtraInfo() ;

            if(!SendInput(1, &in, sizeof(INPUT))) {
                throw RUNTIME_EXCEPT("failed sending keyboard event") ;
            }

            if(enable_vhook) {
                keyabsorber::press_virtually(key) ;
            }
        }

        void pushup_core(std::initializer_list<KeyCode>&& initl) {
            using keyabsorber::close_all_ports ;
            using keyabsorber::open_port ;
            using keyabsorber::open_some_ports ;

            static INPUT ins[6] = {
                {INPUT_KEYBOARD},
                {INPUT_KEYBOARD},
                {INPUT_KEYBOARD},
                {INPUT_KEYBOARD},
                {INPUT_KEYBOARD},
                {INPUT_KEYBOARD},
            } ;

            const auto pre_state = keyabsorber::get_pressed_list() ;

            //optimizing for 1
            switch(initl.size()) {
                case 1:
                    ins[0].ki.wVk         = static_cast<WORD>(*initl.begin()) ;
                    ins[0].ki.dwFlags     = extended_key_flag(*initl.begin()) ;
                    ins[0].ki.dwExtraInfo = GetMessageExtraInfo() ;

                    ins[1].ki.wVk         = static_cast<WORD>(*initl.begin()) ;
                    ins[1].ki.dwFlags     = KEYEVENTF_KEYUP | extended_key_flag(*initl.begin()) ;
                    ins[1].ki.dwExtraInfo = GetMessageExtraInfo() ;

                    open_port(*initl.begin()) ;

                    if(!SendInput(2, ins, sizeof(INPUT))) {
                        throw RUNTIME_EXCEPT("failed sending keyboard event") ;
                    }
                    break ;

                case 2:
                    ins[0].ki.wVk         = static_cast<WORD>(*initl.begin()) ;
                    ins[0].ki.dwFlags     = extended_key_flag(*initl.begin()) ;
                    ins[0].ki.dwExtraInfo = GetMessageExtraInfo() ;

                    ins[1].ki.wVk         = static_cast<WORD>(*(initl.begin() + 1)) ;
                    ins[1].ki.dwFlags     = extended_key_flag(*(initl.begin() + 1)) ;
                    ins[1].ki.dwExtraInfo = GetMessageExtraInfo() ;

                    ins[2].ki.wVk         = static_cast<WORD>(*(initl.begin() + 1)) ;
                    ins[2].ki.dwFlags     = KEYEVENTF_KEYUP | extended_key_flag(*(initl.begin() + 1));
                    ins[2].ki.dwExtraInfo = GetMessageExtraInfo() ;

                    ins[3].ki.wVk         = static_cast<WORD>(*initl.begin()) ;
                    ins[3].ki.dwFlags     = KEYEVENTF_KEYUP | extended_key_flag(*initl.begin()) ;
                    ins[3].ki.dwExtraInfo = GetMessageExtraInfo() ;

                    open_some_ports(initl) ;
                    if(!SendInput(4, ins, sizeof(INPUT))) {
                        throw RUNTIME_EXCEPT("failed sending keyboard event") ;
                    }
                    break ;

                case 3:
                    ins[0].ki.wVk         = static_cast<WORD>(*initl.begin()) ;
                    ins[0].ki.dwFlags     = extended_key_flag(*initl.begin()) ;
                    ins[0].ki.dwExtraInfo = GetMessageExtraInfo() ;

                    ins[1].ki.wVk         = static_cast<WORD>(*(initl.begin() + 1)) ;
                    ins[1].ki.dwFlags     = extended_key_flag(*(initl.begin() + 1)) ;
                    ins[1].ki.dwExtraInfo = GetMessageExtraInfo() ;

                    ins[2].ki.wVk         = static_cast<WORD>(*(initl.begin() + 2)) ;
                    ins[2].ki.dwFlags     = extended_key_flag(*(initl.begin() + 2)) ;
                    ins[2].ki.dwExtraInfo = GetMessageExtraInfo() ;

                    ins[3].ki.wVk         = static_cast<WORD>(*(initl.begin() + 2)) ;
                    ins[3].ki.dwFlags     = KEYEVENTF_KEYUP | extended_key_flag(*(initl.begin() + 2)) ;
                    ins[3].ki.dwExtraInfo = GetMessageExtraInfo() ;

                    ins[4].ki.wVk         = static_cast<WORD>(*(initl.begin() + 1)) ;
                    ins[4].ki.dwFlags     = KEYEVENTF_KEYUP | extended_key_flag(*(initl.begin() + 1)) ;
                    ins[4].ki.dwExtraInfo = GetMessageExtraInfo() ;

                    ins[5].ki.wVk         = static_cast<WORD>(*initl.begin()) ;
                    ins[5].ki.dwFlags     = KEYEVENTF_KEYUP | extended_key_flag(*initl.begin());
                    ins[5].ki.dwExtraInfo = GetMessageExtraInfo() ;

                    open_some_ports(initl) ;
                    if(!SendInput(6, ins, sizeof(INPUT))) {
                        throw RUNTIME_EXCEPT("failed sending keyboard event") ;
                    }
                    break ;

                default: 
                    //>=4
                    using ScopedKeyStack = std::stack<ScopedKey, std::vector<ScopedKey>> ;
                    static ScopedKeyStack st ;

                    for(auto iter = initl.begin() ; iter != initl.end() ; iter ++) {
                        st.emplace(static_cast<KeyCode>(*iter)) ;
                        st.top().press() ;
                    }

                    ScopedKeyStack().swap(st) ; //clear
                    break ;
            }

            close_all_ports() ;

            for(auto key : (pre_state - keyabsorber::get_pressed_list())) {
                press_keystate(key) ;
            }
        }
    }
}
