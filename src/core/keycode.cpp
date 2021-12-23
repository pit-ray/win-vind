#include "keycode.hpp"

#include "keycode_def.hpp"
#include "util/string.hpp"

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

            const auto printable_ascii = {
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

            for(auto c : printable_ascii) {
                auto res = VkKeyScanA(c) ;

                auto keycode = static_cast<unsigned char>(res & 0x00ff) ;
                auto shifted = (res & 0x0100) != 0x0000 ;

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

            std::array<unsigned char, 256> r2p ;
            r2p[KEYCODE_SHIFT] = KEYCODE_LSHIFT ;
            r2p[KEYCODE_CTRL]  = KEYCODE_LCTRL ;
            r2p[KEYCODE_ALT]   = KEYCODE_LALT ;
            r2p[KEYCODE_IME]   = KEYCODE_FROM_EN ;

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

            std::array<std::string, 256> magic_ascii ;
            magic_ascii[' '] = "space" ;
            magic_ascii['-'] = "hbar" ;
            magic_ascii['>'] = "gt" ;
            magic_ascii['<'] = "lt" ;

            for(unsigned short keycode = 0 ; keycode < 256 ; keycode ++) {
                auto code = keycode ;

                if(togglable[keycode]) {
                    code |= CodeMask::TOGGLE ;
                }

                if(r2p[keycode]) {
                    code |= CodeMask::UNREAL ;
                }

                auto nameset = c2ns[keycode] ;
                if(!nameset.empty()) {  // Non-ascii code
                    key2code_[keycode] = code ;

                    std::size_t maxlen = 0 ;
                    std::string longest ;
                    for(const auto& name : nameset) {
                        name2code_.emplace(name, code) ;

                        if(name.length() > maxlen) {
                            maxlen = name.length() ;
                            longest = name ;
                        }
                    }
                    code2name_[code] = longest ;
                }

                auto add_ascii = [this, &magic_ascii](auto code, auto ascii) {
                    if('0' <= ascii && ascii <= '9') {
                        code |= CodeMask::NUMBER ;
                    }

                    key2code_[code & CodeMask::CODE] = code ;

                    ascii2code_[ascii] = code ;
                    code2ascii_[code] = ascii ;

                    auto name = magic_ascii[ascii] ;
                    if(!name.empty()) {
                        name2code_.emplace(name, code) ;
                    }

                    char as[] = {ascii, '\0'} ;
                    code2name_[code] = as ;
                } ;

                // If there are two shift-ascii and ascii,
                // ascii will be registered with priority.
                if(auto s_ascii = s_c2a[keycode]) {
                    add_ascii(code | CodeMask::SHIFT, s_ascii) ;
                }
                if(auto ascii = c2a[keycode]) {
                    add_ascii(code, ascii) ;
                }

            }

            for(unsigned short keycode = 0 ; keycode < 256 ; keycode ++) {
                auto code = key2code_[keycode] ;

                if(auto rep_keycode = p2r[keycode]) {
                    code2repre_[code] = key2code_[rep_keycode] ;
                }

                if(auto phy_keycode = r2p[keycode]) {
                    code2physial_[code] = key2code_[phy_keycode] ;
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

        KeyCode::KeyCode(int keycode)
        : KeyCode(static_cast<unsigned char>(keycode))
        {}

        KeyCode::KeyCode(const std::string& name, bool prefer_ascii)
        : code_(0)
        {
            auto& table = KeyCodeTable::get_instance() ;
            if(prefer_ascii && name.length() == 1) {
                code_ = table.ascii2code_[name[0]] ;
            }
            else {
                code_ = table.name2code_.at(util::A2a(name)) ;
            }
        }

        KeyCode::KeyCode(unsigned short code)
        : code_(code)
        {}

        char KeyCode::to_ascii() const noexcept {
            return KeyCodeTable::get_instance().code2ascii_[code_] ;
        }

        int KeyCode::to_number() const noexcept {
            return to_code() - KEYCODE_0 ;
        }

        unsigned char KeyCode::to_code() const noexcept {
            return static_cast<unsigned char>(code_ & CodeMask::CODE) ;
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

        bool KeyCode::empty() const noexcept {
            return code_ == 0 ;
        }

        const std::string& KeyCode::name() const noexcept {
            return KeyCodeTable::get_instance().code2name_[code_] ;
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
            return code_ == 0 ;
        }

        bool KeyCode::operator==(const KeyCode& rhs) const noexcept {
            return code_ == rhs.code_ ;
        }
        bool KeyCode::operator==(KeyCode&& rhs) const noexcept {
            return code_ == rhs.code_ ;
        }

        bool KeyCode::operator==(char rhs) const noexcept {
            return to_ascii() == rhs ;
        }
        bool KeyCode::operator==(unsigned char rhs) const noexcept {
            return to_code() == rhs ;
        }
        bool KeyCode::operator==(const std::string& rhs) const noexcept {
            return name() == rhs ;
        }
        bool KeyCode::operator==(const char* rhs) const noexcept {
            return name() == rhs ;
        }

        bool KeyCode::operator!=(const KeyCode& rhs) const noexcept {
            return code_ != rhs.code_ ;
        }
        bool KeyCode::operator!=(KeyCode&& rhs) const noexcept {
            return code_ != rhs.code_ ;
        }

        bool KeyCode::operator!=(char rhs) const noexcept {
            return to_ascii() != rhs ;
        }
        bool KeyCode::operator!=(unsigned char rhs) const noexcept {
            return to_code() != rhs ;
        }
        bool KeyCode::operator!=(const std::string& rhs) const noexcept {
            return name() != rhs ;
        }
        bool KeyCode::operator!=(const char* rhs) const noexcept {
            return name() != rhs ;
        }
    }
}
