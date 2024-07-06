#include "inputgate.hpp"

#include "bind/bindedfunc.hpp"
#include "bind/bindinglist.hpp"
#include "bind/saferepeat.hpp"
#include "errlogger.hpp"
#include "keycodedef.hpp"
#include "mode.hpp"
#include "util/container.hpp"
#include "util/debug.hpp"
#include "util/def.hpp"
#include "util/interval_timer.hpp"
#include "util/keystroke_repeater.hpp"
#include "util/winwrap.hpp"

#include <iterator>
#include <windows.h>

#include <array>
#include <chrono>
#include <functional>
#include <queue>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>


#define KEYUP_MASK 0x0001

#define SC_MAPHOOK static_cast<vind::SystemCall>(105)
#define SC_MAPHOOK_REPRD static_cast<vind::SystemCall>(100)


namespace vind
{
    namespace core
    {
        struct InputGate::Impl {
            HHOOK hook_ ;

            // Use bit-based optimization with std::vector<bool>.
            std::vector<bool> lowlevel_state_ ;
            std::vector<bool> real_state_ ;
            std::vector<bool> state_ ;  //Keyboard state win-vind understands.
            std::vector<bool> port_state_ ;

            ModeArray<std::vector<bool>> identity_map_ ;
            ModeArray<std::array<std::vector<KeyCode>, 256>> k2ks_map_ ;

            std::array<std::chrono::system_clock::time_point, 256> timestamps_ ;

            bool absorb_state_ ;

            std::queue<KeySet> pool_ ;
            bool pool_reprd_mode_ ;

            Impl()
            : hook_(nullptr),
              lowlevel_state_(256, false),
              real_state_(256, false),
              state_(256, false),
              port_state_(256, false),
              identity_map_(),
              k2ks_map_(),
              timestamps_(),
              absorb_state_(true),
              pool_(),
              pool_reprd_mode_(false)
            {
                for(std::size_t i = 0 ; i < identity_map_.size() ; i ++) {
                    identity_map_[i].resize(256) ;
                    std::fill(
                        identity_map_[i].begin(),
                        identity_map_[i].end(),
                        false) ;
                }
            }

            ~Impl() noexcept = default ;
            Impl(const Impl&) = delete ;
            Impl& operator=(const Impl&) = delete ;
        } ;

        InputGate::InputGate()
        : pimpl(std::make_unique<Impl>())
        {}

        InputGate::~InputGate() noexcept {
            uninstall_hook() ;
        }

        InputGate& InputGate::get_instance() {
            static InputGate instance ;
            return instance ;
        }

        void InputGate::install_hook() {
            if(!pimpl->hook_) {
                pimpl->hook_ = SetWindowsHookEx(
                    WH_KEYBOARD_LL,
                    &InputGate::hook_proc,
                    NULL, 0
                ) ;
            }

            if(!pimpl->hook_) {
                throw RUNTIME_EXCEPT("KeyAbosorber's hook handle is null") ;
            }
        }

        void InputGate::uninstall_hook() noexcept {
            if(pimpl->hook_) {
                if(!UnhookWindowsHookEx(pimpl->hook_)) {
                    PRINT_ERROR("Cannot unhook LowLevelKeyboardProc") ;
                }
                pimpl->hook_ = nullptr ;
            }

            // prohibit to keep pressing after termination.
            for(unsigned char i = 1 ; i < 255 ; i ++) {
                try {
                    if(pimpl->real_state_[i]) {
                        release_keystate(i) ;
                    }
                }
                catch(const std::exception& e) {
                    PRINT_ERROR(e.what()) ;
                    continue ;
                }
            }
        }

        void InputGate::reconstruct() {
        }

        LRESULT CALLBACK InputGate::hook_proc(int n_code, WPARAM w_param, LPARAM l_param) {
            if(n_code < 0) { //not processed
                return CallNextHookEx(NULL, n_code, w_param, l_param) ;
            }

            auto& self = get_instance() ;

            auto kbd = reinterpret_cast<KBDLLHOOKSTRUCT*>(l_param) ;
            auto code = static_cast<unsigned char>(kbd->vkCode) ;

            auto mode = get_global_mode() ;

            if(!(kbd->flags & LLKHF_INJECTED)) {
                // The message is not generated with SendInput.
                auto state = !(w_param & KEYUP_MASK) ;

                self.pimpl->lowlevel_state_[code] = state ;
                self.pimpl->timestamps_[code] = std::chrono::system_clock::now() ;

                core::KeyCode keycode(code) ;
                if(auto repcode = keycode.to_representative()) {
                    if(self.map_syncstate(repcode, state, mode) \
                            || self.map_syncstate(keycode, state, mode)) {
                        return 1 ;
                    }

                    self.pimpl->real_state_[repcode.to_code()] = state ;
                    self.pimpl->state_[repcode.to_code()]      = state ;
                }
                else if(self.map_syncstate(keycode, state, mode)) {
                    return 1 ;
                }

                self.pimpl->real_state_[code] = state ;
                self.pimpl->state_[code] = state ;
            }

            if(self.pimpl->port_state_[code] \
                    || self.pimpl->identity_map_[static_cast<int>(mode)][code]) {
                return CallNextHookEx(NULL, n_code, w_param, l_param) ;
            }
            if(self.pimpl->absorb_state_) {
                return 1 ;
            }
            return CallNextHookEx(NULL, n_code, w_param, l_param) ;
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
        void InputGate::refresh_toggle_state() {
            static util::IntervalTimer timer(5'000) ;
            if(!timer.is_passed()) {
                return ;
            }

            static auto toggles =[] {
                std::vector<core::KeyCode> buf ;
                for(unsigned char i = 1 ; i < 255 ; i ++) {
                    core::KeyCode k(i) ;
                    if(k.is_toggle()) {
                        buf.push_back(std::move(k)) ;
                    }
                }
                return buf ;
            }() ;

            for(auto k : toggles) {
                if(!pimpl->lowlevel_state_[k.to_code()]) {
                    continue ;
                }

                using namespace std::chrono ;
                if((system_clock::now() - pimpl->timestamps_[k.to_code()]) > 515ms) {
                    // Release mapped keys which is triggered by the toggle key.
                    for(std::size_t m = 0 ; m < mode_num() ; m ++) {
                        map_syncstate(k, false, static_cast<Mode>(m)) ;
                        map_syncstate(k.to_representative(), false, static_cast<Mode>(m)) ;
                    }

                    // Release the toggle key
                    release_keystate(k) ;
                    pimpl->real_state_[k.to_code()] = false ;
                    pimpl->state_[k.to_code()] = false ;
                    pimpl->lowlevel_state_[k.to_code()] = false ;

                    // Also Release representative key
                    release_keystate(k.to_representative()) ;
                    pimpl->real_state_[k.to_representative().to_code()] = false ;
                    pimpl->state_[k.to_representative().to_code()] = false ;
                    pimpl->lowlevel_state_[k.to_representative().to_code()] = false ;
                }
            }
        }

        bool InputGate::is_pressed(KeyCode keycode) noexcept {
            return pimpl->state_[keycode.to_code()] ;
        }
        bool InputGate::is_really_pressed(KeyCode keycode) noexcept {
            return pimpl->real_state_[keycode.to_code()] ;
        }

        CmdUnit InputGate::pressed_list() {
            CmdUnitSet res{} ;
            for(unsigned char i = 1 ; i < 255 ; i ++) {
                if(is_pressed(i)) {
                    res.insert(i) ;
                }
            }
            return CmdUnit(res) ;
        }

        bool InputGate::is_absorbed() noexcept {
            return pimpl->absorb_state_ ;
        }

        void InputGate::absorb() noexcept {
            pimpl->absorb_state_ = true ;
        }
        void InputGate::unabsorb() noexcept {
            pimpl->absorb_state_ = false ;
        }

        void InputGate::close_some_ports(
                std::initializer_list<KeyCode>&& keys) noexcept {
            for(auto k : keys) {
                pimpl->port_state_[k.to_code()] = true ;
            }
        }
        void InputGate::close_some_ports(
                std::initializer_list<KeyCode>::const_iterator begin,
                std::initializer_list<KeyCode>::const_iterator end) noexcept {
            for(auto itr = begin ; itr != end ; itr ++) {
                pimpl->port_state_[itr->to_code()] = false ;
            }
        }

        void InputGate::close_some_ports(
                std::vector<KeyCode>&& keys) noexcept {
            for(auto k : keys) {
                pimpl->port_state_[k.to_code()] = false ;
            }
        }
        void InputGate::close_some_ports(
                std::vector<KeyCode>::const_iterator begin,
                std::vector<KeyCode>::const_iterator end) noexcept {
            for(auto itr = begin ; itr != end ; itr ++) {
                pimpl->port_state_[itr->to_code()] = false ;
            }
        }

        void InputGate::close_some_ports(
                const CmdUnitSet& keys) noexcept {
            for(auto k : keys) {
                pimpl->port_state_[k.to_code()] = false ;
            }
        }

        void InputGate::close_port(KeyCode key) noexcept {
            pimpl->port_state_[key.to_code()] = false ;
        }

        void InputGate::close_all_ports() noexcept {
            std::fill(
                    pimpl->port_state_.begin(),
                    pimpl->port_state_.end(),
                    false) ;
        }

        void InputGate::close_all_ports_with_refresh() {
            // If this function is called by pressed button,
            // it has to send message "KEYUP" to OS (not absorbed).
            for(unsigned char i = 1 ; i < 255 ; i ++) {
                if(pimpl->state_[i]) {
                    // open a port to release the key state.
                    open_port(i) ;
                    release_keystate(i) ;
                }
            }

            std::fill(
                    pimpl->port_state_.begin(),
                    pimpl->port_state_.end(),
                    false) ;
        }

        void InputGate::open_some_ports(
                std::initializer_list<KeyCode>&& keys) noexcept {
            for(auto k : keys) {
                pimpl->port_state_[k.to_code()] = true ;
            }
        }
        void InputGate::open_some_ports(
                std::initializer_list<KeyCode>::const_iterator begin,
                std::initializer_list<KeyCode>::const_iterator end) noexcept {
            for(auto itr = begin ; itr != end ; itr ++) {
                pimpl->port_state_[itr->to_code()] = true ;
            }
        }

        void InputGate::open_some_ports(
                std::vector<KeyCode>&& keys) noexcept {
            for(auto k : keys) {
                pimpl->port_state_[k.to_code()] = true ;
            }
        }
        void InputGate::open_some_ports(
                std::vector<KeyCode>::const_iterator begin,
                std::vector<KeyCode>::const_iterator end) noexcept {
            for(auto itr = begin ; itr != end ; itr ++) {
                pimpl->port_state_[itr->to_code()] = true ;
            }
        }

        void InputGate::open_some_ports(
                const CmdUnitSet& keys) noexcept {
            for(auto k : keys) {
                pimpl->port_state_[k.to_code()] = true ;
            }
        }

        void InputGate::open_port(KeyCode key) noexcept {
            pimpl->port_state_[key.to_code()] = true ;
        }

        void InputGate::release_virtually(KeyCode key) noexcept {
            pimpl->state_[key.to_code()] = false ;
        }
        void InputGate::press_virtually(KeyCode key) noexcept {
            pimpl->state_[key.to_code()] = true ;
        }

        bool InputGate::map_syncstate(KeyCode key, bool press_sync_state, Mode mode) {
            auto target = pimpl->k2ks_map_[static_cast<int>(mode)][key.to_code()] ;
            if(target.empty()) {
                return false;
            }

            if(press_sync_state) {
                open_some_ports(target.begin(), target.end()) ;
                press_keystate(target.begin(), target.end()) ;
                close_some_ports(target.begin(), target.end()) ;
            }
            else {
                open_some_ports(target.begin(), target.end()) ;
                release_keystate(target.begin(), target.end()) ;
                close_some_ports(target.begin(), target.end()) ;
            }
            return true ;
        }

        void InputGate::clear_hotkeys(Mode mode) {
            auto midx = static_cast<int>(mode) ;
            std::fill(
                pimpl->identity_map_[midx].begin(),
                pimpl->identity_map_[midx].end(),
                false) ;
            pimpl->k2ks_map_[midx].fill({}) ;
        }

        void InputGate::register_hotkey(
                const KeyCode& trigger,
                const CmdUnit& target,
                Mode mode) {
            auto trigger_code = trigger.to_physical().to_code() ;

            if(target.size() == 1 && trigger == *target.begin()) {
                pimpl->identity_map_[static_cast<int>(mode)][trigger_code] = true ;
                return ;
            }

            std::vector<KeyCode> keyset(target.begin(), target.end()) ;
            std::sort(keyset.begin(), keyset.end()) ;
            pimpl->k2ks_map_[static_cast<int>(mode)][trigger_code] = std::move(keyset) ;
        }
    } // namespace core
} // namespace vind


namespace
{
    using namespace vind::core ;
    bool is_pressed_actually(KeyCode key) noexcept {
        return GetAsyncKeyState(key.to_code()) & 0x8000 ;
    }
}

namespace vind
{
    namespace core
    {
        struct ScopedKey::Impl {
            INPUT in_ ;
            KeyCode key_ ;

            explicit Impl(KeyCode keycode)
            : in_(),
              key_(keycode)
            {
                in_.type       = INPUT_KEYBOARD ;
                in_.ki.wVk     = static_cast<WORD>(key_.to_code()) ;
                in_.ki.wScan   = static_cast<WORD>(MapVirtualKeyA(in_.ki.wVk, MAPVK_VK_TO_VSC)) ;
            }
        } ;

        ScopedKey::ScopedKey(KeyCode key)
        : pimpl(std::make_unique<Impl>(key))
        {}

        ScopedKey::~ScopedKey() noexcept {
            try {release() ;}
            catch(const std::exception& e) {
                PRINT_ERROR(e.what()) ;
            }
        }

        ScopedKey::ScopedKey(ScopedKey&&)            = default ;
        ScopedKey& ScopedKey::operator=(ScopedKey&&) = default ;

        void ScopedKey::send_event(bool pressed) {
            pimpl->in_.ki.dwFlags = (pressed ? 0 : KEYEVENTF_KEYUP) | extended_key_flag(pimpl->in_.ki.wVk) ;
            if(!SendInput(1, &pimpl->in_, sizeof(INPUT))) {
                throw RUNTIME_EXCEPT("failed sending keyboard event") ;
            }
        }

        void ScopedKey::press() {
            auto& igate = InputGate::get_instance() ;

            igate.open_port(pimpl->key_) ;
            send_event(true) ;
            igate.close_all_ports() ;
            if(!is_pressed_actually(pimpl->key_)) {
                throw RUNTIME_EXCEPT("You sent a key pressing event successfully, but the state of its key was not changed.") ;
            }
        }

        void ScopedKey::release() {
            auto& igate = InputGate::get_instance() ;

            igate.open_port(pimpl->key_) ;
            send_event(false) ;
            igate.close_all_ports() ;
            if(is_pressed_actually(pimpl->key_)) {
                throw RUNTIME_EXCEPT("You sent a key releasing event successfully, but the state of its key was not changed.") ;
            }
        }


        InstantKeyAbsorber::InstantKeyAbsorber()
        : flag_(false)
        {
            auto& igate = InputGate::get_instance() ;
            flag_ = igate.is_absorbed() ;
            igate.close_all_ports_with_refresh() ;
            igate.absorb() ;
        }

        InstantKeyAbsorber::~InstantKeyAbsorber() noexcept {
            try {
                if(!flag_) {
                    InputGate::get_instance().unabsorb() ;
                }
            }
            catch(const std::exception& e) {
                PRINT_ERROR(e.what()) ;
            }
        }
    } // namespace core
} // namespace vind
