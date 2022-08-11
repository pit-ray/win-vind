#include "keycode.hpp"

#include "keycodedef.hpp"
#include "util/debug.hpp"
#include "util/string.hpp"

#include <windows.h>

#include <array>
#include <cctype>
#include <initializer_list>
#include <limits>
#include <string>
#include <unordered_map>
#include <unordered_set>

#define SHIFTED_SHIFT   (0x0100)
#define SHIFTED_CTRL    (0x0200)
#define SHIFTED_ALT     (0x0400)
#define SHIFTED_HANKAKU (0x0800)


namespace
{
    using namespace vind ;

    enum CodeMask : unsigned short {
        CODE    = 0b0000'0000'1111'1111,
        FLAG    = 0b1111'1111'0000'0000,

        NOMAJORSYS = 0b1000'0000'0000'0000,
        PHYSIC     = 0b0010'0000'0000'0000,
        ONLYONE    = 0b0001'0000'0000'0000,
        TOGGLE     = 0b0000'1000'0000'0000,
        SHIFT      = 0b0000'0001'0000'0000,
    } ;


    class KeyCodeTable {
    public:
        std::array<unsigned short, 256> key2code_ ;

        std::array<std::string, 65535> code2name_ ;
        std::unordered_map<std::string, unsigned short> name2code_ ;

        std::array<unsigned short, 65535> code2repre_ ;
        std::array<unsigned short, 65535> code2physial_ ;

    private:
        KeyCodeTable()
        : key2code_(),
          code2name_(),
          name2code_(),
          code2repre_(),
          code2physial_()
        {
            std::vector<bool> togglable(256, false) ;
            togglable[KEYCODE_CAPSLOCK] = true ;
            togglable[KEYCODE_KANA]     = true ;
            togglable[KEYCODE_NUMLOCK]  = true ;
            togglable[KEYCODE_FROM_JP]  = true ;
            togglable[KEYCODE_TO_EN]    = true ;
            togglable[KEYCODE_IME]      = true ;

            std::array<unsigned char, 256> p2r{} ;
            p2r[KEYCODE_LSHIFT]  = KEYCODE_SHIFT ;
            p2r[KEYCODE_RSHIFT]  = KEYCODE_SHIFT ;
            p2r[KEYCODE_LCTRL]   = KEYCODE_CTRL ;
            p2r[KEYCODE_RCTRL]   = KEYCODE_CTRL ;
            p2r[KEYCODE_LWIN]    = KEYCODE_WIN ;
            p2r[KEYCODE_RWIN]    = KEYCODE_WIN ;
            p2r[KEYCODE_LALT]    = KEYCODE_ALT ;
            p2r[KEYCODE_RALT]    = KEYCODE_ALT ;
            p2r[KEYCODE_FROM_EN] = KEYCODE_IME ;
            p2r[KEYCODE_TO_JP]   = KEYCODE_IME ;

            std::vector<bool> unreal(256, false) ;
            unreal[KEYCODE_SHIFT]          = true ;
            unreal[KEYCODE_CTRL]           = true ;
            unreal[KEYCODE_ALT]            = true ;
            unreal[KEYCODE_IME]            = true ;
            unreal[KEYCODE_WIN]            = true ;
            unreal[KEYCODE_SHIFT_CAPSLOCK] = true ;
            unreal[KEYCODE_OPTIONAL]       = true ;
            unreal[KEYCODE_OPTNUMBER]      = true ;

            std::array<unsigned char, 256> r2p{} ;
            r2p[KEYCODE_SHIFT] = KEYCODE_LSHIFT ;
            r2p[KEYCODE_CTRL]  = KEYCODE_LCTRL ;
            r2p[KEYCODE_WIN]   = KEYCODE_LWIN ;
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

                {"win",         KEYCODE_WIN}, // @Double-Win-Key
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

            std::array<std::unordered_set<std::string>, 256> c2ns{} ;
            for(const auto& [n, c] : n2c) {
                c2ns[c].insert(n) ;
            }

            std::vector<bool> shift(256, false) ;
            shift[KEYCODE_SHIFT]  = true ;
            shift[KEYCODE_RALT]   = true ;
            shift[KEYCODE_LSHIFT] = true ;
            shift[KEYCODE_RSHIFT] = true ;

            for(unsigned short keycode = 1 ; keycode < 255 ; keycode ++) {
                auto code = keycode ;

                // Make the virtual key come first
                // when sorted in ascending order.
                if(!unreal[keycode]) {
                    code |= CodeMask::PHYSIC ;
                }

                auto nameset = c2ns[keycode] ;
                if(!nameset.empty()) {  // Non-ascii code
                    if(togglable[keycode]) {
                        code |= CodeMask::TOGGLE ;
                    }

                    if(shift[keycode]) {
                        code |= CodeMask::SHIFT ;
                    }

                    // When sorted in ascending order,
                    // those with the most representative
                    // keys should come first.
                    if(!p2r[keycode]) {
                        code |= CodeMask::ONLYONE ;
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

                code |= CodeMask::NOMAJORSYS ;
                key2code_[keycode] = code ;
            }

            for(unsigned char keycode = 1 ; keycode < 255 ; keycode ++) {
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
        : KeyCode(char_to_keycode(ascii))
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
            if(name.empty()) {
                return ;
            }

            static std::unordered_map<std::string, char> magic_ascii_{
                {"space", ' '},
                {"hbar",  '-'},
                {"gt",    '>'},
                {"lt",    '<'}
            } ;

            auto& table = KeyCodeTable::get_instance() ;

            if(name.length() == 1) {
                if(prefer_ascii) {
                    KeyCode::operator=(char_to_keycode(name[0])) ;
                }
                else {
                    code_ = table.name2code_.at(util::A2a(name)) ;
                }
            }
            else {
                auto n = util::A2a(name) ;

                if(magic_ascii_.find(n) != magic_ascii_.end()) {
                    KeyCode::operator=(
                            char_to_keycode(magic_ascii_.at(n))) ;
                }
                else {
                    code_ = table.name2code_.at(n) ;
                }
            }
        }

        KeyCode::KeyCode(unsigned short code)
        : code_(code)
        {}

        unsigned char KeyCode::to_code() const noexcept {
            return static_cast<unsigned char>(code_ & CodeMask::CODE) ;
        }

        KeyCode KeyCode::to_representative() const {
            return KeyCodeTable::get_instance().code2repre_[code_] ;
        }

        KeyCode KeyCode::to_physical() const {
            return KeyCodeTable::get_instance().code2physial_[code_] ;
        }

        bool KeyCode::is_major_system() const noexcept {
            return !static_cast<bool>(code_ & CodeMask::NOMAJORSYS) ;
        }

        bool KeyCode::is_unreal() const noexcept {
            return !static_cast<bool>(code_ & CodeMask::PHYSIC) ;
        }

        bool KeyCode::is_toggle() const noexcept {
            return static_cast<bool>(code_ & CodeMask::TOGGLE) ;
        }

        bool KeyCode::is_shift() const noexcept {
            return static_cast<bool>(code_ & CodeMask::SHIFT) ;
        }

        bool KeyCode::empty() const noexcept {
            return code_ == 0 ;
        }

        std::string KeyCode::name() const {
            auto sn = KeyCodeTable::get_instance().code2name_[code_] ;
            if(!sn.empty()) {
                return sn ;
            }
            return keycode_to_unicode(*this) ;
        }

        unsigned short KeyCode::get() const noexcept {
            return code_ ;
        }

        KeyCode::operator bool() const noexcept {
            return code_ != 0 ;
        }

        KeyCode::operator unsigned char() const noexcept {
            return static_cast<unsigned char>(code_ & CodeMask::CODE) ;
        }

        KeyCode::operator int() const noexcept {
            return code_ & CodeMask::CODE ;
        }

        KeyCode::operator std::string() const {
            return name() ;
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

        bool KeyCode::operator==(unsigned char rhs) const noexcept {
            return to_code() == rhs ;
        }
        bool KeyCode::operator==(const std::string& rhs) const {
            return name() == rhs ;
        }
        bool KeyCode::operator==(const char* rhs) const {
            return name() == rhs ;
        }

        bool KeyCode::operator!=(const KeyCode& rhs) const noexcept {
            return code_ != rhs.code_ ;
        }
        bool KeyCode::operator!=(KeyCode&& rhs) const noexcept {
            return code_ != rhs.code_ ;
        }

        bool KeyCode::operator!=(unsigned char rhs) const noexcept {
            return to_code() != rhs ;
        }
        bool KeyCode::operator!=(const std::string& rhs) const {
            return name() != rhs ;
        }
        bool KeyCode::operator!=(const char* rhs) const {
            return name() != rhs ;
        }

        std::ostream& operator<<(std::ostream& stream, const KeyCode& rhs) {
            stream << rhs.name() ;
            return stream ;
        }

        int keycode_to_number(const KeyCode& keycode) noexcept {
            return keycode.to_code() - KEYCODE_0 ;
        }

        std::ostream& operator<<(std::ostream& stream, const KeySet& rhs) {
            if(rhs.empty()) {
                return stream ;
            }

            if(rhs.size() == 1) {
                const auto& rhs_f = rhs.front() ;
                if(rhs_f.is_major_system()) {
                    stream << "<" << rhs_f << ">" ;
                }
                else {
                    stream << rhs_f ;
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


        KeyCode get_shift_keycode(char ascii) {
            auto res = VkKeyScanW(
                    util::s_to_ws(std::string{ascii})[0]) ;
            if((res & 0xff00) == 0xff00) {
                return KeyCode{} ;
            }

            if(res & SHIFTED_ALT) {
                return KeyCode{KEYCODE_RALT} ;
            }
            if(res & SHIFTED_SHIFT) {
                return KeyCode{KEYCODE_SHIFT} ;
            }

            return KeyCode{} ;
        }

        KeyCode char_to_keycode(char ascii) {
            auto res = VkKeyScanW(
                    util::s_to_ws(std::string{ascii})[0]) ;
            auto keycode = static_cast<unsigned char>(res & 0x00ff) ;

            if(keycode == 0xff) {
                return KeyCode{} ;
            }

            return KeyCode{keycode} ;
        }

        bool is_need_deadkey_for_input(char ascii) {
            auto res = VkKeyScanW(
                    util::s_to_ws(std::string{ascii})[0]) ;
            auto vkc = static_cast<unsigned char>(res & 0x00ff) ;
            auto scan = MapVirtualKeyW(vkc, MAPVK_VK_TO_VSC) ;

            std::array<unsigned char, 256> states{} ;

            if(res & SHIFTED_ALT) {
                states[KEYCODE_ALT] = 0x80 ;
            }
            if(res & SHIFTED_SHIFT) {
                states[KEYCODE_SHIFT] = 0x80 ;
            }
            if(res & SHIFTED_CTRL) {
                states[KEYCODE_CTRL] = 0x80 ;
            }

            constexpr std::size_t buf_size = 5 ;
            WCHAR buf[buf_size] = {} ;

            auto result = ToUnicode(
                    vkc, scan, states.data(),
                    buf, buf_size, 0) ;

            return result == -1 ;
        }

        std::string keycode_to_unicode_impl(
                const KeyCode& keycode,
                const std::array<unsigned char, 256>& states) {

            if(keycode.is_major_system()) {
                return std::string{} ;
            }

            auto vkc = keycode.to_code() ;
            auto scan = MapVirtualKeyW(vkc, MAPVK_VK_TO_VSC) ;

            constexpr std::size_t buf_size = 5 ;
            WCHAR buf[buf_size] = {} ;

            auto result = ToUnicode(
                    vkc, scan, states.data(),
                    buf, buf_size, 0) ;
            if(result < 0) {
                // When a dead key is entered, the conversion is
                // performed again, and the dead key itself is
                // output as a single character.
                result = ToUnicode(
                        vkc, scan, states.data(),
                        buf, buf_size, 0) ;
                return util::ws_to_s(std::wstring(buf, 1)) ;
            }

            if(result == 0) {
                return std::string() ;
            }

            return util::ws_to_s(std::wstring(buf, result)) ;
        }
    }
}
