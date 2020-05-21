#ifndef _WX_CONSTANT_HPP
#define _WX_CONSTANT_HPP
#include <unordered_map>

namespace wxGUI
{
    static constexpr auto WIDTH  = 800 ;
    static constexpr auto HEIGHT = 494 ;
    static constexpr auto BORDER = 5 ;

    template <typename T>
    using ums_t = std::unordered_map<std::string, T> ;

    enum class Language : unsigned char {
        English,
        Japanese,
    } ;

    enum class IconStyle : unsigned char {
        DARK,
        LIGHT,
    } ;
}

#endif