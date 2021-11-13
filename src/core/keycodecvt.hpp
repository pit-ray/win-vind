#ifndef _KEYCODECVT_HPP
#define _KEYCODECVT_HPP

#include <string>
#include <unordered_set>
#include <vector>

#include "keycode_def.hpp"

namespace vind
{
    namespace core {
        void load_input_combination() ;

        KeyCode get_keycode(char ascii) noexcept ;
        char get_ascii(KeyCode keycode) noexcept ;

        KeyCode get_shifted_keycode(char ascii) noexcept ;
        char get_shifted_ascii(KeyCode keycode) noexcept ;

        KeyCode get_sys_keycode(const std::string& strkey) noexcept ;
        std::unordered_set<KeyCode> get_all_sys_keycode() ;
        std::string get_name(KeyCode keycode) noexcept ; //for debug

        KeyCode get_keycode_of_magic(const std::string& str) ;

        KeyCode get_representative_key(KeyCode key) ;
        bool is_unreal_key(KeyCode key) noexcept ;

        inline bool is_number(KeyCode keycode) noexcept {
            return KEYCODE_0 <= keycode && keycode <= KEYCODE_9 ;
        }

        inline bool is_number_ascii(char ascii) noexcept {
            return '0' <= ascii && ascii <= '9' ;
        }

        template <typename T>
        inline T to_number(KeyCode keycode) noexcept {
            return static_cast<T>(keycode - KEYCODE_0) ;
        }

        template <typename T>
        inline T to_number_ascii(char ascii) noexcept {
            return static_cast<T>(ascii - '0') ;
        }

        inline const std::unordered_set<KeyCode>& get_toggle_keys() {
            static std::unordered_set<KeyCode> tmp {
                KEYCODE_CAPSLOCK,
                KEYCODE_KANA,
                KEYCODE_NUMLOCK,
                KEYCODE_FROM_JP,
                KEYCODE_TO_EN,
                KEYCODE_IME
            } ;
            return tmp ;
        }
    }
}

#endif
