#ifndef _KEYCODE_HPP
#define _KEYCODE_HPP

#include "keycodedef.hpp"
#include "util/debug.hpp"

#include <algorithm>
#include <array>
#include <ostream>
#include <string>
#include <vector>


#define PREFER_ASCII_CODE true
#define PREFER_SYSTEM_CODE false


namespace vind
{
    namespace core
    {
        class KeyCode {
        private:
            unsigned short code_ ;

            KeyCode(unsigned short code_wflag) ;

        public:
            KeyCode() ;
            KeyCode(char ascii) ;
            KeyCode(unsigned char keycode) ;
            KeyCode(int keycode) ;

            /**
             * @param[in] (prefer_ascii) 
             *   If there is a conflict between the name of the 
             *   system keycode and the ascii, the bool value 
             *   determines which takes precedence.
             *
             *   For example, <alt> can also be written as <a>.
             *
             *   KeyCode("a", true)  => KEYCODE_A
             *   KeyCode("a", false) => KEYCODE_ALT
             *
             */
            KeyCode(
                    const std::string& name,
                    bool prefer_ascii=PREFER_ASCII_CODE) ;

            unsigned char to_code() const noexcept ;

            KeyCode to_representative() const ;
            KeyCode to_physical() const ;

            bool is_shift() const noexcept ;
            bool is_major_system() const noexcept ;
            bool is_unreal() const noexcept ;
            bool is_toggle() const noexcept ;

            bool empty() const noexcept ;

            std::string name() const ;

            unsigned short get() const noexcept ;

            operator bool() const noexcept ;
            operator unsigned char() const noexcept ;
            operator int() const noexcept ;
            operator std::string() const ;

            bool operator!() const noexcept ;

            bool operator<(const KeyCode& rhs) const noexcept ;
            bool operator>(const KeyCode& rhs) const noexcept ;
            bool operator<=(const KeyCode& rhs) const noexcept ;
            bool operator>=(const KeyCode& rhs) const noexcept ;

            bool operator==(const KeyCode& rhs) const noexcept ;
            bool operator==(KeyCode&& rhs) const noexcept ;
            bool operator==(unsigned char rhs) const noexcept ;
            bool operator==(const std::string& rhs) const ;
            bool operator==(const char* rhs) const ;

            bool operator!=(const KeyCode& rhs) const noexcept ;
            bool operator!=(KeyCode&& rhs) const noexcept ;
            bool operator!=(unsigned char rhs) const noexcept ;
            bool operator!=(const std::string& rhs) const ;
            bool operator!=(const char* rhs) const ;
        } ;

        std::ostream& operator<<(std::ostream& stream, const KeyCode& rhs) ;

        int keycode_to_number(const KeyCode& keycode) noexcept ;

        KeyCode get_shift_keycode(char ascii) ;

        KeyCode char_to_keycode(char ascii) ;

        bool is_need_deadkey_for_input(char ascii) ;

        std::string keycode_to_unicode_impl(
                const KeyCode& keycode,
                const std::array<unsigned char, 256>& states) ;

        template <typename KeyCodeSet>
        inline auto keycode_to_unicode(const KeyCode& keycode, KeyCodeSet&& set) {
            std::array<unsigned char, 256> states{} ;
            if(!set.empty()) {
                for(const auto& key : set) {
                    states[key.to_code()] = 0x80 ;

                    // The RAlt key is converted internally into Ctrl+Alt.
                    if(key == KEYCODE_RALT) {
                        states[KEYCODE_ALT]  = 0x80 ;
                        states[KEYCODE_CTRL] = 0x80 ;
                    }
                }
            }
            return keycode_to_unicode_impl(keycode, states) ;
        }

        inline auto keycode_to_unicode(const KeyCode& keycode) {
            static std::array<unsigned char, 256> states{} ;
            return keycode_to_unicode_impl(keycode, states) ;
        }

        using KeySet = std::vector<KeyCode> ;
        using Command = std::vector<KeySet> ;
        using CommandList = std::vector<Command> ;
    }
}


namespace std {
    template <>
    struct hash<vind::core::KeyCode> {
        size_t operator()(const vind::core::KeyCode& rhs) const {
            return static_cast<size_t>(rhs.get()) ;
        }
    } ;
}

#endif
