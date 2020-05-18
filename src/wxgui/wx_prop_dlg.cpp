#include "wx_prop_dlg.hpp"
#include "wx_system_tray.hpp"
#include "msg_logger.hpp"

#include "wx_settings.hpp"
#include "wx_bind_list.hpp"
#include "wx_shortcut_apps.hpp"
#include "wx_options.hpp"
#include "wx_constant.hpp"
#include "wx_path.hpp"

#include <unordered_map>
#include <string>
#include <sstream>

namespace wxGUI
{
    constexpr auto TITLE = wxT("win-vind Preferences") ;

    struct PropDlg::Impl {
        SettingsPanel* settings = nullptr ;
        BindListPanel* bind_list = nullptr ;
        ShortcutAppsPanel* shortcut_apps = nullptr ;
        OptionsPanel* options = nullptr ;
    } ;

    PropDlg::PropDlg()
    : wxPropertySheetDialog(nullptr, wxID_ANY, TITLE),
      pimpl(std::make_unique<Impl>()),
      ptbi(std::make_unique<SystemTray>(Path::ICON_PATH(), wxT("win-vind"), this))
    {
        SetIcon(wxIcon(Path::ICON_PATH(), wxBITMAP_TYPE_ICO)) ;

        CreateButtons(wxOK | wxCANCEL | wxAPPLY) ;

        pimpl->settings = new SettingsPanel(GetBookCtrl()) ;
        pimpl->bind_list = new BindListPanel(GetBookCtrl()) ;
        pimpl->shortcut_apps = new ShortcutAppsPanel(GetBookCtrl()) ;
        pimpl->options = new OptionsPanel(GetBookCtrl()) ;

        LayoutDialog() ;
        Centre() ;
        SetSize(wxSize(WIDTH, HEIGHT)) ;

        Bind(wxEVT_BUTTON, [this](auto&) {
            save_all() ;
        }, wxID_APPLY) ;

        Bind(wxEVT_BUTTON, [this](auto&) {
            save_all() ;
            Show(false) ;
        }, wxID_OK) ;

        Bind(wxEVT_BUTTON, [this](auto&) {
            Show(false) ;
        }, wxID_CANCEL) ;

        Bind(wxEVT_CLOSE_WINDOW, [this](auto&) {
            Show(false) ;
        }) ;
    }

    PropDlg::~PropDlg() = default ;

    void PropDlg::save_all() {
        pimpl->settings->save_all() ;
        pimpl->bind_list->save_all() ;
        pimpl->shortcut_apps->save_all() ;
        pimpl->options->save_all() ;
    }
}