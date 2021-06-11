#ifndef _FONT_HPP
#define _FONT_HPP

#include "disable_gcc_warning.hpp"

#include <wx/font.h>

#include "enable_gcc_warning.hpp"

#include <string>

namespace vindgui
{
    wxFont* create_font(int font_size, const std::string& font_name) ;
}

#endif
