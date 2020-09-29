#include "system.hpp"

#include <windows.h>

#include <iostream>

#include "key_binder.hpp"
#include "vkc_converter.hpp"
#include "key_absorber.hpp"
#include "path.hpp"
#include "option_loader.hpp"
#include "keybrd_eventer.hpp"
#include "i_params.hpp"
#include "mode_manager.hpp"

#include "change_mode.hpp"
#include "edi_change_mode.hpp"

#include "common_bindings/mywindow_ctrl.hpp"

namespace System
{
    using namespace std ;

    bool init() {
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
            WIN_ERROR_PRINT("SendInput, MOUSEEVENTF_MOVE") ;
            return 0 ;
        }

        //load keyboard mapping of ascii code
        //For example, we type LShift + 1 or RShift + 1 in order to input '!' at JP-Keyboard.
        VKCConverter::load_input_combination() ;

        //lower keyboard hook
        if(!KeyAbsorber::install_hook()) {
            return false ;
        }

        KeyBinder::init() ;
        load_config() ;

        //initialize system mode
        const std::unordered_map<std::string, kbg::shp_t> cm {
            {"gui_normal", Change2Normal::create()},
            {"gui_insert", Change2Insert::create()},
            {"edi_normal", Change2Editor::create()},
            {"edi_insert", Change2EdiInsert::create()}
        } ;
        try {
            cm.at(iParams::get_s("initial_mode"))->process() ;
        }
        catch(const std::out_of_range& e) {
            ERROR_PRINT(std::string(e.what()) + ", in" + Path::SETTINGS() + ", initial_mode is invalid syntax.") ;
        }

        return true ;
    }

    void load_config() noexcept {
        iParams::load_config() ;
        KeyBinder::load_config() ;
        OptionLoader::load_config() ;
    }

    void load_option_config() noexcept {
        OptionLoader::load_config() ;
    }

    bool update() noexcept {
        KeyBinder::call_matched_funcs() ;
        OptionLoader::call_active_funcs() ;

        using namespace KeyAbsorber ;
        if(is_pressed(VKC_F8) && is_pressed(VKC_F9)) {
            return false ;
        }

        return true ;
    }

    bool update_options() noexcept {
        OptionLoader::call_active_funcs() ;
        return true ;
    }

    //Please use it if you want to show a self config window by command.
    void register_show_window_func(std::function<void()> func) noexcept {
        ShowConfigWindow::register_show_func(std::move(func)) ;
    }

    void register_exit_window_func(std::function<void()> func) noexcept {
        ExitConfigWindow::register_exit_func(std::move(func)) ;
    }
}
