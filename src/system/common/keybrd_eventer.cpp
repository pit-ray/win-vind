#include "keybrd_eventer.hpp"

#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <utility>

#include <windows.h>

#include "key_absorber.hpp"
#include "key_log.hpp"
#include "msg_logger.hpp"
#include "utility.hpp"
#include "vkc_converter.hpp"


using namespace std ;

namespace KeybrdEventer
{
    struct SmartKey::Impl
    {
        INPUT in ;
        unsigned char key ;

        explicit Impl(const unsigned char keycode)
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
        try {
            release() ;
        }
        catch(const std::exception& e) {
            ERROR_PRINT(e.what()) ;
        }
    }

    SmartKey::SmartKey(SmartKey&&)            = default ;
    SmartKey& SmartKey::operator=(SmartKey&&) = default ;

    void SmartKey::send_event(const bool pressed) {
        pimpl->in.ki.dwFlags = pressed ? 0 : KEYEVENTF_KEYUP ;
        if(!SendInput(1, &pimpl->in, sizeof(INPUT))) {
            throw RUNTIME_EXCEPT("failed sending keyboard event") ;
            return ;
        }
    }

    void SmartKey::press() {
        KeyAbsorber::open_key(pimpl->key) ;
        send_event(true) ;
        KeyAbsorber::close() ;
        if(!(GetAsyncKeyState(pimpl->key) & 0x8000)) {
            throw RUNTIME_EXCEPT("You sent a key pressing event successfully, but the state of its key was not changed.") ;
            return ;
        }
    }

    void SmartKey::release() {
        KeyAbsorber::open_key(pimpl->key) ;
        send_event(false) ;
        KeyAbsorber::close() ;
        if(GetAsyncKeyState(pimpl->key) & 0x8000) {
            throw RUNTIME_EXCEPT("You sent a key releasing event successfully, but the state of its key was not changed.") ;
            return ;
        }
    }


    //change key state without input
    void release_keystate(const unsigned char key) {
        INPUT in ;
        in.type = INPUT_KEYBOARD ;
        in.ki.wVk = static_cast<WORD>(key) ;
        in.ki.wScan = static_cast<WORD>(MapVirtualKeyA(key, MAPVK_VK_TO_VSC)) ;
        in.ki.dwFlags = KEYEVENTF_KEYUP ;
        in.ki.time = 0 ;
        in.ki.dwExtraInfo = GetMessageExtraInfo() ;

        if(!SendInput(1, &in, sizeof(INPUT))) {
            throw RUNTIME_EXCEPT("failed sending keyboard event") ;
            return ;
        }
    }

    //change key state without input
    void press_keystate(const unsigned char key) {
        INPUT in ;
        in.type = INPUT_KEYBOARD ;
        in.ki.wVk = static_cast<WORD>(key) ;
        in.ki.wScan = static_cast<WORD>(MapVirtualKeyA(key, MAPVK_VK_TO_VSC)) ;
        in.ki.dwFlags = 0 ;
        in.ki.time = 0 ;
        in.ki.dwExtraInfo = GetMessageExtraInfo() ;

        if(!SendInput(1, &in, sizeof(INPUT))) {
            throw RUNTIME_EXCEPT("failed sending keyboard event") ;
            return ;
        }
    }

    void _pushup_core(std::initializer_list<unsigned char>&& initl) {
        const auto pressing_keys = KeyAbsorber::get_pressed_list() ;

        const auto recover_keystate= [&pressing_keys] {
            for(const auto key : pressing_keys) {
                KeyAbsorber::press_vertually(key) ;
            }
        } ;

        if(initl.size() == 1) {
            SmartKey ins(static_cast<unsigned char>(*initl.begin())) ;
            ins.press() ;
            recover_keystate() ;
            return ;
        }

        std::stack<std::unique_ptr<SmartKey>> st ;
        const auto clear_stack = [&st] {
            while(!st.empty()) {
                st.pop() ;
            }
        } ;

        try {
            for(auto iter = initl.begin() ; iter != initl.end() ; iter ++) {
                const auto key = static_cast<unsigned char>(*iter) ;
                st.push(std::make_unique<SmartKey>(key)) ;
                st.top()->press() ;
            }
        }
        catch(const std::runtime_error& e) {
            clear_stack() ;
            recover_keystate() ;
            throw e ;
            return ;
        }

        clear_stack() ;
        recover_keystate() ;
    }
}
