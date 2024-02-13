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

#define NOMINMAX

#include "core/entry.hpp"
#include "core/errlogger.hpp"
#include "core/path.hpp"
#include "core/settable.hpp"
#include "core/version.hpp"

#include "util/debug.hpp"
#include "util/def.hpp"
#include "util/string.hpp"
#include "util/winwrap.hpp"

#include "startup.hpp"

#include <atomic>
#include <exception>
#include <future>
#include <string>

#include "fluent_tray/fluent_tray.hpp"

#include "argparse/argparse.hpp"

#include <fcntl.h>
#include <io.h>


namespace
{
    inline void trim_quotation(std::string& str) {
        if(str.front() == '\"' && str.back() == '\"') {
            str = str.substr(1, str.size() - 2) ;
        }
    }

    inline bool create_console() {
        if(!AllocConsole()) {
            return false ;
        }

        _setmode(_fileno(stdout), _O_U8TEXT) ;

        FILE* fp ;
        if(freopen_s(&fp, "CONOUT$", "w+", stdout) != 0) {
            return false ;
        }
        return true ;
    }
}

#ifdef DEBUG

////////////////////////////////////
///            Debug             ///
////////////////////////////////////

// Console is available.
int main(int argc, char* argv[])
{
    using namespace vind ;
    std::vector<std::string> args ;
    for(int i = 0 ; i < argc ; i ++ ) {
        args.push_back(std::string(argv[i])) ;
    }

#else

////////////////////////////////////
///           Release           ////
////////////////////////////////////

int WINAPI WinMain(
    HINSTANCE UNUSED(hinstance),
    HINSTANCE UNUSED(h_prev_instance),
    LPSTR UNUSED(lp_cmd_line),
    int UNUSED(n_show_cmd))
{
    using namespace vind ;
    int argc ;
    auto argv = CommandLineToArgvW(GetCommandLineW(), &argc) ;

    std::vector<std::string> args ;
    for(int i = 0 ; i < argc ; i ++ ) {
        args.push_back(util::ws_to_s(argv[i])) ;
    }

#endif
    using namespace std::chrono ;

    std::atomic<bool> runnable = true ;
    std::future<void> ft ;

    try {
        argparse::ArgumentParser parser(
            "win-vind", WIN_VIND_VERSION, argparse::default_arguments::none) ;

        parser.add_argument("-h", "--help")
            .action([&parser] (const auto&) {
                if(create_console()) {
                    std::cout << parser ;
                    system("PAUSE") ;
                }
            })
            .default_value(false)
            .help("Shows help message and exits.")
            .implicit_value(true)
            .nargs(0) ;
        parser.add_argument("-v", "--version")
            .action([&parser] (const auto&) {
                if(create_console()) {
                    std::cout << WIN_VIND_VERSION << std::endl ;
                    system("PAUSE") ;
                }
            })
            .default_value(false)
            .help("Prints version information and exits.")
            .implicit_value(true)
            .nargs(0) ;
        parser.add_argument("-f", "--func").help(
            "Identifier of the function to call in one-shot.") ;
        parser.add_argument("-c", "--command").help(
            "Keystrokes passed to win-vind.") ;

        parser.parse_args(args) ;

        auto exit_process = [&runnable] {
            runnable = false ;
            return false ;
        } ;

        core::VindEntry entry{exit_process} ;

        if(parser.is_used("--command")) {
            auto arg_cmd = parser.get<std::string>("--command") ;
            trim_quotation(arg_cmd) ;
            entry.send_command_request(arg_cmd) ;
        }
        else if(parser.is_used("--func")) {
            auto arg_fn = parser.get<std::string>("--func") ;
            trim_quotation(arg_fn) ;
            entry.send_function_request(arg_fn) ;
        }

        if(entry.is_subprocess()) {
            return 1 ;
        }

        entry.init() ;

        fluent_tray::FluentTray tray{} ;

        auto& settable = core::SetTable::get_instance() ;
        const auto& icon_path = settable.get("icon_style").get<std::string>() ;

        if(!tray.create_tray(
            "win-vind",
            (core::RESOURCE_ROOT_PATH() / "icons" / icon_path).u8string(),
            5, 5, 10, 5, 240, true)) {
            PRINT_ERROR("Failed tray initialization") ;
            return 1 ;
        }

        // --------------------------
        auto check_startup = [] {
            gui::register_to_startup() ;
            return true ;
        } ;
        auto uncheck_startup = [] {
            gui::remove_from_startup() ;
            return true ;
        } ;
        if(!tray.add_menu(
                "Startup with Windows",
                (core::RESOURCE_ROOT_PATH() / "icons" / "fa-tachometer.ico").u8string(),
                true, "・",
                check_startup, uncheck_startup)) {
            PRINT_ERROR("Could not add a menu.") ;
            return 1 ;
        }
        if(gui::check_if_registered()) {
            tray.back().check() ;
        }
        else {
            tray.back().uncheck() ;
        }

        // --------------------------
        tray.add_separator() ;

        auto open_root_dir = [] {
            util::create_process(
                    core::ROOT_PATH(),
                    "explorer",
                    core::CONFIG_PATH().u8string()) ;
            return true ;
        } ;
        if(!tray.add_menu(
                "Open Root Directory",
                (core::RESOURCE_ROOT_PATH() / "icons" / "fa-folder.ico").u8string(),
                false, "", open_root_dir)) {
            PRINT_ERROR("Could not add a menu.") ;
            return 1 ;
        }

        auto show_version = [&tray] {
            constexpr auto title = "About win-vind" ;
            constexpr auto message = \
                "Version: " WIN_VIND_VERSION
                "\n"
                "\n"
                "MIT License (C) 2020-2024 pit-ray" ;

            tray.show_balloon_tip(title, message) ;
            return true ;
        } ;
        if(!tray.add_menu(
                "About",
                (core::RESOURCE_ROOT_PATH() / "icons" / "fa-info-circle.ico").u8string(),
                false, "", show_version)) {
            PRINT_ERROR("Could not add a menu.") ;
            return 1 ;
        }

        // --------------------------
        tray.add_separator() ;

        if(!tray.add_menu(
                "Exit",
                (core::RESOURCE_ROOT_PATH() / "icons" / "fa-sign-out.ico").u8string(),
                false, "", exit_process)) {
            PRINT_ERROR("Could not add a menu.") ;
            return 1 ;
        }
        // --------------------------

        auto vind_process = [&entry, &runnable] {
            while(runnable.load()) {
                try {
                    entry.update() ;
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

            if(runnable.load()) {
                runnable.store(false) ;
                entry.handle_system_call(SystemCall::TERMINATE) ;
            }
        } ;
        ft = std::async(std::launch::async, vind_process) ;

        while(runnable.load()) {
            if(tray.status() == fluent_tray::TrayStatus::SHOULD_STOP) {
                break ;
            }
            if(!tray.update()) {
                PRINT_ERROR("Failed the tray update") ;
                break ;
            }
            Sleep(1) ;
        }

        runnable.store(false) ;
        while(ft.wait_for(50ms) == std::future_status::timeout) {}

        if(tray.status() == fluent_tray::TrayStatus::FAILED) {
            return 1 ;
        }
    }
    catch(const std::exception& e) {
        PRINT_ERROR(e.what()) ;
        runnable.store(false) ;
        while(ft.wait_for(50ms) == std::future_status::timeout) {}
        return 1 ;
    }

    return 0 ;
}
