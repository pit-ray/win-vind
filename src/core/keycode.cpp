#include "keycode.hpp"

#include "keycode_def.hpp"

#include <windows.h>

#include <array>
#include <unordered_map>
#include <unordered_set>

#ifdef DEBUG
#include <iostream>
#endif


namespace
{
    using namespace vind ;

    enum CodeMask : unsigned short {
        CODE   = 0b0000'0000'1111'1111,

        FLAG   = 0b1111'1111'0000'0000,
        SHIFT  = 0b0000'0001'0000'0000,
        TOGGLE = 0b0000'0010'0000'0000,
        UNREAL = 0b0000'0100'0000'0000,
        NUMBER = 0b0000'1000'0000'0000,
    } ;

    class KeyCodeTable {
    public:
        std::array<unsigned short, 256> key2code_ ;

        std::array<unsigned short, 256> ascii2code_ ;
        std::array<char, 65535> code2ascii_ ;

        std::array<std::string, 65535> code2name_ ;
        std::unordered_map<std::string, unsigned short> name2code_ ;

        std::array<unsigned short, 65535> code2repre_ ;
        std::array<unsigned short, 65535> code2physial_ ;

    private:
        KeyCodeTable()
        : key2code_(),
          ascii2code_(),
          code2ascii_(),
          code2name_(),
          name2code_(),
          code2repre_(),
          code2physial_()
        {
            std::array<char, 256> c2a ;
            std::array<unsigned char, 256> a2c ;
            std::array<char, 256> s_c2a ;
            std::array<unsigned char, 256> s_a2c ;

            const auto data = {
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
            std::array<std::string, 256> magic_ascii ;
            magic_ascii[' '] = "space" ;
            magic_ascii['-'] = "hbar" ;
            magic_ascii['>'] = "gt" ;
            magic_ascii['<'] = "lt" ;

            for(auto c : data) {
                auto res = VkKeyScanA(c) ;

                auto keycode = static_cast<unsigned short>(res & 0xff) ;
                auto shifted = (res & 0x0100) != 0 ;

                if(shifted) {
                    s_a2c[c] = keycode ;
                    s_c2a[keycode] = c ;
                }
                else {
                    a2c[c] = keycode ;
                    c2a[keycode] = c ;
                }
            }

            std::array<bool, 256> togglable ;
            togglable[KEYCODE_CAPSLOCK] = true ;
            togglable[KEYCODE_KANA]     = true ;
            togglable[KEYCODE_NUMLOCK]  = true ;
            togglable[KEYCODE_FROM_JP]  = true ;
            togglable[KEYCODE_TO_EN]    = true ;
            togglable[KEYCODE_IME]      = true ;

            std::array<unsigned char, 256> p2r ;
            std::array<bool, 256> represent ;
            p2r[KEYCODE_LSHIFT]  = KEYCODE_SHIFT ;
            p2r[KEYCODE_RSHIFT]  = KEYCODE_SHIFT ;
            p2r[KEYCODE_LCTRL]   = KEYCODE_CTRL ;
            p2r[KEYCODE_RCTRL]   = KEYCODE_CTRL ;
            /* @Double-Win-Key
            p2r[KEYCODE_LWIN]    = KEYCODE_WIN ;
            p2r[KEYCODE_RWIN]    = KEYCODE_WIN ;
            */
            p2r[KEYCODE_LALT]    = KEYCODE_ALT ;
            p2r[KEYCODE_RALT]    = KEYCODE_ALT ;
            p2r[KEYCODE_FROM_EN] = KEYCODE_IME ;
            p2r[KEYCODE_TO_JP]   = KEYCODE_IME ;

            for(unsigned char i = 1 ; i < 255 ; i ++) {
                if(auto r = p2r[i]) {
                    represent[r] = true ;
                }
            }

            const std::unordered_map<std::string, unsigned char> n2c {
                {"ime",         KEYCODE_IME},
                {"ime1",        KEYCODE_FROM_EN},
                {"ime2",        KEYCODE_TO_JP},
                {"tab",         KEYCODE_TAB},
                {"cr",          KEYCODE_ENTER},
                {"enter",       KEYCODE_ENTER},
                {"return",      KEYCODE_ENTER},
                {"shcapslock",  KEYCODE_SHIFT_CAPSLOCK},
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

                {"win",         KEYCODE_LWIN}, // @Double-Win-Key
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
                {"kanji",       KEYCODE_KANJI},
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

                {"numlock",     KEYCODE_NUMLOCK},
                {"backtab",     KEYCODE_BACKTAB},
                {"bktab",       KEYCODE_BACKTAB}
            } ;

            std::array<std::unordered_set<std::string>, 256> c2ns ;
            for(const auto& [n, c] : n2c) {
                c2ns[c].insert(n) ;
            }

            for(unsigned short keycode = 0 ; keycode < 256 ; keycode ++) {
                auto code = keycode ;

                if(togglable[keycode]) {
                    code |= CodeMask::TOGGLE ;
                }

                if(represent[keycode]) {
                    code |= CodeMask::UNREAL ;
                }

                if(KEYCODE_0 <= code && code <= KEYCODE_9) {
                    code |= CodeMask::NUMBER ;
                }

                char ascii = 0 ;
                if(c2a[keycode]) {
                    ascii = c2a[keycode] ;
                }
                else if(s_c2a[keycode]) {
                    code |= CodeMask::SHIFT ;
                    ascii = s_c2a[keycode] ;
                }

                key2code_[keycode] = code ;

                if(ascii) {
                    ascii2code_[ascii] = code ;
                    code2ascii_[code] = ascii ;

                    auto name = magic_ascii[ascii] ;
                    if(name.empty()) {
                        char s[] = {ascii, '\0'} ;
                        name = s ;
                        std::cout << name << std::endl ;
                    }
                    code2name_[code] = name ;
                    name2code_.emplace(name, code) ;
                }
                else {
                    auto nameset = c2ns[keycode] ;
                    if(!nameset.empty()) {
                        std::size_t maxlen = 0 ;
                        std::string longest ;
                        for(const auto& name : nameset) {
                            code2name_[code] = name ;

                            if(name.length() > maxlen) {
                                maxlen = name.length() ;
                                longest = name ;
                            }
                        }

                        code2name_[code] = longest ;
                        name2code_.emplace(longest, code) ;
                    }
                }
            }
        }

        virtual ~KeyCodeTable() noexcept = default ;

    public:
        static KeyCodeTable& get_instance() {
            static KeyCodeTable instance ;
            return instance ;
        }
    } ;
}


namespace vind
{
    namespace core
    {
        /**
         * NOTE:
         * The keycode is flagged and stored as unsigned short.
         * Most of the conversions are done using lookup tables
         * with arrays to speed up the access rather than memory usage.
         */
        KeyCode::KeyCode()
        : code_(0)
        {}

        KeyCode::KeyCode(char ascii)
        : code_(KeyCodeTable::get_instance().ascii2code_[ascii])
        {}

        KeyCode::KeyCode(unsigned char keycode)
        : code_(KeyCodeTable::get_instance().key2code_[keycode])
        {}

        KeyCode::KeyCode(unsigned short code)
        : code_(code)
        {}

        char KeyCode::to_ascii() const noexcept {
            return KeyCodeTable::get_instance().code2ascii_[code_] ;
        }

        int KeyCode::to_number() const noexcept {
            return get() - KEYCODE_0 ;
        }

        KeyCode KeyCode::to_representative() const noexcept {
            return KeyCodeTable::get_instance().code2repre_[code_] ;
        }

        KeyCode KeyCode::to_physical() const noexcept {
            return KeyCodeTable::get_instance().code2physial_[code_] ;
        }

        bool KeyCode::is_ascii() const noexcept {
            return KeyCodeTable::get_instance().code2ascii_[code_] != 0 ;
        }

        bool KeyCode::is_shifted() const noexcept {
            return static_cast<bool>(code_ & CodeMask::SHIFT) ;
        }

        bool KeyCode::is_unreal() const noexcept {
            return static_cast<bool>(code_ & CodeMask::UNREAL) ;
        }

        bool KeyCode::is_number() const noexcept {
            return static_cast<bool>(code_ & CodeMask::NUMBER) ;
        }

        bool KeyCode::is_toggle() const noexcept {
            return static_cast<bool>(code_ & CodeMask::TOGGLE) ;
        }

        bool KeyCode::has_code() const noexcept {
            return code_ != 0 ;
        }

        const std::string& KeyCode::name() const noexcept {
            return KeyCodeTable::get_instance().code2name_[code_] ;
        }

        unsigned char KeyCode::get() const noexcept {
            return static_cast<unsigned char>(code_ & CodeMask::CODE) ;
        }

        KeyCode::operator bool() const noexcept {
            return code_ != 0 ;
        }

        KeyCode::operator char() const noexcept {
            return to_ascii() ;
        }

        KeyCode::operator unsigned char() const noexcept {
            return static_cast<unsigned char>(code_ & CodeMask::CODE) ;
        }

        KeyCode::operator int() const noexcept {
            return code_ & CodeMask::CODE ;
        }

        KeyCode::operator std::string() const noexcept {
            return name() ;
        }

        KeyCode::operator const char*() const noexcept {
            return name().c_str() ;
        }

        bool KeyCode::operator!() const noexcept {
            return !has_code() ;
        }
    }
}
