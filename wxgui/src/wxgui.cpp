#include "wxgui.hpp"

#include <windows.h>
#include <iostream>
#include <atomic>

#include "disable_gcc_warning.hpp"
#include <wx/taskbar.h>
#include <wx/msgdlg.h>
#include <wx/thread.h>
#include <wx/cmdline.h>
#include <wx/string.h>
#include "enable_gcc_warning.hpp"

#include "utility.hpp"
#include "wx_prop_dlg.hpp"
#include "io_params.hpp"
#include "msg_logger.hpp"

#include "win_vind.hpp"

namespace wxGUI
{
    static std::atomic_bool runnable{true} ;

    //core system is wrought at another thread
    class SystemThread : public wxThread {
    private:
        virtual ExitCode Entry() override {
            while(win_vind::update() && runnable.load()) ;
            return static_cast<ExitCode>(0) ;
        }

    public:
        SystemThread()
        : wxThread(wxTHREAD_DETACHED)
        {}
    } ;

    static std::string g_function_name{} ;

    void App::OnInitCmdLine(wxCmdLineParser& parser) {
        parser.AddOption(wxT("f"), wxT("func"), wxT("FunctionName"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL) ;
    }
    bool App::OnCmdLineParsed(wxCmdLineParser& parser) {
        wxString fn ;
        if(parser.Found(wxT("func"), &fn)) {
            g_function_name = fn.ToStdString() ;
        }
        return true ;
    }

    bool App::OnInit() {
        if(!wxApp::OnInit()) {
            return false ;
        }

        if(!wxTaskBarIcon::IsAvailable()) {
            wxMessageBox("not supported win_vind Tray", "Warning", wxOK | wxICON_EXCLAMATION) ;
        }

        if(!win_vind::initialize(g_function_name)) {
            ERROR_PRINT("failed initializing system") ;
            return false ;
        }
        if(!ioParams::load_config()) {
            ERROR_PRINT("failed loading config") ;
            return false ;
        }

        auto ppd = new PropDlg() ;
        ppd->Show(false) ;

        //enable opening window by command
        win_vind::register_show_window_func([ppd] {
            ppd->Show(true) ;
        }) ;

        win_vind::register_exit_window_func([ppd] {
            ppd->Show(true) ;
            ppd->Destroy() ;
        }) ;

        return true ;
    }

    int App::MainLoop() {
        //create a new thread for back-ground system
        auto pst = std::make_unique<SystemThread>() ;
        pst->Run() ;

        return wxApp::MainLoop() ;
    }

    App::~App() noexcept {
        if(runnable.load()) {
            //Core-win_vind is running
            runnable.store(false) ; //terminate core system
        }
    }
}
