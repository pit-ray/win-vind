#include "wxgui.hpp"

#include "wx_config_window.hpp"
#include "wx_prop_dlg.hpp"
#include "system.hpp"

#include <wx/taskbar.h>
#include <wx/msgdlg.h>
#include <wx/thread.h>

#include <windows.h>
#include <iostream>
#include <atomic>

namespace wxGUI
{
    static std::atomic_bool runnable{true} ;

    class SystemThread : public wxThread {
    private:
        virtual ExitCode Entry() override {
            MSG msg ;
            while(System::is_update() && runnable.load()) {
                //MessageRoop
                if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
                    TranslateMessage(&msg) ;
                    DispatchMessage(&msg) ;
                }

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

        if(!System::is_init()) {
            return false ;
        }

        /*
        auto pcw = new ConfigWindow ;
        pcw->Show(false) ;
        */
        auto ppd = new PropDlg() ;
        ppd->Show(true) ;

        return true ;
    }

    int App::MainLoop() {
        //create a new thread for back-ground system
        auto pst = std::make_unique<SystemThread>() ;
        pst->Run() ;

        return wxApp::MainLoop() ;
    }

    App::~App() {
        if(runnable.load()) {
            //System is running
            runnable.store(false) ; //terminate system
        }
    }
}