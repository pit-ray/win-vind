#ifndef _WX_SETTINGS_HPP
#define _WX_SETTINGS_HPP
#include <memory>

#include "wx_panel_core.hpp"

namespace wxGUI
{
    class SettingsPanel : public PanelCore
    {
    private:
        struct Impl ;
        std::unique_ptr<Impl> pimpl ;
        void translate() noexcept override ;
        void do_load_config() noexcept override ;
        void do_save_config() noexcept override ;

    public:
        explicit SettingsPanel(wxBookCtrlBase* const p_book_ctrl) ;
        virtual ~SettingsPanel() noexcept ;

        SettingsPanel(SettingsPanel&&) = delete ;
        SettingsPanel& operator=(SettingsPanel&&) = delete ;

        SettingsPanel(const SettingsPanel&) = delete ;
        SettingsPanel& operator=(const SettingsPanel&) = delete ;
    } ;
}

#endif