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
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "entry.hpp"
#include "keycode_def.hpp"
#include <chrono>

#define _WIN32_WINNT_WIN10 0x0A00 //Windows 10

//for DPI support
#if !defined(WINVER)

#define WINVER        _WIN32_WINNT_WIN10
#define _WIN32_WINNT  _WIN32_WINNT_WIN10

#elif WINVER < _WIN32_WINNT_WIN10

#define WINVER        _WIN32_WINNT_WIN10
#define _WIN32_WINNT  _WIN32_WINNT_WIN10

#endif

#include <windows.h>

#if defined(__GNUC__)
#include <errhandlingapi.h>
#include <memoryapi.h>
#include <winerror.h>
#include <winnt.h>
#endif

#include <cstring>
#include <memory>

#if defined(DEBUG)
#include "util/debug.hpp"
#include <iostream>
#endif

#include "bind/ctrl/mywindow_ctrl.hpp"
#include "bind/emu/edi_change_mode.hpp"
#include "bind/mode/change_mode.hpp"
#include "bind/syscmd/source.hpp"
#include "opt/option_loader.hpp"

#include "err_logger.hpp"
#include "func_finder.hpp"
#include "g_maps.hpp"
#include "g_params.hpp"
#include "key_absorber.hpp"
#include "keycodecvt.hpp"
#include "log_map.hpp"
#include "main_loop.hpp"
#include "mode.hpp"
#include "path.hpp"

#include "util/debug.hpp"
#include "util/interval_timer.hpp"
#include "util/winwrap.hpp"

#define MEMORY_MAPPED_FILE_NAME ("qvCI980BTny1ZSFfY76sO71w7MtLTzuPVd6RQs47_p7Kn4SJZ7cnaH8QwPS901VFd2N5WuxECvx7N3hP7caWK44ZSq6")
#define MEMORY_MAPPED_FILE_SIZE (1024)

//internal linkage
namespace
{
    auto delete_handle = [] (void* handle) {
        CloseHandle(handle) ;
    } ;

    std::unique_ptr<void, decltype(delete_handle)> g_map(NULL, delete_handle) ;

    auto unmap_view = [] (void* view) {
        UnmapViewOfFile(view) ;
    } ;
    inline auto get_memmapped_file(HANDLE map) {
        return std::unique_ptr<void, decltype(unmap_view)>(
                MapViewOfFile(map, FILE_MAP_WRITE, 0, 0, 0), unmap_view) ;
    }


    inline bool show_mouse_cursor() {
        //Thus, send lowlevel move event in order to show cursor.
        INPUT in ;
        in.type           = INPUT_MOUSE ;
        in.mi.dx          = 1 ;
        in.mi.dy          = 1 ;
        in.mi.dwFlags     = MOUSEEVENTF_MOVE ;
        in.mi.dwExtraInfo = GetMessageExtraInfo() ;

        if(!SendInput(1, &in, sizeof(INPUT))) {
            PRINT_ERROR("SendInput, MOUSEEVENTF_MOVE") ;
            return false ;
        }
        return true ;
    }
}

namespace vind
{
    namespace core
    {
        bool initialize(const std::string& func_name) noexcept {
            try {
                if(!std::filesystem::exists(CONFIG_PATH())) {
                    std::filesystem::create_directories(CONFIG_PATH()) ;
                }

                initialize_logger() ;

                auto created_map = CreateFileMappingA(
                        INVALID_HANDLE_VALUE, NULL,
                        PAGE_READWRITE, 0,
                        MEMORY_MAPPED_FILE_SIZE, MEMORY_MAPPED_FILE_NAME) ;

                if(created_map == NULL) {
                    PRINT_ERROR("Could not create memory-mapped file.") ;
                    return false ;
                }
                g_map.reset(created_map) ;

                if(GetLastError() == ERROR_ALREADY_EXISTS) {
                    // Sub win-vind
                    if(!func_name.empty()) {
                        auto hmap = OpenFileMappingA(FILE_MAP_WRITE, FALSE, MEMORY_MAPPED_FILE_NAME) ;
                        if(hmap == NULL) {
                            return false ;
                        }

                        std::unique_ptr<void, decltype(delete_handle)> safe_hmap(hmap, delete_handle) ;

                        auto data = get_memmapped_file(safe_hmap.get()) ;
                        if(data != NULL) {
                            std::memmove(data.get(), func_name.c_str(), func_name.length()) ;
                        }
                    }
                    return false ;
                }

                // Main win-vind
                auto data = get_memmapped_file(g_map.get()) ;
                if(data.get() == NULL) {
                    PRINT_ERROR("Could not open memory-mapped file.") ;
                    return false ;
                }
                std::memset(data.get(), 0, MEMORY_MAPPED_FILE_SIZE) ;
                if(!func_name.empty()) {
                    std::memmove(data.get(), func_name.c_str(), func_name.length()) ;
                }

                //enable high DPI support
                if(!SetProcessDPIAware()) {
                    PRINT_ERROR("Your system is not supported DPI.") ;
                    return false ;
                }

                // Load default config
                initialize_params() ;
                initialize_maps() ;

                //load keyboard mapping of ascii code
                //For example, we type LShift + 1 or RShift + 1 in order to input '!' at JP-Keyboard.
                load_input_combination() ;

                initialize_mainloop() ;

                if(!load_config()) {
                    return false ;
                }

                //lower keyboard hook
                //If you use debugger, must be disable this line not to be slow.
                install_absorber_hook() ;

                // When Windows was started up, cursor is hidden until move mouse by default.
                if(!show_mouse_cursor()) {
                    return false ;
                }

                std::unordered_map<std::string, bind::BindedFunc::SPtr> cm {
                    {mode_to_prefix(Mode::EDI_NORMAL), bind::ToEdiNormal::create()},
                    {mode_to_prefix(Mode::GUI_NORMAL), bind::ToGUINormal::create()},
                    {mode_to_prefix(Mode::INSERT), bind::ToInsert::create()},
                    {mode_to_prefix(Mode::RESIDENT), bind::ToResident::create()}
                } ;
                cm.at(get_s("initmode"))->process() ;

                return true ;
            }
            catch(const std::exception& e) {
                PRINT_ERROR(std::string(e.what()) + ", so system was terminated.") ;
                return false ;
            }
            catch(...) {
                PRINT_ERROR("Fatal error occured.") ;
                return false ;
            }
        }


        bool load_config() noexcept {
            try {
                bind::SyscmdSource::sprocess(RC(), false) ;
                reconstruct_all_components() ;
                return true ;
            }
            catch(const std::exception& e) {
                PRINT_ERROR(e.what()) ;
                return false ;
            }
            catch(...) {
                PRINT_ERROR("Fatal error occured.") ;
                return false ;
            }
        }


        bool reconstruct_all_components() noexcept {
            try {
                load_keycodemap_config() ;
                opt::reconstruct() ;
                reconstruct_mainloop() ;
                return true ;
            }
            catch(const std::exception& e) {
                PRINT_ERROR(e.what()) ;
                return false ;
            }
            catch(...) {
                PRINT_ERROR("Fatal error occured.") ;
                return false ;
            }
        }

        bool update() noexcept {
            try {
                if(!update_background()) {
                    return false ;
                }

                static util::IntervalTimer timer{1000'000} ; //1 s

                if(timer.is_passed()) {
                    //check if received messages from another win-vind.
                    auto mmf = get_memmapped_file(g_map.get()) ;
                    if(mmf.get() != NULL) {
                        std::string name(reinterpret_cast<const char*>(mmf.get())) ;
                        if(!name.empty()) {
                            if(auto func = FuncFinder::find_func_byname(name)) {
                                func->process() ;
                            }
                            else {
                                PRINT_ERROR(name + " is invalid function name.") ;
                            }

                            std::memset(mmf.get(), 0, MEMORY_MAPPED_FILE_SIZE) ;
                        }
                        return true ;
                    }
                }

                update_mainloop() ;

                return true ;
            }
            catch(const std::exception& e) {
                PRINT_ERROR(e.what()) ;
                return false ;
            }
            catch(...) {
                PRINT_ERROR("Fatal error occured.") ;
                return false ;
            }
        }

        bool update_background() noexcept {
            try {
                util::get_win_message() ;

                Sleep(5) ;

                opt::call_active_funcs() ;

                refresh_toggle_state() ;

                if(is_really_pressed(KEYCODE_F8, KEYCODE_F9)) {
                    bind::ExitConfigGUI::sprocess() ; //exit GUI-window in system tray
                    return false ;
                }
                return true ;
            }
            catch(const std::exception& e) {
                PRINT_ERROR(e.what()) ;
                return false ;
            }
            catch(...) {
                PRINT_ERROR("Fatal error occured.") ;
                return false ;
            }
        }

        //Please use it if you want to show a self config window by command.
        void register_show_window_func(std::function<void()> func) noexcept {
            try {
                bind::ShowConfigGUI::register_show_func(std::move(func)) ;
            }
            catch(const std::exception& e) {
                PRINT_ERROR(e.what()) ;
            }
            catch(...) {
                PRINT_ERROR("Fatal error occured.") ;
            }
        }

        void register_exit_window_func(std::function<void()> func) noexcept {
            try {
                bind::ExitConfigGUI::register_exit_func(std::move(func)) ;
            }
            catch(const std::exception& e) {
                PRINT_ERROR(e.what()) ;
            }
            catch(...) {
                PRINT_ERROR("Fatal error occured.") ;
            }
        }
    }
}
