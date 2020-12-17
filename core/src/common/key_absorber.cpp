#include "key_absorber.hpp"

#include <windows.h>

#include <algorithm>
#include <array>
#include <iostream>
#include <memory>
#include <unordered_map>

#include "keybrd_eventer.hpp"
#include "msg_logger.hpp"
#include "utility.hpp"
#include "vkc_converter.hpp"

using namespace std ;

namespace KeyAbsorber
{
    static array<bool, 256> g_state{false} ;
    static bool g_absorbed_flag{true} ;
    static KeyLog::data_t g_ignored_keys{} ;

    static const auto uninstaller = [](HHOOK* p_hook) {
        if(p_hook == nullptr) return ;
        if(!UnhookWindowsHookEx(*p_hook)) {
            ERROR_PRINT("cannot unhook LowLevelKeyboardProc") ;
        }
        delete p_hook ;
        p_hook = nullptr ;
    } ;

    static unique_ptr<HHOOK, decltype(uninstaller)> p_handle(nullptr, uninstaller) ;

    static LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
        const auto release = [&wParam, &lParam](const int code) {
            return CallNextHookEx(*p_handle, code, wParam, lParam) ;
        } ;

        if(nCode < HC_ACTION) {
            //not processed
            return release(nCode) ;
        }
        const auto code = static_cast<unsigned char>(reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam)->vkCode) ;
        const auto state = (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) ;
        g_state[code] = state ;
        g_state[VKCConverter::get_representative_key(code)] = state ;

        if(!g_ignored_keys.empty()) {
            if(std::find(g_ignored_keys.cbegin(), g_ignored_keys.cend(), code) != g_ignored_keys.cend()) {
                return release(HC_ACTION) ;
            }
        }
        return g_absorbed_flag ? -1 : release(HC_ACTION) ;
    }

    void install_hook() {
        g_state.fill(false) ;

        p_handle.reset(new HHOOK{}) ; //added ownership
        if(p_handle == nullptr) {
            RUNTIME_EXCEPT("Cannot alloc a hook handle") ;
            return ;
        }

        *p_handle = SetWindowsHookEx(
            WH_KEYBOARD_LL,
            static_cast<HOOKPROC>(LowLevelKeyboardProc),
            nullptr, 0
        ) ;

        if(!*p_handle) {
            RUNTIME_EXCEPT("KeyAbosorber's hook handle is null") ;
            return ;
        }
    }

    bool is_pressed(const unsigned char keycode) noexcept {
        if(keycode < 1 || keycode > 254) {
            return false ;
        }
        return g_state[keycode] ;
    }

    const KeyLog get_pressed_list() {
        KeyLog::data_t res{} ;
        for(unsigned char i = 1 ; i < 255 ; i ++) {
            if(is_pressed(i)) res.insert(i) ;
        }
        return KeyLog(res) ;
    }

    //if this object is not hooked, can call following functions.
    bool is_closed() noexcept {
        return g_absorbed_flag ;
    }

    void close() noexcept {
        g_ignored_keys.clear() ;
        g_absorbed_flag = true ;
    }

    void close_with_refresh() {
        g_ignored_keys.clear() ;

        //if this function is called by pressed button,
        //it has to send message "KEYUP" to OS (not absorbed).
        unsigned char vkc = 0 ;
        for(const auto& s : g_state) {
            if(s) KeybrdEventer::release_keystate(vkc) ;
            vkc ++ ;
        }
        g_absorbed_flag = true ;
    }

    void open() noexcept {
        g_ignored_keys.clear() ;
        g_absorbed_flag = false ;
    }

    void open_keys(const KeyLog::data_t& keys) noexcept {
        g_ignored_keys = keys ;
    }

    void open_key(const unsigned char key) noexcept {
        try {g_ignored_keys.insert(key) ;}
        catch(bad_alloc& e) {
            ERROR_PRINT(e.what()) ;
            return ;
        }
    }

    void release_vertually(const unsigned char key) noexcept {
        g_state[key] = false ;
    }
    void press_vertually(const unsigned char key) noexcept {
        g_state[key] = true ;
    }
}
