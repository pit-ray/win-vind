#ifndef _METRICS_HPP
#define _METRICS_HPP

#include "util/disable_compiler_warning.hpp"

#include <wx/gdicmn.h>

#include "util/enable_compiler_warning.hpp"

#define GOLDEN_WIDTH_RATIO  (1.618)

namespace vind
{
    namespace gui
    {
        wxSize get_golden_size(int factor) {
            return wxSize(
                static_cast<int>(wxGetDisplayPPI().GetWidth() * GOLDEN_WIDTH_RATIO * factor),
                wxGetDisplayPPI().GetHeight() * factor) ;

        }
    }
}

#endif
