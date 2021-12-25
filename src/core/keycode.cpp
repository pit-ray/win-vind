#include "keycode.hpp"

#include "keycode_def.hpp"
#include "util/string.hpp"

#include <windows.h>

#include <array>
#include <initializer_list>
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
        ASCII  = 0b0000'1000'0000'0000,
        NUMBER = 0b0000'0100'0000'0000,
        UNREAL = 0b0000'0010'0000'0000,
        TOGGLE = 0b0000'0001'0000'0000,
    } ;

    class KeyCodeTable {
    public:
        std::array<unsigned short, 256> key2code_ ;

        // Use bit-based optimization with std::vector<bool>.
        std::vector<bool> shifted_ ;

        std::array<unsigned short, 256> ascii2code_ ;
        std::array<char, 65535> code2ascii_ ;
        std::array<char, 65535> code2shascii_ ;

        std::array<std::string, 65535> code2name_ ;
        std::unordered_map<std::string, unsigned short> name2code_ ;

        std::array<unsigned short, 65535> code2repre_ ;
        std::array<unsigned short, 65535> code2physial_ ;

    private:
        KeyCodeTable()
        : key2code_(),
          shifted_(256, false),
          ascii2code_(),
          code2ascii_(),
          code2shascii_(),
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

                    shifted_[c] = true ;
                }
                else {
                    a2c[c] = keycode ;
                    c2a[keycode] = c ;
                }
            }

            std::vector<bool> togglable(256, false) ;
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

            std::vector<bool> unreal(256, false) ;
            unreal[KEYCODE_SHIFT] = true ;
            unreal[KEYCODE_CTRL] = true ;
            unreal[KEYCODE_ALT] = true ;
            unreal[KEYCODE_IME] = true ;
            unreal[KEYCODE_SHIFT_CAPSLOCK] = true ;
            unreal[KEYCODE_OPTIONAL] = true ;
            unreal[KEYCODE_OPTNUMBER] = true ;

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
                {"bktab",       KEYCODE_BACKTAB},

                {"any",         KEYCODE_OPTIONAL},
                {"num",         KEYCODE_OPTNUMBER}
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

            for(unsigned short keycode = 1 ; keycode < 255 ; keycode ++) {
                auto code = keycode ;

                auto nameset = c2ns[keycode] ;
                if(!nameset.empty()) {  // Non-ascii code
                    if(togglable[keycode]) {
                        code |= CodeMask::TOGGLE ;
                    }

                    if(unreal[keycode]) {
                        code |= CodeMask::UNREAL ;
                    }

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

                    continue ;
                }

                code |= CodeMask::ASCII ;

                if(KEYCODE_0 <= keycode && keycode <= KEYCODE_9) {
                    code |= CodeMask::NUMBER ;
                }
                key2code_[keycode] = code ;

                auto ascii = c2a[keycode] ;
                auto s_ascii = s_c2a[keycode] ;

                code2ascii_[code] = ascii ;
                code2shascii_[code] = s_ascii ;

                for(auto a : {s_ascii, ascii}) {
                    if(a == 0) {
                        continue ;
                    }
                    ascii2code_[a] = code ;

                    auto name = magic_ascii[a] ;
                    if(!name.empty()) {
                        name2code_.emplace(name, code) ;
                    }

                    char as[] = {a, '\0'} ;
                    code2name_[code] = as ;
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
                else {
                    code2physial_[code] = code ;
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

        char KeyCode::to_shifted_ascii() const noexcept {
            return KeyCodeTable::get_instance().code2shascii_[code_] ;
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
            return static_cast<bool>(code_ & CodeMask::ASCII) ;
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

        unsigned short KeyCode::get() const noexcept {
            return code_ ;
        }

        KeyCode::operator bool() const noexcept {
            return code_ != 0 ;
        }

        KeyCode::operator char() const noexcept {
            return to_ascii() ; // use non-shifted ascii
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

        bool KeyCode::operator<(const KeyCode& rhs) const noexcept {
            return code_ < rhs.code_ ;
        }
        bool KeyCode::operator>(const KeyCode& rhs) const noexcept {
            return code_ > rhs.code_ ;
        }
        bool KeyCode::operator<=(const KeyCode& rhs) const noexcept {
            return code_ <= rhs.code_ ;
        }
        bool KeyCode::operator>=(const KeyCode& rhs) const noexcept {
            return code_ >= rhs.code_ ;
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

        bool KeyCode::is_shifted(char ascii) noexcept {
            return KeyCodeTable::get_instance().shifted_[ascii] ;
        }

        std::ostream& operator<<(std::ostream& stream, const KeyCode& rhs) {
            stream << rhs.name() ;
            return stream ;
        }

        std::ostream& operator<<(std::ostream& stream, const KeySet& rhs) {
            if(rhs.empty()) {
                return stream ;
            }

            if(rhs.size() == 1) {
                const auto& rhs_f = rhs.front() ;
                if(rhs_f.is_ascii()) {
                    stream << rhs_f ;
                }
                else {
                    stream << "<" << rhs_f << ">" ;
                }

                return stream;
            }

            stream << "<" ;
            for(auto itr = rhs.cbegin() ; itr != rhs.cend() ; itr ++) {
                if(itr != rhs.cbegin()) {
                    stream << "-" ;
                }
                stream << *itr ;
            }
            stream << ">" ;
            return stream;
        }

        std::ostream& operator<<(std::ostream& stream, const Command& rhs) {
            if(!rhs.empty()) {
                for(const auto& keyset : rhs) {
                    stream << keyset ;
                }
            }
            return stream ;
        }

        std::ostream& operator<<(std::ostream& stream, const CommandList& rhs) {
            if(!rhs.empty()) {
                stream << "[" ;
                for(auto itr = rhs.cbegin() ; itr != rhs.cend() ; itr ++) {
                    if(rhs.size() > 1 && itr != rhs.cbegin()) {
                        stream << ", " ;
                    }
                    stream << *itr ;
                }
                stream << "]" ;
            }

            return stream ;
        }
    }
}
