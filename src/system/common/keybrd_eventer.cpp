#include "keybrd_eventer.hpp"

#include "key_absorber.hpp"
#include "msg_logger.hpp"
#include "key_log.hpp"
#include <windows.h>
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <utility>

using namespace std ;

namespace KeybrdEventer
{
    struct SmartKey::Impl
    {
        INPUT in ;
        explicit Impl(const unsigned char key) noexcept : in() {
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
        const auto&& key = static_cast<unsigned char>(pimpl->in.ki.wVk) ;
        if(!KeyAbsorber::is_down(key)) {
            return ;
        }

        KeyAbsorber::open_key(key) ;
        is_send_event(false) ;
        KeyAbsorber::close() ;
    }

    SmartKey::SmartKey(SmartKey&&) = default ;
    SmartKey& SmartKey::operator=(SmartKey&&) = default ;

    bool SmartKey::is_send_event(const bool pushed) noexcept {
        pimpl->in.ki.dwFlags = pushed ? 0 : KEYEVENTF_KEYUP ;
        if(!SendInput(1, &pimpl->in, sizeof(INPUT))) {
            ERROR_STREAM << "windows.h " << GetLastError() << " (keybrd_eventer.hpp)\n" ;
            return false ;
        }
        return true ;
    }

    bool SmartKey::is_push() noexcept {
        const auto&& key = static_cast<unsigned char>(pimpl->in.ki.wVk) ;
        if(KeyAbsorber::is_down(key)) {
            return true ;
        }

        KeyAbsorber::open_key(key) ;
        if(!is_send_event(true)) {
            return false ;
        }
        KeyAbsorber::close() ;
        return KeyAbsorber::is_down(key) ;
    }


    //free functions

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
            ERROR_STREAM << "windows.h " << GetLastError() << " (keybrd_eventer.hpp::is_release)\n" ;
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
            ERROR_STREAM << "windows.h " << GetLastError() << " (keybrd_eventer.hpp::is_release)\n" ;
            return false ;
        }
        return true ;
    }

    static unordered_map<unsigned char, unique_ptr<SmartKey>> _syncm ;

    bool is_sync_push(const unsigned char bindkey, const unsigned char funckey) {
        try {
            _syncm.at(bindkey) ;
            //exist
            cout << "finded\n" ;
        }
        catch(out_of_range&) {
            cout << "not finded\n" ;
            //not exist
            _syncm.insert(make_pair(bindkey, make_unique<SmartKey>(funckey))) ;

            if(!_syncm.at(bindkey)->is_push()) {
                cout << "not existed\n" ;
                return false ;
            }
        }

        return true ;
    }

    void update_sync_push() noexcept {
        if(_syncm.empty()) {
            return ;
        }

        /*
        if(GetAsyncKeyState(VKC_LEFT) & 0x8000)
        {
            cout << "LEFT is down\n" ;
        }
        */

        for(const auto& pair : _syncm) {
            if(!KeyAbsorber::is_down(pair.first)) {
                cout << "erased\n" ;
                _syncm.erase(pair.first) ;
            }
        }
    }
}