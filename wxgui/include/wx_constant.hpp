#ifndef _WX_CONSTANT_HPP
#define _WX_CONSTANT_HPP

#include "disable_gcc_warning.hpp"
#include <wx/gdicmn.h>
#include "enable_gcc_warning.hpp"

#define GOLDEN_WIDTH_RATIO  (1.618)
#define WIDTH_STATIC_FACTOR (6)

namespace wxGUI
{
    inline int WIDTH() noexcept {
        return static_cast<int>(wxGetDisplayPPI().GetWidth() * GOLDEN_WIDTH_RATIO * WIDTH_STATIC_FACTOR) ;
    }

    inline int HEIGHT() noexcept {
        return wxGetDisplayPPI().GetHeight() * WIDTH_STATIC_FACTOR ;
    }

    static constexpr int BORDER = 5 ;
}

#endif
