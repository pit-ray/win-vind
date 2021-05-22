#include "key/key_absorber.hpp"

#include "key/keycode_def.hpp"
#include "key/keycodecvt.hpp"
#include "key/log_map.hpp"

#include <chrono>
#include <windows.h>

#include <algorithm>
#include <array>
#include <iostream>
#include <memory>
#include <unordered_map>

#include "err_logger.hpp"
#include "io/keybrd.hpp"
#include "time/interval_timer.hpp"
#include "time/keystroke_repeater.hpp"
#include "util/debug.hpp"
#include "util/def.hpp"
#include "util/winwrap.hpp"


#define KEYUP_MASK 0x0001


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
    std::array<bool, 256> g_low_level_state{false} ;
    std::array<bool, 256> g_real_state{false} ;
    std::array<bool, 256> g_state{false} ;  //Keyboard state win-vind understands.
    bool g_absorbed_flag{true} ;
    vind::KeyLog::Data g_ignored_keys{} ;

    std::array<std::chrono::system_clock::time_point, 256>
        g_time_stamps{std::chrono::system_clock::now()} ;

    const auto toggles = vind::keycodecvt::get_toggle_keys() ;

    auto uninstaller = [](HHOOK* p_hook) {
        if(p_hook != nullptr) {
            if(*p_hook != NULL) {
                if(!UnhookWindowsHookEx(*p_hook)) {
                    PRINT_ERROR("cannot unhook LowLevelKeyboardProc") ;
                }
                *p_hook = NULL ;
            }

            delete p_hook ;
            p_hook = nullptr ;
        }

        // prohibit to keep pressing after termination.
        for(int i = 0 ; i < 256 ; i ++) {
            if(g_real_state[i]) {
                vind::keybrd::release_keystate(i) ;
            }
        }
    } ;

    std::unique_ptr<HHOOK, decltype(uninstaller)> p_handle(NULL, uninstaller) ;

    LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
        if(nCode < HC_ACTION) { //not processed
            return CallNextHookEx(*p_handle, nCode, wParam, lParam) ;
        }

        auto kbd = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam) ;
        auto code = static_cast<vind::KeyCode>(kbd->vkCode) ;
        auto state = !(wParam & KEYUP_MASK) ;

        g_low_level_state[code] = state ;

        g_time_stamps[code]   = std::chrono::system_clock::now() ;

        if(auto repcode = vind::keycodecvt::get_representative_key(code)) {
            if(vind::logmap::do_keycode_map(repcode, state) ||
                    vind::logmap::do_keycode_map(code, state)) {
                return 1 ;
            }

            g_real_state[repcode] = state ;
            g_state[repcode]      = state ;
        }
        else if(vind::logmap::do_keycode_map(code, state)) {
            return 1 ;
        }

        g_real_state[code] = state ;
        g_state[code]      = state ;

        if(!g_ignored_keys.empty()) {
            if(g_ignored_keys.find(code) != g_ignored_keys.cend()) {
                return CallNextHookEx(*p_handle, HC_ACTION, wParam, lParam) ;
            }
        }

        if(g_absorbed_flag) {
            return 1 ;
        }
        return CallNextHookEx(*p_handle, HC_ACTION, wParam, lParam) ;
    }
}


namespace vind
{
    namespace keyabsorber
    {
        void install_hook() {
            g_real_state.fill(false) ;
            g_state.fill(false) ;

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

        //
        // It makes the toggle keys behave just like any general keys. 
        // However, the current implementation has a small delay in releasing the state.
        // This is because LowLevelKeyboardProc (LLKP) and SetWindowsHookEx can only
        // capture the down event of the toggle key, not the up event, and the strange event cycle.
        // 
        // (1)
        // For example, we press CapsLock for 3 seconds. Then we will get the following signal in LLKP.
        //
        // Signal in LLKP:
        //
        //      ______________________
        // ____|               _ _ _ _
        //     0    1    2    3
        //
        // Thus, LLKP could not capture up event.
        //
        // (2)
        // So, let's use g_low_level_state to detect if the LLKP has been called or not.
        // Then, if the hook is not called for a while, it will release CapsLock.
        //
        // Signal out of LLKP:
        //
        //      __                          ___________________~~~~_____
        // ____|  |__(about 500ms)_________|                   ~~~~     |____
        //     0                                         1     ....  3 (has some delay)
        //
        // The toggle key is sending me a stroke instead of a key state (e.g. h.... hhhhhhh).
        // This means that there will be a buffer time of about 500ms after the first positive signal.
        // If you assign <CapsLock> to <Ctrl> and press <CapsLock-s>, 
        // as there is a high chance of collision during this buffer time, so it's so bad.
        //
        // (3)
        // As a result, I implemented to be released toggle keys
        // when LLKP has not been called for more than the buffer time.
        // This way, even if the key is actually released, if it is less than 500 ms,
        // it will continue to be pressed, causing a delay.
        //
        //
        // Unfortunately, for keymap and normal mode, the flow of key messages needs
        // to be stopped, and this can only be done using Hook.
        // Therefore, this is a challenging task.  If you have any ideas, we welcome pull requests.
        //
        void refresh_toggle_state() {
            static IntervalTimer timer(5'000) ;
            if(!timer.is_passed()) {
                return ;
            }

            for(auto k : toggles) {
                if(!g_low_level_state[k]) {
                    continue ;
                }

                using namespace std::chrono ;
                if((system_clock::now() - g_time_stamps[k]) > 515ms) {
                    logmap::do_keycode_map(k, false) ;
                    keybrd::release_keystate(k) ;

                    g_real_state[k] = false ;
                    g_state[k]      = false ;

                    g_low_level_state[k] = false ;
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
