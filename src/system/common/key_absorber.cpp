#include "key_absorber.hpp"

#include <windows.h>

#include <array>
#include <memory>
#include <iostream>
#include <algorithm>
#include <unordered_map>

#include "msg_logger.hpp"
#include "keybrd_eventer.hpp"
#include "vkc_converter.hpp"

using namespace std ;

namespace KeyAbsorber
{
    static array<bool, 256> _state{false} ;
    static bool _absorbed_flag{true} ;
    static KeyLog::data_t _ignored_keys{} ;

    static const auto uninstaller = [](HHOOK* p_hook) {
        if(p_hook == nullptr) return ;

        if(!UnhookWindowsHookEx(*p_hook)) {
            WIN_ERROR_PRINT("cannot unhook LowLevelKeyboardProc") ;
        }

        delete p_hook ;
    } ;

    static unique_ptr<HHOOK, decltype(uninstaller)> p_handle(nullptr, uninstaller) ;

    static LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) noexcept {
        const auto release = [&wParam, &lParam](const int code) {
            return CallNextHookEx(*p_handle, code, wParam, lParam) ;
        } ;

        if(nCode < HC_ACTION) {
            //not processed
            return release(nCode) ;
        }
        const auto code = static_cast<unsigned char>(reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam)->vkCode) ;
        const auto state = (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) ;
        _state[code] = state ;
        _state[VKCConverter::get_representative_key(code)] = state ;

        if(!_ignored_keys.empty()) {
            if(std::find(_ignored_keys.cbegin(), _ignored_keys.cend(), code) != _ignored_keys.cend()) {
                return release(HC_ACTION) ;
            }
        }

        if(_absorbed_flag) {
            return -1 ; //absorbing
        }
        //not absorbing
        return release(HC_ACTION) ;
    }

    bool install_hook() noexcept {
        _state.fill(false) ;

        p_handle.reset(new HHOOK{}) ; //added ownership
        if(p_handle == nullptr) {
            return false ;
        }

        *p_handle = SetWindowsHookEx(
            WH_KEYBOARD_LL,
            static_cast<HOOKPROC>(LowLevelKeyboardProc),
            nullptr, 0
        ) ;

        if(!*p_handle) {
            WIN_ERROR_PRINT("handle is nullptr") ;
            return false ;
        }
        return true ;
    }

    bool is_pressed(const unsigned char keycode) noexcept {
        if(keycode < 1 || keycode > 254) {
            return false ;
        }
        return _state[keycode] ;
    }

    const KeyLog get_pressed_list() noexcept {
        KeyLog::data_t res{} ;
        for(unsigned char i = 1 ; i < 255 ; i ++) {
            if(is_pressed(i)) res.insert(i) ;
        }
        return KeyLog(res) ;
    }

    //if this object is not hooked, can call following functions.
    bool is_closed() noexcept {
        return _absorbed_flag ;
    }

    void close() noexcept {
        _ignored_keys.clear() ;
        _absorbed_flag = true ;
    }

    bool close_with_refresh() noexcept {
        _ignored_keys.clear() ;

        //if this function is called by pressed button,
        //it has to send message "KEYUP" to OS (not absorbed).
        for(const auto& vkc : get_pressed_list()) {
            if(!KeybrdEventer::release_keystate(vkc)) {
                return false ;
            }
        }

        _absorbed_flag = true ;
        return true ;
    }

    void open() noexcept {
        _ignored_keys.clear() ;
        _absorbed_flag = false ;
    }

    void open_keys(const KeyLog::data_t& keys) noexcept {
        _ignored_keys = keys ;
    }

    void open_key(const unsigned char key) noexcept {
        try {
            _ignored_keys.insert(key) ;
        }
        catch(bad_alloc& e) {
            ERROR_PRINT(e.what()) ;
            return ;
        }
    }

    void release_vertually(const unsigned char key) noexcept {
        _state[key] = false ;
    }
    void press_vertually(const unsigned char key) noexcept {
        _state[key] = true ;
    }
}
