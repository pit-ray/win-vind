#ifndef _VKC_CONVERTER_HPP
#define _VKC_CONVERTER_HPP
#include <vector>
#include <string>

namespace VKCConverter
{
    void load_input_combination() noexcept ;

    unsigned char get_sys_vkc(const std::string& strkey) noexcept ;
    unsigned char get_vkc(const char ascii) noexcept ;
    char get_ascii(const unsigned char vkc) noexcept ;

    unsigned char get_shifted_vkc(const char ascii) noexcept ;
    char get_shifted_ascii(const unsigned char vkc) noexcept ;

    const std::vector<unsigned char> get_all_sys_vkc() ;
}

#endif