#ifndef _WX_SYSTEM_TRAY_HPP
#define _WX_SYATEM_TRAY_HPP

#include <wx/taskbar.h>
#include <memory>

namespace wxGUI
{
    class PropDlg ;
    class SystemTray : public wxTaskBarIcon
    {
    private:
        PropDlg* const ppd ; //hooked window

    public:
        SystemTray(const wxString iconpath, const wxString tooltips, PropDlg* const ptr_pd) ;
        virtual ~SystemTray() noexcept ;

        SystemTray(SystemTray&&) = delete ;
        SystemTray& operator=(SystemTray&&) = delete ;
        SystemTray(const SystemTray&) = delete ;
        SystemTray& operator=(const SystemTray&) = delete ;

        virtual wxMenu* CreatePopupMenu() override ;
    } ;
}

#endif
