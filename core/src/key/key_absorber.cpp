#include "key/key_absorber.hpp"

#include "key/keycode_def.hpp"
#include "key/keycodecvt.hpp"
#include "key/log_map.hpp"

#include <windows.h>

#include <algorithm>
#include <array>
#include <iostream>
#include <memory>
#include <unordered_map>

#include "err_logger.hpp"
#include "io/keybrd.hpp"
#include "time/interval_timer.hpp"
#include "util/def.hpp"
#include "util/winwrap.hpp"

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

    std::array<bool, 256> g_pressing_toggles{false} ;
    const auto toggles = vind::keycodecvt::get_toggle_keys() ;

    auto uninstaller = [](HHOOK* p_hook) {
        if(p_hook == nullptr) {
            return ;
        }
        if(*p_hook == NULL) {
            return ;
        }
        if(!UnhookWindowsHookEx(*p_hook)) {
            PRINT_ERROR("cannot unhook LowLevelKeyboardProc") ;
        }
        delete p_hook ;
        p_hook = NULL ;

        // prohibit to keep pressing after termination.
        for(int i = 0 ; i < 256 ; i ++) {
            if(g_real_state[i]) {
                vind::keybrd::release_keystate(i) ;
            }
        }
    } ;

    std::unique_ptr<HHOOK, decltype(uninstaller)> p_handle(NULL, uninstaller) ;

    LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
        if(nCode < HC_ACTION) {
            //not processed
            return CallNextHookEx(*p_handle, nCode, wParam, lParam) ;
        }
        auto kbd = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam) ;
        auto code = static_cast<vind::KeyCode>(kbd->vkCode) ;

        auto repcode = vind::keycodecvt::get_representative_key(code) ;

        auto state = (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) ;

        if(toggles.find(code) != toggles.cend()) {
            g_pressing_toggles[code] = state ;
        }

        if(vind::logmap::do_keycode_map(code, state) \
                || vind::logmap::do_keycode_map(repcode, state)) {
            return -1 ; // remove recived message not to pass other application
        }

        g_real_state[code] = state ;
        g_state[code]      = state ;

        g_real_state[repcode] = state ;
        g_state[repcode]      = state ;

        if(!g_ignored_keys.empty()) {
            if(g_ignored_keys.find(code) != g_ignored_keys.cend()) {
                return CallNextHookEx(*p_handle, HC_ACTION, wParam, lParam) ;
            }
        }

        if(g_absorbed_flag) {
            return -1 ;
        }
        else {
            return CallNextHookEx(*p_handle, HC_ACTION, wParam, lParam) ;
        }
    }
}


namespace vind
{
    namespace keyabsorber
    {
        void install_hook() {
            g_real_state.fill(false) ;
            g_state.fill(false) ;
            g_pressing_toggles.fill(false) ;

            p_handle.reset(new HHOOK(NULL)) ; //added ownership
            if(p_handle == nullptr) {
                throw RUNTIME_EXCEPT("Cannot alloc a hook handle") ;
            }

            *p_handle = SetWindowsHookEx(
                WH_KEYBOARD_LL,
                LowLevelKeyboardProc,
                NULL, 0
            ) ;

            if(*p_handle == NULL) {
                throw RUNTIME_EXCEPT("KeyAbosorber's hook handle is null") ;
            }
        }

        void refresh_toggle_state() {
            /*
            static vind::IntervalTimer timer{100'000} ; //100 ms

            if(!timer.is_passed()) {
                return ;
            }
            */
            for(auto k : toggles) {
                if(!g_pressing_toggles[k]) {
                    if(g_real_state[k]) {
                        g_real_state[k] = false ;
                        g_state[k]      = false ;
                    }
                    logmap::do_keycode_map(k, false) ;
                }
                else {
                    g_pressing_toggles[k] = false ;
                }
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
