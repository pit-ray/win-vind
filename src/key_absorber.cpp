#include "key_absorber.hpp"

#include <windows.h>
#include <array>
#include <memory>
#include <iostream>
#include <algorithm>

#include "msg_logger.hpp"
#include "key_log.hpp"

using namespace std ;

namespace KeyAbsorber
{
    static array<bool, 256> state{false} ;
    static bool absorbed_flag{true} ;
    static key_log_t ignored_keys{} ;

    static const auto uninstaller = [](HHOOK* p_hook) {
        if(p_hook == nullptr) return ;

        if(!UnhookWindowsHookEx(*p_hook)) {
            Logger::error_stream << "[Error] windows.h: " \
            << GetLastError()\
            << " (win_key_absorber_impl.cpp)\n" ;
        }

        delete p_hook ;
    } ;

    static unique_ptr<HHOOK, decltype(uninstaller)> p_handle(nullptr, uninstaller) ;

    static LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) noexcept {
        static const auto release = [&wParam, &lParam](const int code){return CallNextHookEx(*p_handle, code, wParam, lParam) ;} ;
        if(nCode < HC_ACTION) {
            //not processed
            return release(nCode) ;
        }
        try {
            const auto pkbs = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam) ;

            state.at(pkbs->vkCode) = (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) ;

            if(!ignored_keys.empty()) {
                if(std::find(ignored_keys.cbegin(), ignored_keys.cend(), pkbs->vkCode)
                    != ignored_keys.cend()) {
                    return release(HC_ACTION) ;
                }
            }

            if(absorbed_flag) {
                return -1 ; //absorbing
            }

            //not absorbing
            return release(HC_ACTION) ;
        }
        catch(out_of_range&) {
            return release(nCode) ;
        }
    }

    bool is_install_hook() noexcept {
        state.fill(false) ;

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
            Logger::error_stream << "[Error] windows.h: " \
            << GetLastError()
            << " (win_key_absorber_impl.cpp)\n" ;
            return false ;
        }
        return true ;
    }

    bool is_down(const unsigned char keycode) noexcept {
        if(keycode < 1 || keycode > 254) {
            return false ;
        }

        try{
            return state.at(keycode) ;
        }
        catch(out_of_range&) {
            return false ;
        }
    }

    const KeyLog get_downed_list() noexcept {
        vector<unsigned char> res{} ;

        for(unsigned char i = 1 ; i < 255 ; i ++) {
            if(is_down(i)) {
                res.push_back(i) ;
            }
        }

        return std::move(KeyLog(res)) ;
    }

    //if this object is not hooked, can call following functions.
    bool is_closed() noexcept {
        return absorbed_flag ;
    }

    void close() noexcept {
        ignored_keys.clear() ;
        absorbed_flag = true ;
    }

    void open() noexcept {
        ignored_keys.clear() ;
        absorbed_flag = false ;
    }

    void open_keys(const key_log_t& keys) noexcept {
        ignored_keys = keys ;
    }

    void open_key(const unsigned char key) noexcept {
        try {
            ignored_keys.push_back(key) ;
        }
        catch(bad_alloc& e) {
            Logger::error_stream << "[Error] " \
            << e.what() << " (key_absorber.cpp::open_key)\n" ;
            return ;
        }
    }
}