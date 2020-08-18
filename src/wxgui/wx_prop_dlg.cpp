#include "wx_prop_dlg.hpp"

#include <unordered_map>
#include <string>
#include <sstream>

#include <windows.h>

#include <wx/button.h>
#include <wx/sizer.h>

#include "wx_settings.hpp"
#include "wx_bind_list.hpp"
#include "wx_shortcut_apps.hpp"
#include "wx_options.hpp"

#include "wx_constant.hpp"
#include "wx_path.hpp"
#include "wx_system_tray.hpp"
#include "msg_logger.hpp"
#include "system.hpp"
#include "ui_translator.hpp"


namespace wxGUI
{
    using namespace UITrans ;
    constexpr auto APP_NAME = wxT("win-vind") ;

    inline static const auto _load_icon_path() {
        switch(PrefParser::load_sticon()) {
            case IconStyle::Dark:
                return "resources/icon32_dark.ico" ;

            case IconStyle::Light:
                return "resources/icon32_light.ico" ;

            default:
                return "resources/icon32_dark.ico" ;
        }
    }

    struct PropDlg::Impl {
        SettingsPanel* settings          = nullptr ;
        BindListPanel* bind_list         = nullptr ;
        ShortcutAppsPanel* shortcut_apps = nullptr ;
        OptionsPanel* options            = nullptr ;
    } ;

    PropDlg::PropDlg()
    : wxPropertySheetDialog(nullptr, wxID_ANY, trans(Label::SysTray_Preferences)),
      pimpl(std::make_unique<Impl>()),
      ptbi(std::make_unique<SystemTray>(_load_icon_path(), APP_NAME, this))
    {
        SetIcon(wxIcon(_load_icon_path(), wxBITMAP_TYPE_ICO)) ;

        pimpl->settings      = new SettingsPanel(GetBookCtrl()) ;
        pimpl->bind_list     = new BindListPanel(GetBookCtrl()) ;
        pimpl->shortcut_apps = new ShortcutAppsPanel(GetBookCtrl()) ;
        pimpl->options       = new OptionsPanel(GetBookCtrl()) ;

        wxSizerFlags flags ;
        flags.Border(wxALL, BORDER) ;

        auto btn_sizer = new wxBoxSizer(wxHORIZONTAL) ;
        btn_sizer->Add(new wxButton(this, wxID_OK,     trans(Label::Pref_OK)),     flags) ;
        btn_sizer->Add(new wxButton(this, wxID_CANCEL, trans(Label::Pref_Cancel)), flags) ;
        btn_sizer->Add(new wxButton(this, wxID_APPLY,  trans(Label::Pref_Apply)),  flags) ;

        flags.Align(wxALIGN_RIGHT) ;
        GetInnerSizer()->Add(btn_sizer, flags) ; //The inner sizer contains the book control and button sizer.

        LayoutDialog() ;
        Centre() ;
        auto size = wxSize(WIDTH, HEIGHT) ;
        SetSize(size) ;

        Bind(wxEVT_BUTTON, [this](auto&) {
            save_all() ;
            System::load_config() ;
        }, wxID_APPLY) ;

        Bind(wxEVT_BUTTON, [this](auto&) {
            save_all() ;
            System::load_config() ;
            Show(false) ;
        }, wxID_OK) ;

        Bind(wxEVT_BUTTON, [this](auto&) {
            Show(false) ;
        }, wxID_CANCEL) ;

        Bind(wxEVT_CLOSE_WINDOW, [this](auto&) {
            Show(false) ;
        }) ;

        Bind(wxEVT_CHAR_HOOK, [](auto& e) {
            switch(e.GetKeyCode()) {
                case WXK_ESCAPE: //disable closing window by ESC
                    return ;

                case WXK_RETURN: //disable pressing OK by Enter
                    return ;

                default:
                    e.Skip() ; //others
                    break ;
            }
        }) ;
    }

    PropDlg::~PropDlg() noexcept = default ;

    void PropDlg::save_all() {
        pimpl->settings->save_all() ;
        pimpl->bind_list->save_all() ;
        pimpl->shortcut_apps->save_all() ;
        pimpl->options->save_all() ;
    }

    bool PropDlg::Show(bool show) {
        if(show) {
            //true is shown. false is hidden.
            pimpl->settings->load_all() ;
            pimpl->bind_list->load_all() ;
            pimpl->shortcut_apps->load_all() ;
            pimpl->options->load_all() ;

            if(!SetForegroundWindow(GetHandle())) {
                WIN_ERROR_STREAM << "Preferences Window was not brought to the foreground\n" ;

            } //shown as most top window
        }

        return wxPropertySheetDialog::Show(show) ;
    }
}