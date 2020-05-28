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
        is_release() ;
    }

    SmartKey::SmartKey(SmartKey&&) noexcept = default ;
    SmartKey& SmartKey::operator=(SmartKey&&) noexcept = default ;

    bool SmartKey::is_send_event(const bool pushed) noexcept {
        pimpl->in.ki.dwFlags = pushed ? 0 : KEYEVENTF_KEYUP ;
        if(!SendInput(1, &pimpl->in, sizeof(INPUT))) {
            WIN_ERROR_STREAM << "(KeybrdEventer::SmartKey::is_send_event::SendInput)\n" ;
            return false ;
        }
        return true ;
    }

    bool SmartKey::is_push() noexcept {
        KeyAbsorber::open_key(pimpl->key) ;
        if(!is_send_event(true)) {
            return false ;
        }
        KeyAbsorber::close() ;

        return GetAsyncKeyState(pimpl->key) & 0x8000 ;
    }

    bool SmartKey::is_release() noexcept {
        KeyAbsorber::open_key(pimpl->key) ;
        if(!is_send_event(false)) {
            return false ;
        }
        KeyAbsorber::close() ;

        return !(GetAsyncKeyState(pimpl->key) & 0x8000) ;
    }


    //change key state without input
    bool is_release_keystate(const unsigned char key) noexcept {
        INPUT in ;
        in.type = INPUT_KEYBOARD ;
        in.ki.wVk = static_cast<WORD>(key) ;
        in.ki.wScan = static_cast<WORD>(MapVirtualKeyA(key, MAPVK_VK_TO_VSC)) ;
        in.ki.dwFlags = KEYEVENTF_KEYUP ;
        in.ki.time = 0 ;
        in.ki.dwExtraInfo = GetMessageExtraInfo() ;

        if(!SendInput(1, &in, sizeof(INPUT))) {
            WIN_ERROR_STREAM << "(keybrd_eventer.hpp::is_release)\n" ;
            return false ;
        }

        return true ;
    }

    //change key state without input
    bool is_push_keystate(const unsigned char key) noexcept {
        INPUT in ;
        in.type = INPUT_KEYBOARD ;
        in.ki.wVk = static_cast<WORD>(key) ;
        in.ki.wScan = static_cast<WORD>(MapVirtualKeyA(key, MAPVK_VK_TO_VSC)) ;
        in.ki.dwFlags = 0 ;
        in.ki.time = 0 ;
        in.ki.dwExtraInfo = GetMessageExtraInfo() ;

        if(!SendInput(1, &in, sizeof(INPUT))) {
            WIN_ERROR_STREAM << "(keybrd_eventer.hpp::is_release)\n" ;
            return false ;
        }
        return true ;
    }

    bool _is_pushup_core(std::initializer_list<unsigned char>&& initl) {
        const auto pushing_keys = KeyAbsorber::get_downed_list() ;

        const auto recover_keystate= [&pushing_keys] {
            for(const auto key : pushing_keys) {
                KeyAbsorber::push_vertually(key) ;
            }
        } ;

        if(initl.size() == 1) {
            SmartKey ins(static_cast<unsigned char>(*initl.begin())) ;

            if(!ins.is_push()) {
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
            if(!st.top()->is_push()) {
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