#include "key/key_absorber.hpp"

#include "key/keycode_def.hpp"
#include "key/keycodecvt.hpp"

#include <windows.h>

#include <algorithm>
#include <array>
#include <iostream>
#include <memory>
#include <unordered_map>

#include "coreio/err_logger.hpp"
#include "io/keybrd.hpp"
#include "util/def.hpp"

/*Absorber Overview
                       _____
                      /     \      ->     _____
  [User's Keyboard]  /       \           /     \

   ____________                         __________
  |            |                       |          |
  |  Keyboard  |   a key is pressed    |    OS    | [Hooked]
  |____________|   ================>>  |__________| LowLevelKeyboardProc

   __________
  |          |  WM_KEYDOWN       save state as variable
  |    OS    |  WM_SYSKEYDOWN        send no message
  |__________|  =============>>   LowLevelKeyboardProc  =========== Other Application

*/

namespace
{
    std::array<bool, 256> g_real_state{false} ;
    std::array<bool, 256> g_state{false} ;  //Keyboard state win-vind understands.
    bool g_absorbed_flag{true} ;
    vind::KeyLog::Data g_ignored_keys{} ;

    const auto uninstaller = [](HHOOK* p_hook) {
        if(p_hook == nullptr) return ;
        if(!UnhookWindowsHookEx(*p_hook)) {
            PRINT_ERROR("cannot unhook LowLevelKeyboardProc") ;
        }
        delete p_hook ;
        p_hook = nullptr ;
    } ;

    std::unique_ptr<HHOOK, decltype(uninstaller)> p_handle(nullptr, uninstaller) ;

    LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
        if(nCode < HC_ACTION) {
            //not processed
            return CallNextHookEx(*p_handle, nCode, wParam, lParam) ;
        }

        auto code = static_cast<vind::KeyCode>(
                reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam)->vkCode) ;

        auto state = (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) ;
        g_real_state[code] = state ;
        g_state[code]      = state ;

        auto rep = vind::keycodecvt::get_representative_key(code) ;
        g_real_state[rep] = state ;
        g_state[rep]      = state ;

        if(!g_ignored_keys.empty()) {
            if(std::find(g_ignored_keys.cbegin(), g_ignored_keys.cend(), code) != g_ignored_keys.cend()) {
                return CallNextHookEx(*p_handle, HC_ACTION, wParam, lParam) ;
            }
        }

        if(g_absorbed_flag) return -1 ;
        else return CallNextHookEx(*p_handle, HC_ACTION, wParam, lParam) ;
    }
}


namespace vind
{
    namespace keyabsorber
    {
        void install_hook() {
            g_real_state.fill(false) ;
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

        bool is_pressed(KeyCode keycode) noexcept {
            if(keycode < 1 || keycode > 254) {
                return false ;
            }
            return g_state[keycode] ;
        }
        bool is_really_pressed(KeyCode keycode) noexcept {
            if(keycode < 1 || keycode > 254) {
                return false ;
            }
            return g_real_state[keycode] ;
        }

        KeyLog get_pressed_list() {
            KeyLog::Data res{} ;
            for(KeyCode i = 1 ; i < 255 ; i ++) {
                if(is_pressed(i)) res.insert(i) ;
            }
            return KeyLog(res) ;
        }

        //if this object is not hooked, can call following functions.
        bool is_absorbed() noexcept {
            return g_absorbed_flag ;
        }

        void absorb() noexcept {
            g_absorbed_flag = true ;
        }
        void unabsorb() noexcept {
            g_absorbed_flag = false ;
        }

        void close_all_ports() noexcept {
            g_ignored_keys.clear() ;
        }

        void close_all_ports_with_refresh() {
            g_ignored_keys.clear() ;

            //if this function is called by pressed button,
            //it has to send message "KEYUP" to OS (not absorbed).
            KeyCode keycode = 0 ;
            for(const auto& s : g_state) {
                if(s) {
                    keybrd::release_keystate(keycode) ;
                }
                keycode ++ ;
            }
        }

        void open_some_ports(const KeyLog::Data& keys) noexcept {
            g_ignored_keys = keys ;
        }

        void open_port(KeyCode key) noexcept {
            try {g_ignored_keys.insert(key) ;}
            catch(const std::bad_alloc& e) {
                PRINT_ERROR(e.what()) ;
                return ;
            }
        }

        void release_virtually(KeyCode key) noexcept {
            g_state[key] = false ;
        }
        void press_virtually(KeyCode key) noexcept {
            g_state[key] = true ;
        }

        struct InstantKeyAbsorber::Impl {
            bool flag = false ;
        } ;
        InstantKeyAbsorber::InstantKeyAbsorber()
        : pimpl(std::make_unique<Impl>())
        {
            pimpl->flag = is_absorbed() ;
            close_all_ports_with_refresh() ;
            absorb() ;
        }
        InstantKeyAbsorber::~InstantKeyAbsorber() noexcept {
            if(!pimpl->flag) unabsorb() ;
        }
    }
}
