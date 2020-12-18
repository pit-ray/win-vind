#include "system.hpp"

#include <windows.h>

#include <iostream>

#include "i_params.hpp"
#include "key_absorber.hpp"
#include "key_binder.hpp"
#include "keybrd_eventer.hpp"
#include "mode_manager.hpp"
#include "option_loader.hpp"
#include "path.hpp"
#include "uia_global.hpp"
#include "vkc_converter.hpp"

#include "change_mode.hpp"
#include "edi_change_mode.hpp"
#include "mywindow_ctrl.hpp"

namespace System
{
    using namespace std ;

    bool initialize() noexcept {
        try {
            //show mouse cursor
            //When Windows was started up, cursor is hidden until move mouse by default.
            //Thus, send lowlevel move event in order to show cursor.
            INPUT in ;
            in.type = INPUT_MOUSE ;
            in.mi.dx = 1 ;
            in.mi.dy = 1 ;
            in.mi.mouseData = 0 ;
            in.mi.dwFlags = MOUSEEVENTF_MOVE ;
            in.mi.time = 0 ;
            in.mi.dwExtraInfo = GetMessageExtraInfo() ;

            if(!SendInput(1, &in, sizeof(INPUT))) {
                ERROR_PRINT("SendInput, MOUSEEVENTF_MOVE") ;
                return false ;
            }

            //load keyboard mapping of ascii code
            //For example, we type LShift + 1 or RShift + 1 in order to input '!' at JP-Keyboard.
            VKCConverter::load_input_combination() ;

            UIA::initialize() ;

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
            Sleep(5) ;
            Utility::get_win_message() ;

            KeyBinder::call_matched_funcs() ;
            OptionLoader::call_active_funcs() ;

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

    bool update_options() noexcept {
        try {
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
