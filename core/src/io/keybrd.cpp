#include "io/keybrd.hpp"

#include <algorithm>
#include <iostream>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>

#include <windows.h>

#include "key/key_absorber.hpp"
#include "key/key_log.hpp"
#include "key/keycode_def.hpp"
#include "key/keycodecvt.hpp"
#include "coreio/err_logger.hpp"
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

        bool is_pressed_actually(const unsigned char key) noexcept {
            return GetAsyncKeyState(key) & 0x8000 ;
        }

        struct SmartKey::Impl {
            INPUT in ;
            unsigned char key ;

            explicit Impl(const unsigned char keycode)
            : in(),
              key(keycode)
            {
                ZeroMemory(&in, sizeof(INPUT)) ;

                in.type     = INPUT_KEYBOARD ;
                in.ki.wVk   = static_cast<WORD>(key) ;
                in.ki.wScan = static_cast<WORD>(MapVirtualKeyA(key, MAPVK_VK_TO_VSC)) ;
            }
        } ;

        SmartKey::SmartKey(const unsigned char key)
        : pimpl(std::make_unique<Impl>(key))
        {}

        SmartKey::~SmartKey() noexcept {
            try {release() ;}
            catch(const std::exception& e) {
                PRINT_ERROR(e.what()) ;
            }
        }

        SmartKey::SmartKey(SmartKey&&)            = default ;
        SmartKey& SmartKey::operator=(SmartKey&&) = default ;

        void SmartKey::send_event(const bool pressed) {
            pimpl->in.ki.dwFlags = (pressed ? 0 : KEYEVENTF_KEYUP) | extended_key_flag(pimpl->key) ;
            if(!SendInput(1, &pimpl->in, sizeof(INPUT))) {
                throw RUNTIME_EXCEPT("failed sending keyboard event") ;
            }
        }

        void SmartKey::press() {
            keyabsorber::open_port(pimpl->key) ;
            send_event(true) ;
            keyabsorber::close_all_ports() ;
            if(!is_pressed_actually(pimpl->key)) {
                throw RUNTIME_EXCEPT("You sent a key pressing event successfully, but the state of its key was not changed.") ;
            }
        }

        void SmartKey::release() {
            keyabsorber::open_port(pimpl->key) ;
            send_event(false) ;
            keyabsorber::close_all_ports() ;
            if(is_pressed_actually(pimpl->key)) {
                throw RUNTIME_EXCEPT("You sent a key releasing event successfully, but the state of its key was not changed.") ;
            }
        }


        //change key state without input
        void release_keystate(const unsigned char key) {
            static INPUT in ;
            in.type           = INPUT_KEYBOARD ;
            in.ki.wVk         = static_cast<WORD>(key) ;
            in.ki.wScan       = static_cast<WORD>(MapVirtualKeyA(key, MAPVK_VK_TO_VSC)) ;
            in.ki.dwFlags     = KEYEVENTF_KEYUP | extended_key_flag(key) ;
            in.ki.time        = 0 ;
            in.ki.dwExtraInfo = GetMessageExtraInfo() ;

            if(!SendInput(1, &in, sizeof(INPUT))) {
                throw RUNTIME_EXCEPT("failed sending keyboard event") ;
            }
        }

        //change key state without input
        void press_keystate(const unsigned char key) {
            static INPUT in ;
            in.type           = INPUT_KEYBOARD ;
            in.ki.wVk         = static_cast<WORD>(key) ;
            in.ki.wScan       = static_cast<WORD>(MapVirtualKeyA(key, MAPVK_VK_TO_VSC)) ;
            in.ki.dwFlags     = extended_key_flag(key) ;
            in.ki.time        = 0 ;
            in.ki.dwExtraInfo = GetMessageExtraInfo() ;

            if(!SendInput(1, &in, sizeof(INPUT))) {
                throw RUNTIME_EXCEPT("failed sending keyboard event") ;
            }
        }

        void pushup_core(std::initializer_list<unsigned char>&& initl) {
            using keyabsorber::close_all_ports ;
            using keyabsorber::open_port ;
            using keyabsorber::open_some_ports ;

            auto state = keyabsorber::get_pressed_list() ;
            auto recover_keystate= [&state] {
                for(const auto key : state)
                    keyabsorber::press_virtually(key) ;
            } ;
            static INPUT ins[6] = {
                {INPUT_KEYBOARD, {.ki = {0, 0, 0, 0, 0}}},
                {INPUT_KEYBOARD, {.ki = {0, 0, 0, 0, 0}}},
                {INPUT_KEYBOARD, {.ki = {0, 0, 0, 0, 0}}},
                {INPUT_KEYBOARD, {.ki = {0, 0, 0, 0, 0}}},
                {INPUT_KEYBOARD, {.ki = {0, 0, 0, 0, 0}}},
                {INPUT_KEYBOARD, {.ki = {0, 0, 0, 0, 0}}}
            } ;

            //optimizing for 1
            if(initl.size() == 1) {
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

                close_all_ports() ;
                recover_keystate() ;
                return ;
            }

            //optimizing for 2
            if(initl.size() == 2) {
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
                close_all_ports() ;
                recover_keystate() ;
                return ;
            }

            //optimizing for 3
            if(initl.size() == 3) {
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
                close_all_ports() ;
                recover_keystate() ;
                return ;
            }

            //>=4
            using SmartKeyStack = std::stack<SmartKey, std::vector<SmartKey>> ;
            static SmartKeyStack st ;

            try {
                for(auto iter = initl.begin() ; iter != initl.end() ; iter ++) {
                    st.push(SmartKey(static_cast<unsigned char>(*iter))) ;
                    st.top().press() ;
                }
            }
            catch(const std::runtime_error& e) {
                SmartKeyStack().swap(st) ; //clear
                recover_keystate() ;
                throw e ;
            }

            SmartKeyStack().swap(st) ; //clear
            recover_keystate() ;
        }
    }
}
