#ifndef _WX_CONFIG_WINDOW_HPP
#define _WX_CONFIG_WINDOW_HPP

#include <wx/frame.h>
#include <memory>

namespace wxGUI
{
    class SystemTray ;

    class ConfigWindow : public wxFrame
    {
    private:
        std::unique_ptr<SystemTray> ptbi ;

    public:
        ConfigWindow() ;
        virtual ~ConfigWindow() ;

        ConfigWindow(ConfigWindow&&) = delete ;
        ConfigWindow& operator=(ConfigWindow&&) = delete ;

        ConfigWindow(const ConfigWindow&) = delete ;
        ConfigWindow& operator=(const ConfigWindow&) = delete ;

        void OnApply(wxCommandEvent& event) ;
        void OnCancel(wxCommandEvent& event) ;
        void OnCloseWindow(wxCloseEvent& event) ;

        //virtual bool Destroy() override ;

        wxDECLARE_EVENT_TABLE() ;
    } ;
}

#endif