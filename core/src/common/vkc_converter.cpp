#include "vkc_converter.hpp"

#include "msg_logger.hpp"

#include <unordered_map>
#include <utility>
#include <windows.h>
#include <iostream>
#include <array>

using namespace std ;

namespace VKCConverter
{
    inline static const auto& g_printable_ascii() noexcept {
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

    void load_input_combination() {
        char2vkc.fill(0) ;
        vkc2char.fill(0) ;
        shifted_char2vkc.fill(0) ;
        shifted_vkc2char.fill(0) ;

        for(const auto c : g_printable_ascii()) {
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

    static const unordered_map<string, unsigned char> g_sys_vkc {
        {"ime",         VKC_IME},
        {"ime1",        VKC_FROM_EN},
        {"ime2",        VKC_TO_JP},
        {"tab",         VKC_TAB},
        {"cr",          VKC_ENTER},
        {"enter",       VKC_ENTER},
        {"return",      VKC_ENTER},
        {"capslock",    VKC_CAPS_LOCK},
        {"left",        VKC_LEFT},
        {"right",       VKC_RIGHT},
        {"up",          VKC_UP},
        {"down",        VKC_DOWN},
        {"bs",          VKC_BKSPACE},

        {"shift",       VKC_SHIFT},
        {"s",           VKC_SHIFT},
        {"lshift",      VKC_LSHIFT},
        {"ls",          VKC_LSHIFT},
        {"rshift",      VKC_RSHIFT},
        {"rs",          VKC_RSHIFT},

        {"ctrl",        VKC_CTRL},
        {"c",           VKC_CTRL},
        {"lctrl",       VKC_LCTRL},
        {"lc",          VKC_LCTRL},
        {"rctrl",       VKC_RCTRL},
        {"rc",          VKC_RCTRL},

        {"win",         VKC_WIN},
        {"lwin",        VKC_LWIN},
        {"rwin",        VKC_RWIN},

        {"alt",         VKC_ALT},
        {"a",           VKC_ALT},
        {"lalt",        VKC_LALT},
        {"la",          VKC_LALT},
        {"ralt",        VKC_RALT},
        {"ra",          VKC_RALT},

        {"m",           VKC_ALT},
        {"lm",          VKC_LALT},
        {"rm",          VKC_RALT},

        {"nocvt",       VKC_NOCONVERT},
        {"cvt",         VKC_CONVERT},
        {"kana",        VKC_KANA},
        {"app",         VKC_APP},
        {"esc",         VKC_ESC},

        {"f1",          VKC_F1},
        {"f2",          VKC_F2},
        {"f3",          VKC_F3},
        {"f4",          VKC_F4},
        {"f5",          VKC_F5},
        {"f6",          VKC_F6},
        {"f7",          VKC_F7},
        {"f8",          VKC_F8},
        {"f9",          VKC_F9},
        {"f10",         VKC_F10},
        {"f11",         VKC_F11},
        {"f12",         VKC_F12},
        {"f13",         VKC_F13},
        {"f14",         VKC_F14},
        {"f15",         VKC_F15},
        {"f16",         VKC_F16},
        {"f17",         VKC_F17},
        {"f18",         VKC_F18},
        {"f19",         VKC_F19},
        {"f20",         VKC_F20},
        {"f21",         VKC_F21},
        {"f22",         VKC_F22},
        {"f23",         VKC_F23},
        {"f24",         VKC_F24},

        {"snapshot",    VKC_SNAPSHOT},
        {"scroll",      VKC_SCROLL},
        {"pause",       VKC_PAUSE},
        {"insert",      VKC_INSERT},
        {"home",        VKC_HOME},
        {"pageup",      VKC_PAGEUP},
        {"del",         VKC_DELETE},
        {"end",         VKC_END},
        {"pagedown",    VKC_PAGEDOWN},

        {"numlock",     VKC_NUMLOCK}
    } ;

    unsigned char get_sys_vkc(const string& strkey) noexcept {
        try {return g_sys_vkc.at(strkey) ;}
        catch(const std::out_of_range&) {return 0 ;}
    }

    const unordered_set<unsigned char> get_all_sys_vkc() {
        unordered_set<unsigned char> set ;
        for(const auto& i : g_sys_vkc) set.insert(i.second) ;
        return set ;
    }

    //for debug
    const std::string get_name(const unsigned char vkc) noexcept {
        if(auto ascii = get_ascii(vkc)) {
            char s[] = {ascii} ;
            return s ;
        }

        for(const auto& sk : g_sys_vkc) {
            if(sk.second == vkc) return sk.first ;
        }

        return "Unknown" ;
    }

    inline static const auto _create_related_keys() {
            std::array<unsigned char, 256> a{0} ;
            a[VKC_LSHIFT]  = VKC_SHIFT ;
            a[VKC_RSHIFT]  = VKC_SHIFT ;
            a[VKC_LCTRL]   = VKC_CTRL ;
            a[VKC_RCTRL]   = VKC_CTRL ;
            a[VKC_LWIN]    = VKC_WIN ;
            a[VKC_RWIN]    = VKC_WIN ;
            a[VKC_LALT]    = VKC_ALT ;
            a[VKC_RALT]    = VKC_ALT ;
            a[VKC_FROM_EN] = VKC_IME ;
            a[VKC_TO_JP]   = VKC_IME ;
            return a ;
    }

    unsigned char get_representative_key(const unsigned char key) {
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
