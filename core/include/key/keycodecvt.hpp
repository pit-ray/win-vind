#ifndef _KEYCODECVT_HPP
#define _KEYCODECVT_HPP

#include <string>
#include <unordered_set>
#include <vector>

#include "keycode_def.hpp"

namespace vind
{
    namespace keycodecvt {
        void load_input_combination() ;

        unsigned char get_keycode(char ascii) noexcept ;
        char get_ascii(unsigned char keycode) noexcept ;

        unsigned char get_shifted_keycode(char ascii) noexcept ;
        char get_shifted_ascii(unsigned char keycode) noexcept ;

        unsigned char get_sys_keycode(const std::string& strkey) noexcept ;
        std::unordered_set<unsigned char> get_all_sys_keycode() ;
        std::string get_name(unsigned char keycode) noexcept ; //for debug

        unsigned char get_keycode_of_magic(const std::string& str) ;

        unsigned char get_representative_key(unsigned char key) ;
        bool is_unreal_key(unsigned char key) noexcept ;

        inline bool is_number(unsigned char keycode) noexcept {
            return KEYCODE_0 <= keycode && keycode <= KEYCODE_9 ;
        }

        inline bool is_number_ascii(char ascii) noexcept {
            return '0' <= ascii && ascii <= '9' ;
        }

        template <typename T>
        inline T to_number(unsigned char keycode) noexcept {
            return static_cast<T>(keycode - KEYCODE_0) ;
        }

        template <typename T>
        inline T to_number_ascii(char ascii) noexcept {
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
