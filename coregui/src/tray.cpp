#include "tray.hpp"

#include "disable_gcc_warning.hpp"

#include <wx/icon.h>
#include <wx/menu.h>

#include "enable_gcc_warning.hpp"

#include "path.hpp"
#include "util/winwrap.hpp"

#include "startup.hpp"
#include "tray.hpp"
#include "update.hpp"

#ifdef DEBUG
#include "err_logger.hpp"
#endif


namespace
{
    enum : unsigned int {
        MENU_CONFIG = 10001,
        MENU_STARTUP,
        MENU_OPENROOT,
        MENU_UPDATE,
        MENU_ABOUT,
        MENU_EXIT,
    } ;

    bool g_startup = true ;
}

namespace vindgui
{
    SystemTray::SystemTray(
            wxTopLevelWindow* const parent,
            const std::string& iconpath,
            const std::string& tooltips)
    : wxTaskBarIcon()
    {
        wxTaskBarIcon::SetIcon(
                wxIcon(iconpath, wxBITMAP_TYPE_ICO),
                tooltips) ;

        Bind(wxEVT_MENU, [this](auto&) {
            g_startup = !g_startup ;
            if(g_startup) {
                register_to_startup() ;
            }
            else {
                remove_from_startup() ;
            }
        }, MENU_STARTUP) ;

        Bind(wxEVT_MENU, [this](auto&) {
            using namespace vind ;
            util::create_process(
                    path::ROOT_PATH(),
                    "explorer",
                    path::CONFIG_PATH()) ;
        }, MENU_OPENROOT) ;

        Bind(wxEVT_MENU, [parent](auto&) {
            auto dlg = new UpdateDialog(
                    parent,
                    vind::gparams::get_s("icon_style"),
                    vind::gparams::get_i("gui_font_size"),
                    vind::gparams::get_s("gui_font_name")) ;
            dlg->CenterOnScreen() ;
            dlg->Show(true) ;
        }, MENU_UPDATE) ;

        Bind(wxEVT_MENU, [parent](auto&) {
            parent->CentreOnScreen() ;
            parent->Show(true) ;
        }, MENU_ABOUT) ;

        Bind(wxEVT_MENU, [parent](auto&) {
            parent->Destroy() ;
        }, MENU_EXIT) ;
    }

    wxMenu* SystemTray::CreatePopupMenu() {
        auto menu = new wxMenu() ;
        //menu->Append(MENU_CONFIG, wxT("Preferences")) ;
        //menu->AppendSeparator() ;
        menu->AppendCheckItem(MENU_STARTUP, wxT("Startup with Windows")) ;
        menu->AppendSeparator() ;
        menu->Append(MENU_OPENROOT, wxT("Open Root Directory")) ;
        menu->Append(MENU_UPDATE, wxT("Check Update")) ;
        menu->AppendSeparator() ;
        menu->Append(MENU_ABOUT, wxT("About")) ;
        menu->AppendSeparator() ;
        menu->Append(MENU_EXIT, wxT("Exit")) ;

        g_startup = check_if_registered() ;
        menu->Check(MENU_STARTUP, g_startup) ;

        return menu ;
    }
}
