#ifndef _KEYBRD_EVENTER_HPP
#define _KEYBRD_EVENTER_HPP

#include "key_absorber.hpp"
#include "msg_logger.hpp"
#include <windows.h>
#include <iostream>
#include <algorithm>
#include <stack>
#include <memory>
#include "virtual_key_fwd.hpp"

namespace KeybrdEventer
{
    class SmartKey {
    private:
        INPUT in ;
        unsigned char key ;

        bool is_send_event(const bool pushed) noexcept {
            in.ki.dwFlags = pushed ? 0 : KEYEVENTF_KEYUP ;
            if(!SendInput(1, &in, sizeof(INPUT))) {
                Logger::error_stream << "[Error] windows.h " \
                << GetLastError() \
                << " (keybrd_eventer.hpp)\n" ;
                return false ;
            }
            return true ;
        }

    public:
        template <typename T>
        explicit SmartKey(const T keycode) noexcept : in(), key(static_cast<unsigned char>(keycode)) {
            in.type = INPUT_KEYBOARD ;
            in.ki.wVk = static_cast<WORD>(keycode) ;
            in.ki.wScan = 0 ;
            in.ki.dwFlags = 0 ;
            in.ki.time = 0 ;
            in.ki.dwExtraInfo = GetMessageExtraInfo() ;
        }

        bool is_push() noexcept {
            if(KeyAbsorber::is_down(key)) {
                return true ;
            }

            KeyAbsorber::open_key(key) ;
            is_send_event(true) ;
            KeyAbsorber::close() ;
            return KeyAbsorber::is_down(key) ;
        }

        ~SmartKey() noexcept {
            if(!KeyAbsorber::is_down(key)) {
                return ;
            }

            KeyAbsorber::open_key(key) ;
            is_send_event(false) ;
            KeyAbsorber::close() ;
        }

        SmartKey(SmartKey&&) = default ;
        SmartKey& operator=(SmartKey&&) = default ;

        SmartKey(const SmartKey&) = default ;
        SmartKey& operator=(const SmartKey&) = default ;
    } ;

    template <typename T, typename ...Args>
    static bool is_pushup(const T arg, const Args... args) {
        const std::initializer_list<T> initl = {arg, args...} ;

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
                return false ;
            }
        }

        clear_stack() ;

        return true ;
    }

    template <typename T>
    bool is_release(const T key) noexcept {
        INPUT in ;
        in.type = INPUT_KEYBOARD ;
        in.ki.wVk = static_cast<WORD>(key) ;
        in.ki.wScan = 0 ;
        in.ki.dwFlags = KEYEVENTF_KEYUP ;
        in.ki.time = 0 ;
        in.ki.dwExtraInfo = GetMessageExtraInfo() ;

        if(!SendInput(1, &in, sizeof(INPUT))) {
            Logger::error_stream << "[Error] windows.h " \
            << GetLastError() \
            << " (keybrd_eventer.hpp::is_release)\n" ;
            return false ;
        }

        return true ;
    }

    template <typename T>
    bool is_push(const T key) noexcept {
        INPUT in ;
        in.type = INPUT_KEYBOARD ;
        in.ki.wVk = static_cast<WORD>(key) ;
        in.ki.wScan = 0 ;
        in.ki.dwFlags = 0 ;
        in.ki.time = 0 ;
        in.ki.dwExtraInfo = GetMessageExtraInfo() ;

        if(!SendInput(1, &in, sizeof(INPUT))) {
            Logger::error_stream << "[Error] windows.h " \
            << GetLastError() \
            << " (keybrd_eventer.hpp::is_release)\n" ;
            return false ;
        }
        return true ;
    }
}

#endif