#include "win_vind.hpp"

#include <windows.h>

#if defined(__GNUC__)
#include <errhandlingapi.h>
#include <memoryapi.h>
#include <winerror.h>
#include <winnt.h>
#endif

#include <memory>
#include <iostream>
#include <cstring>

#include "i_params.hpp"
#include "interval_timer.hpp"
#include "key_absorber.hpp"
#include "key_binder.hpp"
#include "keybrd_eventer.hpp"
#include "mode_manager.hpp"
#include "msg_logger.hpp"
#include "option_loader.hpp"
#include "path.hpp"
#include "vkc_converter.hpp"

#include "change_mode.hpp"
#include "edi_change_mode.hpp"
#include "mywindow_ctrl.hpp"

#define MEMORY_MAPPED_FILE_NAME ("qvCI980BTny1ZSFfY76sO71w7MtLTzuPVd6RQs47_p7Kn4SJZ7cnaH8QwPS901VFd2N5WuxECvx7N3hP7caWK44ZSq6")
#define MEMORY_MAPPED_FILE_SIZE (1024)

namespace win_vind
{
    using namespace std ;

    auto delete_handle = [] (void* handle) {
        CloseHandle(handle) ;
    } ;
    static std::unique_ptr<void, decltype(delete_handle)> g_map(NULL, delete_handle) ;

    auto unmap_view = [] (void* view) {
        UnmapViewOfFile(view) ;
    } ;
    inline static auto get_memmapped_file(HANDLE map) {
        return std::unique_ptr<void, decltype(unmap_view)>(
                MapViewOfFile(map, FILE_MAP_WRITE, 0, 0, 0), unmap_view) ;
    }

    bool initialize(const std::string func_name) noexcept {
        try {
            auto created_map = CreateFileMappingA(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, MEMORY_MAPPED_FILE_SIZE, MEMORY_MAPPED_FILE_NAME) ;
            if(GetLastError() == ERROR_ALREADY_EXISTS) {
                if(!func_name.empty()) {
                    auto hmap = OpenFileMappingA(FILE_MAP_WRITE, FALSE, MEMORY_MAPPED_FILE_NAME) ;
                    if(hmap == NULL) {
                        return false ;
                    }

                    auto data = get_memmapped_file(hmap) ;
                    if(data != NULL) {
                        std::memmove(data.get(), func_name.c_str(), func_name.length()) ;
                    }
                }
                return false ;
            }

            Logger::initialize() ;

            if(created_map == NULL) {
                ERROR_PRINT("Could not create memory-mapped file.") ;
                return false ;
            }

            g_map.reset(created_map) ;

            auto data = get_memmapped_file(g_map.get()) ;
            if(data.get() == NULL) {
                ERROR_PRINT("Could not open memory-mapped file.") ;
                return false ;
            }
            std::memset(data.get(), 0, MEMORY_MAPPED_FILE_SIZE) ;

            //show mouse cursor
            //When Windows was started up, cursor is hidden until move mouse by default.
            //Thus, send lowlevel move event in order to show cursor.
            INPUT in ;
            in.type           = INPUT_MOUSE ;
            in.mi.dx          = 1 ;
            in.mi.dy          = 1 ;
            in.mi.mouseData   = 0 ;
            in.mi.dwFlags     = MOUSEEVENTF_MOVE ;
            in.mi.time        = 0 ;
            in.mi.dwExtraInfo = GetMessageExtraInfo() ;

            if(!SendInput(1, &in, sizeof(INPUT))) {
                ERROR_PRINT("SendInput, MOUSEEVENTF_MOVE") ;
                return false ;
            }

            //load keyboard mapping of ascii code
            //For example, we type LShift + 1 or RShift + 1 in order to input '!' at JP-Keyboard.
            VKCConverter::load_input_combination() ;

            //lower keyboard hook
            KeyAbsorber::install_hook() ;

            KeyBinder::init() ;

            load_config() ;

            //initialize system mode
            const std::unordered_map<std::string, BindedFunc::shp_t> cm {
                {"gui_normal", Change2Normal::create()},
                {"gui_insert", Change2Insert::create()},
                {"edi_normal", Change2EdiNormal::create()},
                {"edi_insert", Change2EdiInsert::create()}
            } ;
            cm.at(iParams::get_s("initial_mode"))->process(true, 1) ;

            if(!func_name.empty()) {
                auto func = KeyBinder::find_func_byname(func_name) ;
                func->process(true, 1, nullptr, nullptr) ;
            }

            return true ;
        }
        catch(const std::exception& e) {
            ERROR_PRINT(std::string(e.what()) + ", so system was terminated.") ;
            return false ;
        }
        catch(...) {
            ERROR_PRINT("Fatal error occured.") ;
            return false ;
        }
    }

    bool load_config() noexcept {
        try {
            iParams::load_config() ;
            KeyBinder::load_config() ;
            OptionLoader::load_config() ;
            return true ;
        }
        catch(const std::exception& e) {
            ERROR_PRINT(e.what()) ;
            return false ;
        }
        catch(...) {
            ERROR_PRINT("Fatal error occured.") ;
            return false ;
        }
    }

    bool load_option_config() noexcept {
        try {
            OptionLoader::load_config() ;
            return true ;
        }
        catch(const std::exception& e) {
            ERROR_PRINT(e.what()) ;
            return false ;
        }
        catch(...) {
            ERROR_PRINT("Fatal error occured.") ;
            return false ;
        }
    }

    bool update() noexcept {
        try {
            if(!update_background()) {
                return false ;
            }

            static IntervalTimer timer{1000'000} ; //100 ms

            if(timer.is_passed()) {
                //check if received messages from another win-vind.
                auto mmf = get_memmapped_file(g_map.get()) ;
                if(mmf.get() != NULL) {
                    std::string name(reinterpret_cast<const char*>(mmf.get())) ;
                    if(!name.empty()) {
                        auto func = KeyBinder::find_func_byname(name) ;
                        if(func != nullptr) {
                            func->process(true, 1, nullptr, nullptr) ;
                        }
                        else {
                            ERROR_PRINT(name + " is invalid function name.") ;
                        }

                        std::memset(mmf.get(), 0, MEMORY_MAPPED_FILE_SIZE) ;
                    }
                    return true ;
                }
            }

            KeyBinder::call_matched_funcs() ;
            OptionLoader::call_active_funcs() ;

            return true ;
        }
        catch(const std::exception& e) {
            ERROR_PRINT(e.what()) ;
            return false ;
        }
        catch(...) {
            ERROR_PRINT("Fatal error occured.") ;
            return false ;
        }
    }

    bool update_background() noexcept {
        try {
            Sleep(5) ;

            OptionLoader::call_active_funcs() ;

            Utility::get_win_message() ;

            using namespace KeyAbsorber ;
            if(is_pressed(VKC_F8) && is_pressed(VKC_F9)) {
                ExitConfigWindow::sprocess(true, 1, nullptr, nullptr) ; //exit GUI-window in system tray
                return false ;
            }

            return true ;
        }
        catch(const std::exception& e) {
            ERROR_PRINT(e.what()) ;
            return false ;
        }
        catch(...) {
            ERROR_PRINT("Fatal error occured.") ;
            return false ;
        }
    }

    //Please use it if you want to show a self config window by command.
    void register_show_window_func(std::function<void()> func) noexcept {
        try {
            ShowConfigWindow::register_show_func(std::move(func)) ;
        }
        catch(const std::exception& e) {
            ERROR_PRINT(e.what()) ;
        }
        catch(...) {
            ERROR_PRINT("Fatal error occured.") ;
        }
    }

    void register_exit_window_func(std::function<void()> func) noexcept {
        try {
            ExitConfigWindow::register_exit_func(std::move(func)) ;
        }
        catch(const std::exception& e) {
            ERROR_PRINT(e.what()) ;
        }
        catch(...) {
            ERROR_PRINT("Fatal error occured.") ;
        }
    }
}
