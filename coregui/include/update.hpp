#ifndef _UPDATE_HPP
#define _UPDATE_HPP

#include "disable_gcc_warning.hpp"

#include <wx/dialog.h>

#include "enable_gcc_warning.hpp"

#include <memory>
#include <string>


namespace vindgui
{
    class UpdateDialog : public wxDialog
    {
    public:
        explicit UpdateDialog(
            wxWindow* parent, 
            const std::string& iconpath,
            int font_size,
            const std::string& font_name) ;
    } ;
}

#endif
