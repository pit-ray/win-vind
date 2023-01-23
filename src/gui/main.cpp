/*
             _                  _           __
   _      __(_)___       _   __(_)___  ____/ /
  | | /| / / / __ \_____| | / / / __ \/ __  /
  | |/ |/ / / / / /_____/ |/ / / / / / /_/ /
  |__/|__/_/_/ /_/      |___/_/_/ /_/\__,_/

       - Vim Key Binder for Windows -

MIT License

Copyright (c) 2020-2022 pit-ray
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
#include <string>

#include "util/disable_compiler_warning.hpp"

#include <wx/app.h>
#include <wx/cmdline.h>
#include <wx/msgdlg.h>

#include "util/enable_compiler_warning.hpp"

#include "core/entry.hpp"
#include "core/errlogger.hpp"
#include "core/path.hpp"
#include "core/settable.hpp"

#include "util/debug.hpp"

#include "about.hpp"


namespace
{
    inline void error_box(const wxString& msg) {
        wxMessageBox(msg, wxT("Error - win-vind"), wxOK | wxICON_EXCLAMATION) ;
    }

    void trim_quotation(std::string& str) {
        if(str.front() == '\"' && str.back() == '\"') {
            str = str.substr(1, str.size() - 2) ;
        }
    }
}

namespace vind
{
    namespace gui
    {
        //core system is wrought at another thread
        class SystemThread : public wxThread {
        public:
            core::VindEntry entry_ ;

            std::atomic<bool> runnable_ ;

            template <typename ExitFunc>
            SystemThread(ExitFunc&& exit_func)
            : wxThread(wxTHREAD_DETACHED),
              entry_(std::forward<ExitFunc>(exit_func)),
              runnable_(true)
            {}

        private:
            virtual ExitCode Entry() override {
                runnable_.store(true) ;
                while(runnable_.load()) {
                    try {
                        entry_.update() ;
                    }
                    catch(const std::exception& e) {
                        core::Logger::get_instance().error(e.what()) ;
                        break ;
                    }
                    catch(const core::SafeForcedTermination& e) {
                        core::Logger::get_instance().message(e.what()) ;
                        break ;
                    }
                    catch(...) {
                        core::Logger::get_instance().error("Fatal error occured.") ;
                        break ;
                    }
                }

                if(runnable_.load()) {
                    runnable_.store(false) ;
                    entry_.handle_system_call(SystemCall::TERMINATE) ;
                }

                return static_cast<ExitCode>(0) ;
            }

        public:
            void exit() noexcept {
                runnable_.store(false) ;
            }

            bool is_running() const noexcept {
                return runnable_.load() ;
            }

            virtual ~SystemThread() noexcept = default ;

            SystemThread(SystemThread&&)                 = delete ;
            SystemThread& operator=(SystemThread&&)      = delete ;
            SystemThread(const SystemThread&)            = delete ;
            SystemThread& operator=(const SystemThread&) = delete ;
        } ;


        class App : public wxApp {
        private:
            std::unique_ptr<SystemThread> pst_ ;

            std::string arg_func_name_ ;
            std::string arg_command_ ;

        public:
            App()
            : pst_(nullptr),
              arg_func_name_(),
              arg_command_()
            {}

            virtual ~App() noexcept {
                // Core win_vind is running yet, so terminate core system.
                pst_->exit() ;
            }

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

                    pst_ = std::make_unique<SystemThread>([this] {
                        ExitMainLoop() ;
                    }) ;

                    if(!arg_func_name_.empty()) {
                        pst_->entry_.send_function_request(arg_func_name_) ;
                    }
                    if(!arg_command_.empty()) {
                        pst_->entry_.send_command_request(arg_command_) ;
                    }

                    if(pst_->entry_.is_subprocess()) {
                        return false ;
                    }

                    pst_->entry_.init() ;

                    auto& settable = core::SetTable::get_instance() ;

                    const auto& icon_style = settable.get("icon_style") ;

                    // Root window
                    auto dlg = new AboutDialog(
                            (vind::core::RESOURCE_ROOT_PATH() / icon_style.get<std::string>()).u8string(),
                            "win-vind",
                            settable.get("gui_fontsize").get<int>(),
                            settable.get("gui_fontname").get<std::string>()) ;

                    dlg->Show(false) ;
                }
                catch(const std::exception& e) {
                    error_box(wxString::FromUTF8(e.what()) \
                            + wxT(" Could not initialize win-vind, so terminate." \
                            + wxT(" (Windows Error Code: ") + std::to_string(GetLastError()) + ")")) ;
                    return false ;
                }
                return true ;
            }

            int MainLoop() override {
                pst_->Run() ;
                return wxApp::MainLoop() ;
            }

            int OnRun() {
                wxApp::OnRun() ;
                return 3 ;
            }

            void OnInitCmdLine(wxCmdLineParser& parser) override {
                parser.AddSwitch(
                        wxT("h"),
                        wxT("help"),
                        wxT("Print usage and exit."),
                        wxCMD_LINE_PARAM_OPTIONAL) ;

                parser.AddOption(
                        wxT("f"),
                        wxT("func"),
                        wxT("Identifier of the function to call in one-shot."),
                        wxCMD_LINE_VAL_STRING,
                        wxCMD_LINE_PARAM_OPTIONAL) ;

                parser.AddOption(
                        wxT("c"),
                        wxT("command"),
                        wxT("Keystrokes passed to win-vind."),
                        wxCMD_LINE_VAL_STRING,
                        wxCMD_LINE_PARAM_OPTIONAL) ;
            }

            bool OnCmdLineParsed(wxCmdLineParser& parser) override {
                wxString fn ;
                if(parser.Found(wxT("help"))) {
                    parser.Usage() ;
                    return false ;
                }
                if(parser.Found(wxT("func"), &fn)) {
                    arg_func_name_ = fn.ToStdString() ;
                    trim_quotation(arg_func_name_) ;
                }
                if(parser.Found(wxT("command"), &fn)) {
                    arg_command_ = fn.ToStdString() ;
                    trim_quotation(arg_command_) ;
                }
                return true ;
            }

            bool OnExceptionInMainLoop() override {
                try {
                    throw ; //Rethrow the current exception.
                }
                catch(const std::exception& e) {
                    pst_->exit() ;
                    PRINT_ERROR(e.what()) ;
                }

                return false ; //exit program
            }

            void OnUnhandledException() override {
                try {
                    throw ; //Rethrow the current exception.
                }
                catch(const std::exception& e) {
                    pst_->exit() ;
                    PRINT_ERROR(e.what()) ;
                }

                //the program is already about to exit.
            }
        } ;
    }
}


#include "util/disable_compiler_warning.hpp"

DECLARE_APP(vind::gui::App) ;

#ifdef DEBUG
IMPLEMENT_APP_CONSOLE(vind::gui::App) ;
#else
IMPLEMENT_APP(vind::gui::App) ;
#endif

#include "util/enable_compiler_warning.hpp"
