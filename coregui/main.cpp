/*
             _                  _           __
   _      __(_)___       _   __(_)___  ____/ /
  | | /| / / / __ \_____| | / / / __ \/ __  /
  | |/ |/ / / / / /_____/ |/ / / / / / /_/ /
  |__/|__/_/_/ /_/      |___/_/_/ /_/\__,_/

       - Vim Key Binder for Windows -

MIT License

Copyright (c) 2020-2021 pit-ray
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR MENURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <atomic>
#include <memory>

#include "disable_gcc_warning.hpp"

#include <wx/app.h>
#include <wx/button.h>
#include <wx/cmdline.h>
#include <wx/defs.h>
#include <wx/font.h>
#include <wx/fontenum.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/msgdlg.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/taskbar.h>

#include "enable_gcc_warning.hpp"

#include "entry.hpp"
#include "err_logger.hpp"
#include "g_params.hpp"
#include "path.hpp"
#include "util/winwrap.hpp"

#include "version.hpp"


namespace
{
    inline void error_box(const wxString& msg) {
        wxMessageBox(msg, wxT("Error - win-vind"), wxOK | wxICON_EXCLAMATION) ;
    }

    namespace Event {
        enum : unsigned int {
            MENU_CONFIG = 10001,
            MENU_STARTUP,
            MENU_OPENROOT,
            MENU_UPDATE,
            MENU_ABOUT,
            MENU_EXIT,

            DIALOG_CLOSE,
        } ;
    }

    std::atomic_bool g_runnable(true) ;

    std::string g_argument_func{} ;

    bool g_startup = true ;
}

namespace CoreGUI
{ 
    class SystemTray : public wxTaskBarIcon
    {
    public:
        template <typename T1, typename T2, typename T3>
        explicit SystemTray(
                T1 parent,
                T2&& iconpath,
                T3&& tooltips)
        : wxTaskBarIcon()
        {
            wxTaskBarIcon::SetIcon(
                    wxIcon(std::forward<T2>(iconpath), wxBITMAP_TYPE_ICO),
                    std::forward<T3>(tooltips)) ;

            Bind(wxEVT_MENU, [this](auto&) {
                g_startup = !g_startup ;
                if(g_startup) {
                    std::cout << "on\n" ;
                }
                else {
                    std::cout << "off\n" ;
                }
            }, Event::MENU_STARTUP) ;

            Bind(wxEVT_MENU, [this](auto&) {
                using namespace vind ;
                util::create_process(
                        path::ROOT_PATH(),
                        "explorer.exe",
                        path::CONFIG_PATH()) ;
            }, Event::MENU_OPENROOT) ;

            Bind(wxEVT_MENU, [this](auto&) {
                std::cout << "Check Update\n" ;
            }, Event::MENU_UPDATE) ;

            Bind(wxEVT_MENU, [parent](auto&) {
                parent->CentreOnScreen() ;
                parent->Show(true) ;
            }, Event::MENU_ABOUT) ;

            Bind(wxEVT_MENU, [parent](auto&) {
                parent->Destroy() ;
            }, Event::MENU_EXIT) ;
        }

        virtual wxMenu* CreatePopupMenu() override {
            auto menu = new wxMenu() ;
            //menu->Append(Event::MENU_CONFIG, wxT("Preferences")) ;
            //menu->AppendSeparator() ;
            menu->AppendCheckItem(Event::MENU_STARTUP, wxT("Startup with Windows")) ;
            menu->AppendSeparator() ;
            menu->Append(Event::MENU_OPENROOT, wxT("Show Root Directory")) ;
            menu->Append(Event::MENU_UPDATE, wxT("Check Update")) ;
            menu->AppendSeparator() ;
            menu->Append(Event::MENU_ABOUT, wxT("About")) ;
            menu->AppendSeparator() ;
            menu->Append(Event::MENU_EXIT, wxT("Exit")) ;

            menu->Check(Event::MENU_STARTUP, g_startup) ;

            return menu ;
        }
    } ;


    class AboutDialog : public wxDialog
    {
    private:
        std::unique_ptr<SystemTray> systray_ ;

    public:
        template <typename T1, typename T2, typename T3, typename T4>
        explicit AboutDialog(
                T1&& iconpath,
                T2&& tooltips,
                T3&& font_size,
                T4&& font_name)
        : wxDialog(nullptr, wxID_ANY, wxT("About win-vind")),
          systray_(std::make_unique<SystemTray>(this, iconpath, std::forward<T2>(tooltips)))
        {
            SetIcon(wxIcon(iconpath, wxBITMAP_TYPE_ICO)) ;

            auto font = wxFont::New(9, wxFONTFAMILY_TELETYPE, wxFONTFLAG_DEFAULT) ;
            font->SetPointSize(std::forward<T3>(font_size)) ;
            if(wxFontEnumerator().IsValidFacename(font_name)) {
                font->SetFaceName(font_name) ;
            }
            else {
                PRINT_ERROR("The font name " + font_name + " is not available.") ;
            }

            auto root = new wxBoxSizer(wxVERTICAL) ;

            wxSizerFlags flags ;
            flags.Border(wxALL, 30) ;
            flags.Align(wxALIGN_CENTER_HORIZONTAL) ;

            constexpr auto message = wxT(\
                "Version: " WIN_VIND_VERSION
                "\n"
                "\n"
                "License: MIT License (C) 2020-2021 pit-ray") ;

            auto main_text = new wxStaticText(this, wxID_ANY, message) ;
            main_text->SetFont(*font) ;
            root->Add(main_text, flags) ;

            flags.Border(wxALL, 10) ;
            auto btn = new wxButton(this, wxID_CLOSE, wxT("Close")) ;
            btn->SetFont(*font) ;
            root->Add(btn, flags) ;

            Bind(wxEVT_BUTTON, [this](auto&) {
                Show(false) ;
            }, wxID_CLOSE) ;

            SetSizerAndFit(root) ;
        }
    } ;

    //core system is wrought at another thread
    class SystemThread : public wxThread {
    private:
        virtual ExitCode Entry() override {
            while(vind::update() && g_runnable.load()) ;
            return static_cast<ExitCode>(0) ;
        }

    public:
        SystemThread()
        : wxThread(wxTHREAD_DETACHED)
        {}
    } ;


    class App : public wxApp
    {
    private:
        bool OnInit() override {
            try {
                if(!wxApp::OnInit()) {
                    return false ;
                }

                if(!wxTaskBarIcon::IsAvailable()) {
                    wxMessageBox(
                            wxT("Not supported System Tray"),
                            wxT("Warning - win-vind"),
                            wxOK | wxICON_EXCLAMATION) ;
                }

                if(!vind::initialize(g_argument_func)) {
                    return false ;
                }

                auto dlg = new AboutDialog(
                        vind::gparams::get_s("icon_style"),
                        wxT("win-vind"),
                        vind::gparams::get_i("gui_font_size"),
                        vind::gparams::get_s("gui_font_name")) ;

                dlg->Show(false) ;

                //enable opening window by command
                /*
                vind::register_show_window_func([ppd] {
                    //ppd->Show(true) ;
                }) ;
                */

                vind::register_exit_window_func([dlg] {
                    dlg->Destroy() ;
                }) ;
            }
            catch(const std::exception& e) {
                error_box(wxString::FromUTF8(e.what()) \
                        + wxT(" Could not initialize win-vind, so terminate." \
                        + wxT(" (Windows Error Code: ") + std::to_string(GetLastError()) + ")")) ;
            }
            return true ;
        }

        int MainLoop() override {
            //create a new thread for back-ground system
            auto pst = std::make_unique<SystemThread>() ;
            pst->Run() ;
            return wxApp::MainLoop() ;
        }

        void OnInitCmdLine(wxCmdLineParser& parser) override {
            parser.AddOption(
                    wxT("f"),
                    wxT("func"),
                    wxT("FunctionName"),
                    wxCMD_LINE_VAL_STRING,
                    wxCMD_LINE_PARAM_OPTIONAL) ;
        }

        bool OnCmdLineParsed(wxCmdLineParser& parser) override {
            wxString fn ;
            if(parser.Found(wxT("func"), &fn)) {
                g_argument_func = fn.ToStdString() ;
            }
            return true ;
        }

        bool OnExceptionInMainLoop() override {
            try {
                throw ; //Rethrow the current exception.
            }
            catch(const std::exception& e) {
                PRINT_ERROR(e.what()) ;
            }

            return false ; //exit program
        }

        void OnUnhandledException() override {
            try {
                throw ; //Rethrow the current exception.
            }
            catch(const std::exception& e) {
                PRINT_ERROR(e.what()) ;
            }
            //the program is already about to exit.
        }

    public:
        virtual ~App() noexcept {
            if(g_runnable.load()) {
                // Core win_vind is running yet, so terminate core system.
                g_runnable.store(false) ;
            }
        }
    } ;
}


#include "disable_gcc_warning.hpp"

DECLARE_APP(CoreGUI::App) ;
IMPLEMENT_APP(CoreGUI::App) ;

#include "enable_gcc_warning.hpp"
