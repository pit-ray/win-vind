#ifndef _WX_PANEL_CORE_HPP
#define _WX_PANEL_CORE_HPP

#include <wx/panel.h>
#include <wx/bookctrl.h>

namespace wxGUI
{
    class PanelCore : public wxPanel
    {
    private:
        wxBookCtrlBase* const pbk ;
        const std::string uip ;

        virtual void translate() noexcept = 0 ;
        virtual void do_load_config() noexcept = 0 ;
        virtual void do_save_config() noexcept = 0 ;

        void trans_page() noexcept ;

    public:
        explicit PanelCore(wxBookCtrlBase* const p_book_ctrl, const std::string ui_path_code) ;
        virtual ~PanelCore() noexcept ;

        void load_config() noexcept ;
        void save_config() noexcept ;

        PanelCore(PanelCore&&)                 = delete ;
        PanelCore& operator=(PanelCore&&)      = delete ;
        PanelCore(const PanelCore&)            = delete ;
        PanelCore& operator=(const PanelCore&) = delete ;
    } ;
}

#endif