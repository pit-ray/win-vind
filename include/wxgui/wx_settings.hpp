#ifndef _WX_SETTINGS_HPP
#define _WX_SETTINGS_HPP
#include <memory>

#include <wx/panel.h>
#include <wx/bookctrl.h>

#include "pref_parser.hpp"

namespace wxGUI
{
    class SettingsPanel : public wxPanel
    {
    private:
        struct Impl ;
        std::unique_ptr<Impl> pimpl ;

        void load_core(
            const PrefParser::ums_str_t params,
            const std::string kb_path,
            const Language ui_idx,
            const IconStyle ico_idx,
            const unsigned char res_ppi_idx
        ) ;

        void load_default() ;

    public:
        explicit SettingsPanel(wxBookCtrlBase* const p_book_ctrl) ;
        virtual ~SettingsPanel() noexcept ;

        void save_all() ;
        void load_all() ;

        SettingsPanel(SettingsPanel&&) = delete ;
        SettingsPanel& operator=(SettingsPanel&&) = delete ;

        SettingsPanel(const SettingsPanel&) = delete ;
        SettingsPanel& operator=(const SettingsPanel&) = delete ;
    } ;
}

#endif