#include "wx_system_tray.hpp"

#include <wx/msgdlg.h>
#include <wx/icon.h>
#include <wx/menu.h>

#include "ui_translator.hpp"
#include "wx_prop_dlg.hpp"

namespace wxGUI
{
    namespace Event {
        enum : unsigned int {
            PU_CONFIG = 10001,
            PU_ABOUT,
            PU_EXIT,
        } ;
    }

    SystemTray::SystemTray(const wxString iconpath, const wxString tooltips, PropDlg* const ptr_pd)
    : wxTaskBarIcon(), ppd(ptr_pd)
    {
        wxTaskBarIcon::SetIcon(wxIcon(iconpath, wxBITMAP_TYPE_ICO), tooltips) ;

        Bind(wxEVT_MENU, [this](auto&) {
            ppd->Show(true) ;
        }, Event::PU_CONFIG) ;

        Bind(wxEVT_MENU, [this](auto&) {
            static const char* const title = "About win-vind" ;
            static const char* const message = \
                "Version: 2.1.1\n"
                "\n"
                "License: MIT License (C) 2020 pit-ray" ;

#if defined(__WXMSW__) && wxUSE_TASKBARICON_BALLOONS
                ShowBalloon(title, message, 15000, wxICON_INFORMATION);
#else
                wxMessageBox(message, title, wxICON_INFORMATION|wxOK);
#endif
        }, Event::PU_ABOUT) ;

        Bind(wxEVT_MENU, [this](auto&) {
            ppd->Destroy() ;
        }, Event::PU_EXIT) ;
    }

    SystemTray::~SystemTray() noexcept = default ;

    wxMenu* SystemTray::CreatePopupMenu() {
        using namespace UITrans ;
        auto menu = new wxMenu ;
        menu->Append(Event::PU_CONFIG, trans("notify/preferences")) ;
        menu->AppendSeparator() ;
        menu->Append(Event::PU_ABOUT, trans("notify/about")) ;
        menu->AppendSeparator() ;
        menu->Append(Event::PU_EXIT, trans("notify/exit")) ;

        return menu ;
    }
}
