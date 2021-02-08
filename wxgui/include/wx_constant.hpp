#ifndef _WX_CONSTANT_HPP
#define _WX_CONSTANT_HPP

#include "disable_gcc_warning.hpp"
#include <wx/gdicmn.h>
#include "enable_gcc_warning.hpp"

#define GOLDEN_WIDTH_RATIO  (1.618)
#define WIDTH_STATIC_FACTOR (6)

namespace wxGUI
{
    static double g_dpi_scale_factor = 1.0 ;
    inline void enable_high_dpi_support(const double factor) noexcept {
        g_dpi_scale_factor = factor ;
    }

    //Settings > System > Display > Scale and layout
    inline const auto RESCALED_FACTOR() noexcept {
        return g_dpi_scale_factor * WIDTH_STATIC_FACTOR ;
    }

    inline const auto WIDTH() noexcept {
        return static_cast<int>(wxGetDisplayPPI().GetWidth() * GOLDEN_WIDTH_RATIO * RESCALED_FACTOR()) ;
    }

    inline const auto HEIGHT() noexcept {
        return static_cast<int>(wxGetDisplayPPI().GetHeight() * RESCALED_FACTOR()) ;
    }

    static constexpr int BORDER = 5 ;
}

#endif
