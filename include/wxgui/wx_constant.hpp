#ifndef _WX_CONSTANT_HPP
#define _WX_CONSTANT_HPP
#include <unordered_map>
#include <wx/gdicmn.h>
#include "io_params.hpp"

namespace wxGUI
{
    inline const auto WIDTH() noexcept {
        static const auto a = static_cast<int>(wxGetDisplayPPI().GetWidth() * 1.618 * ioParams::get_vi("ppi_factor")) ;
        return a ;
    }
    inline const auto HEIGHT() noexcept {
        static const auto a = wxGetDisplayPPI().GetHeight() * ioParams::get_vi("ppi_factor") ;
        return a ;
    }
    static const auto BORDER = 5 ;
}

#endif