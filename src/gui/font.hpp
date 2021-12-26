#ifndef _FONT_HPP
#define _FONT_HPP

#include "util/disable_compiler_warning.hpp"

#include <wx/font.h>

#include "util/enable_compiler_warning.hpp"

#include <string>

namespace vind
{
    namespace gui
    {
        wxFont* create_font(int font_size, const std::string& font_name) ;
    }
}

#endif
