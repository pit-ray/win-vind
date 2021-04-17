#include "wxgui.hpp"

#include <windows.h>

#include <atomic>
#include <exception>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

#include "disable_gcc_warning.hpp"

#include <wx/cmdline.h>
#include <wx/defs.h>
#include <wx/msgdlg.h>
#include <wx/string.h>
#include <wx/taskbar.h>
#include <wx/thread.h>

#include <nlohmann/json.hpp>


#include "enable_gcc_warning.hpp"

#include "coreio/err_logger.hpp"
#include "coreio/path.hpp"
#include "entry.hpp"

#include "util/winwrap.hpp"

#include "io_params.hpp"
#include "wx_prop_dlg.hpp"

namespace wxGUI
{
    using namespace vind ;

    inline static void error_box(const wxString& msg) {
        wxMessageBox(msg, wxT("Error - win-vind"), wxOK | wxICON_EXCLAMATION) ;
    }

    inline static bool is_pre_initialized() {
        std::ifstream ifs(path::to_u8path(path::ROOT_PATH() + "is_initialized")) ;
        if(!ifs.is_open()) {
            return false ;
        }
        std::string str{} ;
        std::getline(ifs, str) ;
        return str.front() == 'y' || str.front() == 'Y' ;
    }

    inline static void finish_pre_initialization() {
        std::ofstream ofs(path::to_u8path(path::ROOT_PATH() + "is_initialized"), std::ios::trunc) ;
        ofs << "y" ;
    }

    inline static bool initialize_config_files() ;

    static std::atomic_bool runnable{true} ;

    //core system is wrought at another thread
    class SystemThread : public wxThread {
    private:
        virtual ExitCode Entry() override {
            while(vind::update() && runnable.load()) ;
            return static_cast<ExitCode>(0) ;
        }

    public:
        SystemThread()
        : wxThread(wxTHREAD_DETACHED)
        {}
    } ;

    static std::string g_function_name{} ;

    void App::OnInitCmdLine(wxCmdLineParser& parser) {
        parser.AddOption(wxT("f"), wxT("func"),
                wxT("FunctionName"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL) ;
    }
    bool App::OnCmdLineParsed(wxCmdLineParser& parser) {
        wxString fn ;
        if(parser.Found(wxT("func"), &fn)) {
            g_function_name = fn.ToStdString() ;
        }
        return true ;
    }

    bool App::OnInit() {
        try {
            if(!wxApp::OnInit()) {
                return false ;
            }

            if(!wxTaskBarIcon::IsAvailable()) {
                wxMessageBox(
                        wxT("Not supported System Tray"),
                        wxT("Warning - win-vind"), wxOK | wxICON_EXCLAMATION) ;
            }

            if(!is_pre_initialized()) {
                if(!initialize_config_files()) {
                    return false ;
                }
            }

            if(!vind::initialize(g_function_name)) {
                return false ;
            }
            if(!ioParams::initialize()) {
                PRINT_ERROR("Failed initialize ioParams.") ;
                return false ;
            }

            auto ppd = new PropDlg() ;
            ppd->Show(false) ;

            //enable opening window by command
            vind::register_show_window_func([ppd] {
                ppd->Show(true) ;
            }) ;

            vind::register_exit_window_func([ppd] {
                ppd->Show(true) ;
                ppd->Destroy() ;
            }) ;
        }
        catch(const std::exception& e) {
            error_box(wxString::FromUTF8(e.what()) \
                    + wxT(" Could not initialize win-vind, so terminate." \
                    + wxT(" (Windows Error Code: ") + std::to_string(GetLastError()) + ")")) ;
        }
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

    bool App::OnExceptionInMainLoop() {
        try {
            throw ; //Rethrow the current exception.
        }
        catch(const std::exception& e) {
            PRINT_ERROR(e.what()) ;
        }

        return false ; //exit program
    }

    void App::OnUnhandledException() {
        try {
            throw ; //Rethrow the current exception.
        }
        catch(const std::exception& e) {
            PRINT_ERROR(e.what()) ;
        }
        //the program is already about to exit.
    }

    inline static bool initialize_config_files() {
        auto overwrite_bindings = [] {
            util::copy_file(path::Default::BINDINGS(), path::BINDINGS(), true) ;
        } ;
        auto overwrite_settings = [] {
            util::copy_file(path::Default::SETTINGS(), path::SETTINGS(), true) ;
        } ;

        auto write_kmp = [] (BOOL allow_overwrite) {
            //The kmp file needs a better format and a dedicated management system.
            std::vector<std::string> def_kmp = {
                path::MODULE_ROOT_PATH() + "default_config\\JP.kmp",
                path::MODULE_ROOT_PATH() + "default_config\\US.kmp",
                path::MODULE_ROOT_PATH() + "default_config\\custom.kmp"
            } ;
            std::vector<std::string> kmp = {
                path::CONFIG_PATH() + "JP.kmp",
                path::CONFIG_PATH() + "US.kmp",
                path::CONFIG_PATH() + "custom.kmp"
            } ;

            for(std::size_t i = 0 ; i < def_kmp.size() ; i ++) {
                try {
                    util::copy_file(def_kmp[i], kmp[i], allow_overwrite) ;
                }
                catch(const std::runtime_error& e) {
                    if(!allow_overwrite) continue ;
                    else throw e ;
                }
            }
        } ;

        try {
            if(!util::is_existed_dir(path::CONFIG_PATH().c_str())) { //clean install
                util::create_directory(path::CONFIG_PATH()) ;
                overwrite_bindings() ;
                overwrite_settings() ;
                write_kmp(TRUE) ;
            }
            else { //a config directory is already existed.
                auto answer = wxMessageBox(
                        wxT("The old configuration files are detected. "\
                            "Do you want to keep the configuration? "\
                            "If you cancel, will terminate win-vind and ask again the next time."),
                        wxT("Confirm - win-vind"), wxYES_NO | wxCANCEL) ;

                if(answer == wxCANCEL) return false ;

                if(answer == wxYES) { //keep
                    auto keep_old_and_only_write_new = [] (
                            const auto& default_path,
                            const auto& new_path,
                            auto& old_ifs) {
                        using json = nlohmann::json ;

                        json dfj ;
                        std::ifstream def_ifs(path::to_u8path(default_path)) ;
                        def_ifs >> dfj ; //keep old

                        json olj ;
                        old_ifs >> olj ; 

                        if(dfj.is_object() && olj.is_object()) {
                            dfj.update(olj) ; //overwrite differences
                        }
                        else if(dfj.is_array() && olj.is_array()) {
                            for(auto& obj : dfj) {
                                for(auto& old : olj) {
                                    try {
                                        if(obj.at("name") == old.at("name")) {
                                            obj.update(old) ;
                                            break ;
                                        }
                                    }
                                    catch(const json::out_of_range&) {
                                        continue ;
                                    }
                                }
                            }
                        }
                        else {
                            throw std::runtime_error("The format of " + new_path + " is not supported.") ;
                        }

                        std::ofstream ofs(path::to_u8path(new_path)) ;
                        ofs << std::setw(4) << dfj << std::endl ;
                    } ;

                    //bindings.json
                    std::ifstream bindings_ifs(path::to_u8path(path::BINDINGS())) ;
                    if(!bindings_ifs.is_open()) {
                        overwrite_bindings() ; //does not exist
                    }
                    else {
                        keep_old_and_only_write_new(
                                path::Default::BINDINGS(),
                                path::BINDINGS(),
                                bindings_ifs) ;
                    }

                    //settings.json
                    std::ifstream settings_ifs(path::to_u8path(path::SETTINGS())) ;
                    if(!settings_ifs.is_open()) {
                        overwrite_settings() ; //does not exist
                    }
                    else {
                        keep_old_and_only_write_new(
                                path::Default::SETTINGS(),
                                path::SETTINGS(),
                                settings_ifs) ;
                    }

                    //JP.kmp, US.kmp. custom.kmp
                    write_kmp(false) ; //If a old file exist, not overwrite.
                }
                else { //overwrite
                    overwrite_bindings() ;
                    overwrite_settings() ;
                    write_kmp(true) ;
                }

            }
        }
        catch(const std::exception& e) {
            error_box(wxString::FromUTF8(e.what()) \
                    + wxT(" Could not create setting files of win-vind, so terminate." \
                    + wxT(" (Windows Error Code: ") + std::to_string(GetLastError()) + ")")) ;
            return false ;
        }

        finish_pre_initialization() ;
        return true ;
    }
}
