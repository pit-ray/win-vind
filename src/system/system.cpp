#include "system.hpp"

#include "key_binder.hpp"
#include "vkc_converter.hpp"
#include "key_absorber.hpp"
#include "path.hpp"
#include "option_loader.hpp"
#include "keybrd_eventer.hpp"

#include <iostream>
#include <windows.h>

#include "gui_bindings/mywindow_ctrl.hpp"

namespace System
{
    using namespace std ;

    static KeyBinder _kb ;
    static OptionLoader _ol(&_kb) ;

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
            WIN_ERROR_STREAM << "(SendInput, MOUSEEVENTF_MOVE)\n" ;
            return 0 ;
        }

        //load keyboard mapping of ascii code
        //For example, we type LShift + 1 or RShift + 1 in order to input '!' at JP-Keyboard.
        VKCConverter::load_input_combination() ;

        //lower keyboard hook
        if(!KeyAbsorber::install_hook()) {
            return false ;
        }

        load_config() ;
        return true ;
    }

    void load_config() {
        _kb.load_config(Path::BINDINGS_LIST()) ;
        _ol.load_config(Path::CONFIG_OPTS_BOOL_INI()) ;
    }

    void load_option_config() {
        _ol.load_config(Path::CONFIG_OPTS_BOOL_INI()) ;
    }

    bool update() {
        _kb.update() ;
        _ol.update() ;

        using namespace KeyAbsorber ;
        if(is_downed(VKC_F8) && is_downed(VKC_F9)) {
            MESSAGE_STREAM << "Completed successfully (ExitCommand)\n" ;
            return false ;
        }

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