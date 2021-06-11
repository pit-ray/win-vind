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

#include "disable_gcc_warning.hpp"

#include <wx/app.h>
#include <wx/cmdline.h>
#include <wx/msgdlg.h>

#include "enable_gcc_warning.hpp"

#include "entry.hpp"
#include "err_logger.hpp"
#include "g_params.hpp"

#include "about.hpp"


namespace
{
    inline void error_box(const wxString& msg) {
        wxMessageBox(msg, wxT("Error - win-vind"), wxOK | wxICON_EXCLAMATION) ;
    }

    std::atomic_bool g_runnable(true) ;

    std::string g_argument_func{} ;
}

namespace vindgui
{ 
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

                // Root window
                auto dlg = new vindgui::AboutDialog(
                        vind::gparams::get_s("icon_style"),
                        "win-vind",
                        vind::gparams::get_i("gui_font_size"),
                        vind::gparams::get_s("gui_font_name")) ;

                dlg->Show(false) ;

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
            parser.AddSwitch(
                    wxT("h"),
                    wxT("help"),
                    wxT("Print usage and exit"),
                    wxCMD_LINE_PARAM_OPTIONAL) ;

            parser.AddOption(
                    wxT("f"),
                    wxT("func"),
                    wxT("Identifier of the function to call in one-shot"),
                    wxCMD_LINE_VAL_STRING,
                    wxCMD_LINE_PARAM_OPTIONAL) ;
        }

        bool OnCmdLineParsed(wxCmdLineParser& parser) override {
            wxString fn ;
            if(parser.Found(wxT("help"))) {
                parser.Usage() ;
                return false ;
            }
            else if(parser.Found(wxT("func"), &fn)) {
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

DECLARE_APP(vindgui::App) ;
IMPLEMENT_APP(vindgui::App) ;

#include "enable_gcc_warning.hpp"
