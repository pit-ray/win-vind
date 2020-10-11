#ifndef _VKC_CONVERTER_HPP
#define _VKC_CONVERTER_HPP
#include <vector>
#include <string>
#include <unordered_set>

namespace VKCConverter
{
    void load_input_combination() ;

    unsigned char get_sys_vkc(const std::string& strkey) noexcept ;
    unsigned char get_vkc(const char ascii) noexcept ;
    char get_ascii(const unsigned char vkc) noexcept ;

    unsigned char get_shifted_vkc(const char ascii) noexcept ;
    char get_shifted_ascii(const unsigned char vkc) noexcept ;

    const std::unordered_set<unsigned char> get_all_sys_vkc() ;

    unsigned char get_representative_key(const unsigned char key) ;
    bool is_unreal_key(const unsigned char key) noexcept ;
}

#endif
