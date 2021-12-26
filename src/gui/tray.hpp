#ifndef _TRAY_HPP
#define _TRAY_HPP

#include "util/disable_compiler_warning.hpp"

#include <wx/taskbar.h>
#include <wx/toplevel.h>

#include "util/enable_compiler_warning.hpp"

namespace vind
{
    namespace gui
    {
        class SystemTray : public wxTaskBarIcon {
        public:
            explicit SystemTray(
                    wxTopLevelWindow* const parent,
                    const std::string& iconpath,
                    const std::string& tooltips) ;
            virtual wxMenu* CreatePopupMenu() override ;

            virtual ~SystemTray() noexcept ;
        } ;
    }
}

#endif
