#ifndef _KEYCODECVT_HPP
#define _KEYCODECVT_HPP

#include <vector>
#include <string>
#include <unordered_set>

#include "keycode_def.hpp"

namespace vind
{
    namespace keycodecvt {
        void load_input_combination() ;

        unsigned char get_keycode(const char ascii) noexcept ;
        char get_ascii(const unsigned char keycode) noexcept ;

        unsigned char get_shifted_keycode(const char ascii) noexcept ;
        char get_shifted_ascii(const unsigned char keycode) noexcept ;

        unsigned char get_sys_keycode(const std::string& strkey) noexcept ;
        const std::unordered_set<unsigned char> get_all_sys_keycode() ;
        const std::string get_name(const unsigned char keycode) noexcept ; //for debug

        unsigned char get_keycode_of_magic(const std::string& str) ;

        unsigned char get_representative_key(const unsigned char key) ;
        bool is_unreal_key(const unsigned char key) noexcept ;

        inline bool is_number(const unsigned char keycode) noexcept {
            return KEYCODE_0 <= keycode && keycode <= KEYCODE_9 ;
        }

        inline bool is_number_ascii(const char ascii) noexcept {
            return '0' <= ascii && ascii <= '9' ;
        }

        template <typename T>
        inline T to_number(const unsigned char keycode) noexcept {
            return static_cast<T>(keycode - KEYCODE_0) ;
        }

        template <typename T>
        inline T to_number_ascii(const char ascii) noexcept {
            return static_cast<T>(ascii - '0') ;
        }

        inline const std::unordered_set<unsigned char>& get_toggle_keys() {
            static std::unordered_set<unsigned char> tmp {
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
