#include "wx_config_window.hpp"
#include "wx_system_tray.hpp"

#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/stattext.h>

namespace wxGUI
{
    const wxString TITLE("win-vind Preference") ;
    const wxString ICON_PATH("resources/icon32.ico") ;

    ConfigWindow::ConfigWindow()
    : wxFrame(nullptr, wxID_ANY, TITLE),
      ptbi(std::make_unique<SystemTray>(ICON_PATH, wxT("win-vind"), this))
    {
        SetIcon(wxIcon(ICON_PATH, wxBITMAP_TYPE_ICO)) ;

        auto sizer_top = new wxBoxSizer(wxVERTICAL) ;
        wxSizerFlags flags ;
        flags.Border(wxALL, 10) ;

        sizer_top->Add(new wxStaticText(this, wxID_ANY, wxT("Hello World")), flags) ;

        //setup buttons
        auto sizer_btns = new wxBoxSizer(wxHORIZONTAL) ;
        sizer_btns->Add(new wxButton(this, wxID_OK, wxT("Apply")), flags) ;
        sizer_btns->Add(new wxButton(this, wxID_CANCEL, wxT("Cancel")), flags) ;

        sizer_top->Add(sizer_btns, flags.Align(wxALIGN_RIGHT)) ;
        Centre() ;
    }
    ConfigWindow::~ConfigWindow() = default ;

    wxBEGIN_EVENT_TABLE(ConfigWindow, wxFrame)
        EVT_BUTTON(wxID_OK, ConfigWindow::OnApply)
        EVT_BUTTON(wxID_CANCEL, ConfigWindow::OnCancel)
        EVT_CLOSE(ConfigWindow::OnCloseWindow)
    wxEND_EVENT_TABLE()

    void ConfigWindow::OnApply(wxCommandEvent& WXUNUSED(event)) {
        //save config

        Show(false) ;
    }
    
    void ConfigWindow::OnCancel(wxCommandEvent& WXUNUSED(event)) {
        //Close(false) ;
        Show(false) ;
    }

    void ConfigWindow::OnCloseWindow(wxCloseEvent& WXUNUSED(event)) {
        //Close(false) ;
        Show(false) ;
    }
}