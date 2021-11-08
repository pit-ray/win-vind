#include "font.hpp"

#include "util/disable_gcc_warning.hpp"

#include <wx/font.h>
#include <wx/fontenum.h>

#include "util/enable_gcc_warning.hpp"

#include "core/err_logger.hpp"

namespace vind
{
    namespace gui
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
}
