#ifndef _TRAY_HPP
#define _TRAY_HPP

#include "disable_gcc_warning.hpp"

#include <wx/taskbar.h>
#include <wx/toplevel.h>

#include "enable_gcc_warning.hpp"

namespace vindgui
{
    class SystemTray : public wxTaskBarIcon
    {
    public:
        explicit SystemTray(
                wxTopLevelWindow* const parent,
                const std::string& iconpath,
                const std::string& tooltips) ;
        virtual wxMenu* CreatePopupMenu() override ;
    } ;
}

#endif
