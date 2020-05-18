#ifndef _WX_SHORTCUT_APPS_HPP
#define _WX_SHORTCUT_APPS_HPP

#include <wx/panel.h>
#include <wx/bookctrl.h>
#include <memory>

#include "pref_parser.hpp"

namespace wxGUI
{
    class ShortcutAppsPanel : public wxPanel
    {
    private:
        struct Impl ;
        std::unique_ptr<Impl> pimpl ;

        void load_core(const PrefParser::ums_str_t paths) ;
        void load_default() ;
        void load_all() ;

    public:
        explicit ShortcutAppsPanel(wxBookCtrlBase* const p_book_ctrl) ;
        virtual ~ShortcutAppsPanel() ;

        void save_all() ;

        ShortcutAppsPanel(ShortcutAppsPanel&&) = delete ;
        ShortcutAppsPanel& operator=(ShortcutAppsPanel&&) = delete ;

        ShortcutAppsPanel(const ShortcutAppsPanel&) = delete ;
        ShortcutAppsPanel& operator=(const ShortcutAppsPanel&) = delete ;
    } ;
}

#endif