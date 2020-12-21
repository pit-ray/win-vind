#ifndef _VKC_CONVERTER_HPP
#define _VKC_CONVERTER_HPP
#include <vector>
#include <string>
#include <unordered_set>

#include "virtual_key_fwd.hpp"

namespace VKCConverter
{
    void load_input_combination() ;

    unsigned char get_vkc(const char ascii) noexcept ;
    char get_ascii(const unsigned char vkc) noexcept ;

    unsigned char get_shifted_vkc(const char ascii) noexcept ;
    char get_shifted_ascii(const unsigned char vkc) noexcept ;

    unsigned char get_sys_vkc(const std::string& strkey) noexcept ;
    const std::unordered_set<unsigned char> get_all_sys_vkc() ;
    const std::string get_name(const unsigned char vkc) noexcept ; //for debug

    unsigned char get_representative_key(const unsigned char key) ;
    bool is_unreal_key(const unsigned char key) noexcept ;

    inline bool is_number(const unsigned char vkc) noexcept {
        return VKC_0 <= vkc && vkc <= VKC_9 ;
    }

    inline bool is_number_ascii(const char ascii) noexcept {
        return '0' <= ascii && ascii <= '9' ;
    }

    inline unsigned int to_number(const unsigned char vkc) noexcept {
        return vkc - VKC_0 ;
    }
}

#endif
