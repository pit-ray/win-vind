#include "keybrd_eventer.hpp"

#include <algorithm>
#include <iostream>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>

#include <windows.h>

#include "key_absorber.hpp"
#include "key_log.hpp"
#include "msg_logger.hpp"
#include "utility.hpp"
#include "vkc_converter.hpp"
#include "virtual_key_fwd.hpp"

using namespace std ;

template <typename T>
using vec_stack = std::stack<T, std::vector<T>> ;

namespace KeybrdEventer
{
    template <typename T>
    inline static auto extended_key_flag(T key) {
        switch(key) {
            case VKC_UP:
            case VKC_DOWN:
            case VKC_LEFT:
            case VKC_RIGHT:
            case VKC_HOME:
            case VKC_END:
            case VKC_PAGEUP:
            case VKC_PAGEDOWN:
            case VKC_INSERT:
            case VKC_DELETE:
                return KEYEVENTF_EXTENDEDKEY ;
            default:
                return 0 ;
        }
    }

    struct SmartKey::Impl
    {
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
    : pimpl(make_unique<Impl>(key))
    {}

    SmartKey::~SmartKey() noexcept {
        try {release() ;}
        catch(const std::exception& e) {
            ERROR_PRINT(e.what()) ;
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
        KeyAbsorber::open_key(pimpl->key) ;
        send_event(true) ;
        KeyAbsorber::close() ;
        if(!(GetAsyncKeyState(pimpl->key) & 0x8000)) {
            throw RUNTIME_EXCEPT("You sent a key pressing event successfully, but the state of its key was not changed.") ;
        }
    }

    void SmartKey::release() {
        KeyAbsorber::open_key(pimpl->key) ;
        send_event(false) ;
        KeyAbsorber::close() ;
        if(GetAsyncKeyState(pimpl->key) & 0x8000) {
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

    void _pushup_core(std::initializer_list<unsigned char>&& initl) {
        using KeyAbsorber::close ;
        using KeyAbsorber::open_key ;
        using KeyAbsorber::open_keys ;

        auto state = KeyAbsorber::get_pressed_list() ;
        auto recover_keystate= [&state] {
            for(const auto key : state)
                KeyAbsorber::press_vertually(key) ;
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

            open_key(*initl.begin()) ;

            if(!SendInput(2, ins, sizeof(INPUT))) {
                throw RUNTIME_EXCEPT("failed sending keyboard event") ;
            }

            close() ;
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

            open_keys(initl) ;
            if(!SendInput(4, ins, sizeof(INPUT))) {
                throw RUNTIME_EXCEPT("failed sending keyboard event") ;
            }
            close() ;
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

            open_keys(initl) ;
            if(!SendInput(6, ins, sizeof(INPUT))) {
                throw RUNTIME_EXCEPT("failed sending keyboard event") ;
            }
            close() ;
            recover_keystate() ;
            return ;
        }

        //>=4
        static vec_stack<std::unique_ptr<SmartKey>> st ;
        const auto clear_stack = [] {while(!st.empty()) st.pop() ;} ;

        try {
            for(auto iter = initl.begin() ; iter != initl.end() ; iter ++) {
                st.push(std::make_unique<SmartKey>(
                            static_cast<unsigned char>(*iter))) ;
                st.top()->press() ;
            }
        }
        catch(const std::runtime_error& e) {
            clear_stack() ;
            recover_keystate() ;
            throw e ;
        }

        clear_stack() ;
        recover_keystate() ;
    }
}
