#include "wxgui.hpp"

#include <windows.h>
#include <iostream>
#include <atomic>

#include <wx/taskbar.h>
#include <wx/msgdlg.h>
#include <wx/thread.h>

#include "utility.hpp"
#include "wx_prop_dlg.hpp"
#include "io_params.hpp"
#include "system/system.hpp"
#include "msg_logger.hpp"

namespace wxGUI
{
    static std::atomic_bool runnable{true} ;

    //core system is wrought at another thread
    class SystemThread : public wxThread {
    private:
        virtual ExitCode Entry() override {
            while(System::update() && runnable.load()) {
                //MessageRoop
                Utility::get_win_message() ;
                Sleep(5) ; //5ms
            }
            return static_cast<ExitCode>(0) ;
        }

    public:
        SystemThread()
        : wxThread(wxTHREAD_DETACHED)
        {}
    } ;

    bool App::OnInit() {
        if(!wxApp::OnInit()) {
            return false ;
        }

        if(!wxTaskBarIcon::IsAvailable()) {
            wxMessageBox("not supported System Tray", "Warning", wxOK | wxICON_EXCLAMATION) ;
        }

        if(!System::init()) {
            ERROR_PRINT("failed initializing system") ;
            return false ;
        }
        if(!ioParams::load_config()) {
            ERROR_PRINT("failed loading config") ;
            return false ;
        }

        auto ppd = new PropDlg() ;
        //ppd->Show(true) ;
        ppd->Show(false) ;

        //enable opening window by command
        System::register_show_window_func([ppd] {
            ppd->Show(true) ;
        }) ;

        System::register_exit_window_func([ppd] {
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
            //Core-System is running
            runnable.store(false) ; //terminate core system
        }
    }
}
