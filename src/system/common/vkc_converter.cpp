#include "vkc_converter.hpp"

#include "virtual_key_fwd.hpp"
#include "msg_logger.hpp"

#include <unordered_map>
#include <utility>
#include <windows.h>
#include <iostream>
#include <array>

using namespace std ;

namespace VKCConverter
{
    inline static const auto& _printable_ascii() noexcept {
        static const auto data = {
            ' ', '!', '\"', '#', '$', '%', '&', '\'', '(', ')',
            '*', '+', ',', '-', '.', '/', '0', '1', '2', '3',
            '4', '5', '6', '7', '8', '9', ':', ';', '<', '=',
            '>', '?', '@', 'A', 'B', 'C', 'D', 'E', 'F', 'G',
            'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q',
            'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '[',
            '\\', ']', '^', '_', '`', 'a', 'b', 'c', 'd', 'e',
            'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o',
            'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y',
            'z', '{', '|', '}', '~'
        } ;
        return data ;
    }

    static std::array<unsigned char, 256> char2vkc{0} ;
    static std::array<char, 256> vkc2char{0} ;
    static std::array<unsigned char, 256> shifted_char2vkc{0} ;
    static std::array<char, 256> shifted_vkc2char{0} ;

    void load_input_combination() noexcept {
        try {
            char2vkc.fill(0) ;
            vkc2char.fill(0) ;
            shifted_char2vkc.fill(0) ;
            shifted_vkc2char.fill(0) ;
        }
        catch(const std::exception& e) {
            ERROR_PRINT(std::string(e.what()) + "failed loading input combination") ;
            return ;
        }

        for(const auto c : _printable_ascii()) {
            const auto res = VkKeyScanA(c) ;

            const auto vkc = static_cast<unsigned char>(res & 0xff) ;
            const auto shifted = (res & 0x0100) != 0 ;

            if(shifted) {
                shifted_char2vkc[static_cast<unsigned char>(c)] = vkc ;
                shifted_vkc2char[vkc] = c ;
            }
            else {
                char2vkc[static_cast<unsigned char>(c)] = vkc ;
                vkc2char[vkc] = c ;
            }
        }
    }

    unsigned char get_vkc(const char ascii) noexcept {
        return char2vkc[static_cast<unsigned char>(ascii)] ;
    }
    char get_ascii(const unsigned char vkc) noexcept {
        return vkc2char[vkc] ;
    }
    unsigned char get_shifted_vkc(const char ascii) noexcept {
        return shifted_char2vkc[static_cast<unsigned char>(ascii)] ;
    }
    char get_shifted_ascii(const unsigned char vkc) noexcept {
        return shifted_vkc2char[vkc] ;
    }

    static const unordered_map<string, unsigned char> _sys_vkc {
        {"IME",         VKC_IME},
        {"IME1",        VKC_FROM_EN},
        {"IME2",        VKC_TO_JP},
        {"Tab",         VKC_TAB},
        {"Enter",       VKC_ENTER},
        {"CapsLock",    VKC_CAPS_LOCK},
        {"Left",        VKC_LEFT},
        {"Right",       VKC_RIGHT},
        {"Up",          VKC_UP},
        {"Down",        VKC_DOWN},
        {"BkSpace",     VKC_BKSPACE},

        {"Shift",       VKC_SHIFT},
        {"LShift",      VKC_LSHIFT},
        {"RShift",      VKC_RSHIFT},

        {"Ctrl",        VKC_CTRL},
        {"LCtrl",       VKC_LCTRL},
        {"RCtrl",       VKC_RCTRL},

        {"Win",         VKC_WIN},
        {"LWin",        VKC_LWIN},
        {"RWin",        VKC_RWIN},

        {"Alt",         VKC_ALT},
        {"LAlt",        VKC_LALT},
        {"RAlt",        VKC_RALT},

        {"NoConvert",   VKC_NOCONVERT},
        {"Convert",     VKC_CONVERT},
        {"Kana",        VKC_KANA},
        {"App",         VKC_APP},
        {"Esc",         VKC_ESC},

        {"F1",          VKC_F1},
        {"F2",          VKC_F2},
        {"F3",          VKC_F3},
        {"F4",          VKC_F4},
        {"F5",          VKC_F5},
        {"F6",          VKC_F6},
        {"F7",          VKC_F7},
        {"F8",          VKC_F8},
        {"F9",          VKC_F9},
        {"F10",         VKC_F10},
        {"F11",         VKC_F11},
        {"F12",         VKC_F12},
        {"F13",         VKC_F13},
        {"F14",         VKC_F14},
        {"F15",         VKC_F15},
        {"F16",         VKC_F16},
        {"F17",         VKC_F17},
        {"F18",         VKC_F18},
        {"F19",         VKC_F19},
        {"F20",         VKC_F20},
        {"F21",         VKC_F21},
        {"F22",         VKC_F22},
        {"F23",         VKC_F23},
        {"F24",         VKC_F24},

        {"SnapShot",    VKC_SNAPSHOT},
        {"Scroll",      VKC_SCROLL},
        {"Pause",       VKC_PAUSE},
        {"Insert",      VKC_INSERT},
        {"Home",        VKC_HOME},
        {"PageUp",      VKC_PAGEUP},
        {"Delete",      VKC_DELETE},
        {"End",         VKC_END},
        {"PageDown",    VKC_PAGEDOWN},

        {"NumLock",     VKC_NUMLOCK}
    } ;

    unsigned char get_sys_vkc(const string& strkey) noexcept {
        try {return _sys_vkc.at(strkey) ;}
        catch(const std::out_of_range&) {return 0 ;}
    }

    const unordered_set<unsigned char> get_all_sys_vkc() {
        unordered_set<unsigned char> set ;
        for(const auto& i : _sys_vkc) set.insert(i.second) ;
        return set ;
    }

    inline static const auto _create_related_keys() {
            std::array<unsigned char, 256> a{0} ;
            a[VKC_LSHIFT]   = VKC_SHIFT ;
            a[VKC_RSHIFT]   = VKC_SHIFT ;
            a[VKC_LCTRL]    = VKC_CTRL ;
            a[VKC_RCTRL]    = VKC_CTRL ;
            a[VKC_LWIN]     = VKC_WIN ;
            a[VKC_RWIN]     = VKC_WIN ;
            a[VKC_LALT]     = VKC_ALT ;
            a[VKC_RALT]     = VKC_ALT ;
            a[VKC_FROM_EN]  = VKC_IME ;
            a[VKC_TO_JP]    = VKC_IME ;
            return a ;
    }
    unsigned char get_representative_key(const unsigned char key) noexcept {
        static const auto a = _create_related_keys() ;
        return a[key] ;
    }

    inline static const auto _create_unreal_keys() {
        std::array<bool, 256> a{false} ;
        for(const auto i : _create_related_keys()) {
            if(i) a[i] = true ;
        }
        return a ;
    }
    bool is_unreal_key(const unsigned char key) noexcept {
        static const auto a = _create_unreal_keys() ;
        return a[key] ;
    }
}
