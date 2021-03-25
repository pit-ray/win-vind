#include "keycodecvt.hpp"

#include "err_logger.hpp"

#include <unordered_map>
#include <utility>
#include <windows.h>
#include <iostream>
#include <array>

namespace
{
    inline const auto& g_printable_ascii() noexcept {
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

    std::array<unsigned char, 256> char2vkc{0} ;
    std::array<char, 256> vkc2char{0} ;
    std::array<unsigned char, 256> shifted_char2vkc{0} ;
    std::array<char, 256> shifted_vkc2char{0} ;
}

namespace vind
{
    namespace keycodecvt {

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

    }
}

namespace
{
    const std::unordered_map<std::string, unsigned char> g_sys_vkc {
        {"ime",         KEYCODE_IME},
        {"ime1",        KEYCODE_FROM_EN},
        {"ime2",        KEYCODE_TO_JP},
        {"tab",         KEYCODE_TAB},
        {"cr",          KEYCODE_ENTER},
        {"enter",       KEYCODE_ENTER},
        {"return",      KEYCODE_ENTER},
        {"capslock",    KEYCODE_CAPSLOCK},
        {"left",        KEYCODE_LEFT},
        {"right",       KEYCODE_RIGHT},
        {"up",          KEYCODE_UP},
        {"down",        KEYCODE_DOWN},
        {"bs",          KEYCODE_BKSPACE},

        {"shift",       KEYCODE_SHIFT},
        {"s",           KEYCODE_SHIFT},
        {"lshift",      KEYCODE_LSHIFT},
        {"ls",          KEYCODE_LSHIFT},
        {"rshift",      KEYCODE_RSHIFT},
        {"rs",          KEYCODE_RSHIFT},

        {"ctrl",        KEYCODE_CTRL},
        {"c",           KEYCODE_CTRL},
        {"lctrl",       KEYCODE_LCTRL},
        {"lc",          KEYCODE_LCTRL},
        {"rctrl",       KEYCODE_RCTRL},
        {"rc",          KEYCODE_RCTRL},

        {"win",         KEYCODE_WIN},
        {"lwin",        KEYCODE_LWIN},
        {"rwin",        KEYCODE_RWIN},

        {"alt",         KEYCODE_ALT},
        {"a",           KEYCODE_ALT},
        {"lalt",        KEYCODE_LALT},
        {"la",          KEYCODE_LALT},
        {"ralt",        KEYCODE_RALT},
        {"ra",          KEYCODE_RALT},

        {"m",           KEYCODE_ALT},
        {"lm",          KEYCODE_LALT},
        {"rm",          KEYCODE_RALT},

        {"nocvt",       KEYCODE_NOCONVERT},
        {"cvt",         KEYCODE_CONVERT},
        {"kana",        KEYCODE_KANA},
        {"app",         KEYCODE_APP},
        {"esc",         KEYCODE_ESC},

        {"f1",          KEYCODE_F1},
        {"f2",          KEYCODE_F2},
        {"f3",          KEYCODE_F3},
        {"f4",          KEYCODE_F4},
        {"f5",          KEYCODE_F5},
        {"f6",          KEYCODE_F6},
        {"f7",          KEYCODE_F7},
        {"f8",          KEYCODE_F8},
        {"f9",          KEYCODE_F9},
        {"f10",         KEYCODE_F10},
        {"f11",         KEYCODE_F11},
        {"f12",         KEYCODE_F12},
        {"f13",         KEYCODE_F13},
        {"f14",         KEYCODE_F14},
        {"f15",         KEYCODE_F15},
        {"f16",         KEYCODE_F16},
        {"f17",         KEYCODE_F17},
        {"f18",         KEYCODE_F18},
        {"f19",         KEYCODE_F19},
        {"f20",         KEYCODE_F20},
        {"f21",         KEYCODE_F21},
        {"f22",         KEYCODE_F22},
        {"f23",         KEYCODE_F23},
        {"f24",         KEYCODE_F24},

        {"snapshot",    KEYCODE_SNAPSHOT},
        {"scroll",      KEYCODE_SCROLL},
        {"pause",       KEYCODE_PAUSE},
        {"insert",      KEYCODE_INSERT},
        {"home",        KEYCODE_HOME},
        {"pageup",      KEYCODE_PAGEUP},
        {"del",         KEYCODE_DELETE},
        {"end",         KEYCODE_END},
        {"pagedown",    KEYCODE_PAGEDOWN},

        {"numlock",     KEYCODE_NUMLOCK}
    } ;

    inline const auto create_related_keys() {
            std::array<unsigned char, 256> a{0} ;
            a[KEYCODE_LSHIFT]  = KEYCODE_SHIFT ;
            a[KEYCODE_RSHIFT]  = KEYCODE_SHIFT ;
            a[KEYCODE_LCTRL]   = KEYCODE_CTRL ;
            a[KEYCODE_RCTRL]   = KEYCODE_CTRL ;
            a[KEYCODE_LWIN]    = KEYCODE_WIN ;
            a[KEYCODE_RWIN]    = KEYCODE_WIN ;
            a[KEYCODE_LALT]    = KEYCODE_ALT ;
            a[KEYCODE_RALT]    = KEYCODE_ALT ;
            a[KEYCODE_FROM_EN] = KEYCODE_IME ;
            a[KEYCODE_TO_JP]   = KEYCODE_IME ;
            return a ;
    }

    inline const auto create_unreal_keys() {
        std::array<bool, 256> a{false} ;
        for(const auto i : create_related_keys()) {
            if(i) a[i] = true ;
        }
        return a ;
    }
}

namespace vind
{
    namespace keycodecvt {
        unsigned char get_sys_vkc(const std::string& strkey) noexcept {
            try {return g_sys_vkc.at(strkey) ;}
            catch(const std::out_of_range&) {return 0 ;}
        }

        const std::unordered_set<unsigned char> get_all_sys_vkc() {
            std::unordered_set<unsigned char> set ;
            for(const auto& i : g_sys_vkc) set.insert(i.second) ;
            return set ;
        }

        //for debug
        const std::string get_name(const unsigned char vkc) noexcept {
            if(auto ascii = get_ascii(vkc)) {
                char s[] = {ascii, '\0'} ;
                return s ;
            }

            for(const auto& sk : g_sys_vkc) {
                if(sk.second == vkc) return sk.first ;
            }

            return "Unknown" ;
        }

        unsigned char get_representative_key(const unsigned char key) {
            static const auto a = create_related_keys() ;
            return a[key] ;
        }

        bool is_unreal_key(const unsigned char key) noexcept {
            static const auto a = create_unreal_keys() ;
            return a[key] ;
        }
    }
}
