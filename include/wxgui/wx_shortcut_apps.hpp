#ifndef _WX_SHORTCUT_APPS_HPP
#define _WX_SHORTCUT_APPS_HPP

#include <memory>
#include "wx_panel_core.hpp"

namespace wxGUI
{
    class ShortcutAppsPanel : public PanelCore
    {
    private:
        struct Impl ;
        std::unique_ptr<Impl> pimpl ;

        void translate() noexcept override ;
        void do_save_config() noexcept override ;
        void do_load_config() noexcept override ;
    public:
        explicit ShortcutAppsPanel(wxBookCtrlBase* const p_book_ctrl) ;
        virtual ~ShortcutAppsPanel() noexcept ;

        ShortcutAppsPanel(ShortcutAppsPanel&&) = delete ;
        ShortcutAppsPanel& operator=(ShortcutAppsPanel&&) = delete ;

        ShortcutAppsPanel(const ShortcutAppsPanel&) = delete ;
        ShortcutAppsPanel& operator=(const ShortcutAppsPanel&) = delete ;
    } ;
}

#endif
