#include "system.hpp"

#include "key_binder.hpp"
#include "vkc_converter.hpp"
#include "key_absorber.hpp"
#include "path.hpp"
#include "option_loader.hpp"
#include "keybrd_eventer.hpp"

#include <iostream>

//for debug
#include <utility>

namespace System
{
    using namespace std ;

    static KeyBinder _kb ;
    static OptionLoader _ol(&_kb) ;

    bool is_init() {
        //load keyboard mapping of ascii code
        //For example, we type LShift + 1 or RShift + 1 in order to input '!' at JP-Keyboard.
        VKCConverter::load_input_combination() ;

        //lower keyboard hook
        if(!KeyAbsorber::is_install_hook()) {
            return false ;
        }

        load_config() ;

        return true ;
    }

    void load_config() {
        _kb.load_config(Path::CONFIG_XML()) ;
        _ol.load_config(Path::CONFIG_OPTION_INI()) ;
    }

    void load_option_config() {
        _ol.load_config(Path::CONFIG_OPTION_INI()) ;
    }

    bool is_update() {
        //KeybrdEventer::update_sync_push() ;

        _kb.update() ;
        _ol.update() ;

        using namespace KeyAbsorber ;
        if(is_down(VKC_F8) && is_down(VKC_F9)) {
            Logger::msg_stream << "[Message] Completed successfully (ExitCommand)\n" ;
            return false ;
        }

        return true ;
    }
}