#ifndef _METRICS_HPP
#define _METRICS_HPP

#include "disable_gcc_warning.hpp"

#include <wx/gdicmn.h>

#include "enable_gcc_warning.hpp"

#define GOLDEN_WIDTH_RATIO  (1.618)

namespace vindgui
{
    wxSize get_golden_size(int factor) {
        return wxSize(
            static_cast<int>(wxGetDisplayPPI().GetWidth() * GOLDEN_WIDTH_RATIO * factor),
            wxGetDisplayPPI().GetHeight() * factor) ;

    }
}

#endif
