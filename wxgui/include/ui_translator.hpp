#ifndef _UI_TRANSLATOR_HPP
#define _UI_TRANSLATOR_HPP

#include <string>

#include "disable_gcc_warning.hpp"
#include <wx/string.h>
#include "enable_gcc_warning.hpp"

namespace UITrans
{
    const wxString trans(const std::string uipath) noexcept ;
}

#endif
