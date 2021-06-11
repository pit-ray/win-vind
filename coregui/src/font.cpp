#include "font.hpp"

#include "disable_gcc_warning.hpp"

#include <wx/font.h>
#include <wx/fontenum.h>

#include "enable_gcc_warning.hpp"

#include "err_logger.hpp"

namespace vindgui
{
    wxFont* create_font(int font_size, const std::string& font_name) {
        auto font = wxFont::New(9, wxFONTFAMILY_TELETYPE, wxFONTFLAG_DEFAULT) ;
        font->SetPointSize(font_size) ;
        if(wxFontEnumerator().IsValidFacename(font_name)) {
            font->SetFaceName(font_name) ;
        }
        else {
            PRINT_ERROR("The font name " + font_name + " is not available.") ;
        }
        return font ;
    }
}
