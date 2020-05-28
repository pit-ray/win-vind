#ifndef _WX_CONSTANT_HPP
#define _WX_CONSTANT_HPP
#include <unordered_map>
#include <wx/gdicmn.h>
#include "pref_parser.hpp"

namespace wxGUI
{
    inline static const auto RESOLUTION_PPI_MAGNIFICATION() {
        static auto a = PrefParser::load_propdlg_res() ;
        return a ;
    }

    static const auto WIDTH  = static_cast<int>(wxGetDisplayPPI().GetWidth() * 1.618 * RESOLUTION_PPI_MAGNIFICATION()) ;
    static const auto HEIGHT = wxGetDisplayPPI().GetHeight() * RESOLUTION_PPI_MAGNIFICATION() ;
    static const auto BORDER = 5 ;

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