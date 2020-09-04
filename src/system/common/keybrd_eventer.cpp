#include "keybrd_eventer.hpp"

#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <utility>

#include <windows.h>

#include "key_absorber.hpp"
#include "msg_logger.hpp"
#include "key_log.hpp"
#include "vkc_converter.hpp"


using namespace std ;

namespace KeybrdEventer
{
    struct SmartKey::Impl
    {
        INPUT in ;
        unsigned char key ;

        explicit Impl(const unsigned char keycode) noexcept
        : in(),
          key(keycode)
        {
            ZeroMemory(&in, sizeof(INPUT)) ;

            in.type = INPUT_KEYBOARD ;
            in.ki.wVk = static_cast<WORD>(key) ;
            in.ki.wScan = static_cast<WORD>(MapVirtualKeyA(key, MAPVK_VK_TO_VSC)) ;
        }
    } ;

    SmartKey::SmartKey(const unsigned char key)
    : pimpl(make_unique<Impl>(key))
    {}

    SmartKey::~SmartKey() noexcept {
        release() ;
    }

    SmartKey::SmartKey(SmartKey&&) noexcept = default ;
    SmartKey& SmartKey::operator=(SmartKey&&) noexcept = default ;

    bool SmartKey::send_event(const bool pressed) noexcept {
        pimpl->in.ki.dwFlags = pressed ? 0 : KEYEVENTF_KEYUP ;
        if(!SendInput(1, &pimpl->in, sizeof(INPUT))) {
            WIN_ERROR_STREAM << "(KeybrdEventer::SmartKey::send_event::SendInput)\n" ;
            return false ;
        }
        return true ;
    }

    bool SmartKey::press() noexcept {
        KeyAbsorber::open_key(pimpl->key) ;
        if(!send_event(true)) {
            return false ;
        }
        KeyAbsorber::close() ;

        return GetAsyncKeyState(pimpl->key) & 0x8000 ;
    }

    bool SmartKey::release() noexcept {
        KeyAbsorber::open_key(pimpl->key) ;
        if(!send_event(false)) {
            return false ;
        }
        KeyAbsorber::close() ;

        return !(GetAsyncKeyState(pimpl->key) & 0x8000) ;
    }


    //change key state without input
    bool release_keystate(const unsigned char key) noexcept {
        INPUT in ;
        in.type = INPUT_KEYBOARD ;
        in.ki.wVk = static_cast<WORD>(key) ;
        in.ki.wScan = static_cast<WORD>(MapVirtualKeyA(key, MAPVK_VK_TO_VSC)) ;
        in.ki.dwFlags = KEYEVENTF_KEYUP ;
        in.ki.time = 0 ;
        in.ki.dwExtraInfo = GetMessageExtraInfo() ;

        if(!SendInput(1, &in, sizeof(INPUT))) {
            WIN_ERROR_STREAM << "(keybrd_eventer.hpp::release)\n" ;
            return false ;
        }

        return true ;
    }

    //change key state without input
    bool press_keystate(const unsigned char key) noexcept {
        INPUT in ;
        in.type = INPUT_KEYBOARD ;
        in.ki.wVk = static_cast<WORD>(key) ;
        in.ki.wScan = static_cast<WORD>(MapVirtualKeyA(key, MAPVK_VK_TO_VSC)) ;
        in.ki.dwFlags = 0 ;
        in.ki.time = 0 ;
        in.ki.dwExtraInfo = GetMessageExtraInfo() ;

        if(!SendInput(1, &in, sizeof(INPUT))) {
            WIN_ERROR_STREAM << "(keybrd_eventer.hpp::release)\n" ;
            return false ;
        }
        return true ;
    }

    bool _pressup_core(std::initializer_list<unsigned char>&& initl) {
        const auto pressing_keys = KeyAbsorber::get_pressed_list() ;

        const auto recover_keystate= [&pressing_keys] {
            for(const auto key : pressing_keys) {
                KeyAbsorber::press_vertually(key) ;
            }
        } ;

        if(initl.size() == 1) {
            SmartKey ins(static_cast<unsigned char>(*initl.begin())) ;

            if(!ins.press()) {
                return false ;
            }

            recover_keystate() ;
            return true ;
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
            if(!st.top()->press()) {
                clear_stack() ;
                recover_keystate() ;
                return false ;
            }
        }

        clear_stack() ;
        recover_keystate() ;
        return true ;
    }
}