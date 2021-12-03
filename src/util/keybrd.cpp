#include "keybrd.hpp"

#include <algorithm>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>

#if defined(DEBUG)
#include <iostream>
#endif

#include <windows.h>

#include "core/err_logger.hpp"
#include "core/key_absorber.hpp"
#include "core/key_log.hpp"
#include "core/keycode_def.hpp"
#include "core/keycodecvt.hpp"
#include "util/def.hpp"


namespace vind
{
    namespace util {
        bool is_pressed_actually(KeyCode key) noexcept {
            return GetAsyncKeyState(key) & 0x8000 ;
        }

        struct ScopedKey::Impl {
            INPUT in ;
            KeyCode key ;

            explicit Impl(KeyCode keycode)
            : in(),
              key(keycode)
            {
                in.type     = INPUT_KEYBOARD ;
                in.ki.wVk   = static_cast<WORD>(key) ;
                in.ki.wScan = static_cast<WORD>(MapVirtualKeyA(key, MAPVK_VK_TO_VSC)) ;
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
            pimpl->in.ki.dwFlags = (pressed ? 0 : KEYEVENTF_KEYUP) | extended_key_flag(pimpl->key) ;
            if(!SendInput(1, &pimpl->in, sizeof(INPUT))) {
                throw RUNTIME_EXCEPT("failed sending keyboard event") ;
            }
        }

        void ScopedKey::press() {
            core::open_port(pimpl->key) ;
            send_event(true) ;
            core::close_all_ports() ;
            if(!is_pressed_actually(pimpl->key)) {
                throw RUNTIME_EXCEPT("You sent a key pressing event successfully, but the state of its key was not changed.") ;
            }
        }

        void ScopedKey::release() {
            core::open_port(pimpl->key) ;
            send_event(false) ;
            core::close_all_ports() ;
            if(is_pressed_actually(pimpl->key)) {
                throw RUNTIME_EXCEPT("You sent a key releasing event successfully, but the state of its key was not changed.") ;
            }
        }
    }
}
