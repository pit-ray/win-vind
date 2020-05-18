#ifndef _WX_SETTINGS_HPP
#define _WX_SETTINGS_HPP

#include <wx/panel.h>
#include <wx/bookctrl.h>
#include <memory>

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
            const std::string ui_path,
            const std::string ico_path
        ) ;

        void load_default() ;
        void load_all() ;

    public:
        explicit SettingsPanel(wxBookCtrlBase* const p_book_ctrl) ;
        virtual ~SettingsPanel() ;

        void save_all() ;

        SettingsPanel(SettingsPanel&&) = delete ;
        SettingsPanel& operator=(SettingsPanel&&) = delete ;

        SettingsPanel(const SettingsPanel&) = delete ;
        SettingsPanel& operator=(const SettingsPanel&) = delete ;
    } ;
}

#endif