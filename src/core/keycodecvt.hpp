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
        template <typename T>
        inline auto get_ascii(T keycode) noexcept {
            return get_ascii(static_cast<KeyCode>(keycode)) ;
        }

        KeyCode get_shifted_keycode(char ascii) noexcept ;

        char get_shifted_ascii(KeyCode keycode) noexcept ;
        template <typename T>
        inline auto get_shifted_ascii(T keycode) noexcept {
            return get_shifted_ascii(static_cast<KeyCode>(keycode)) ;
        }

        KeyCode get_sys_keycode(const std::string& strkey) noexcept ;
        std::unordered_set<KeyCode> get_all_sys_keycode() ;

        std::string get_keycode_name(KeyCode keycode) noexcept ; //for debug
        template <typename T>
        inline auto get_keycode_name(T keycode) noexcept {
            return get_keycode_name(static_cast<KeyCode>(keycode)) ;
        }

        KeyCode get_keycode_of_magic(const std::string& str) ;

        KeyCode get_representative_key(KeyCode key) ;
        template <typename T>
        inline auto get_representative_key(T key) {
            return get_representative_key(static_cast<KeyCode>(key)) ;
        }

        bool is_unreal_key(KeyCode key) noexcept ;
        template <typename T>
        inline auto is_unreal_key(T key) noexcept {
            return is_unreal_key(static_cast<KeyCode>(key)) ;
        }

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
