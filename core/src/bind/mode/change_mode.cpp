#include "bind/mode/change_mode.hpp"

#include <iostream>

#include <windows.h>

#include "bind/emu/simple_text_selecter.hpp"
#include "err_logger.hpp"
#include "io/keybrd.hpp"
#include "io/mouse.hpp"
#include "key/char_logger.hpp"
#include "key/key_absorber.hpp"
#include "key/keycode_def.hpp"
#include "key/ntype_logger.hpp"
#include "mode.hpp"
#include "opt/virtual_cmd_line.hpp"
#include "util/def.hpp"


namespace vind
{
    //Change2Normal
    Change2Normal::Change2Normal()
    : BindedFuncCreator("change_to_normal")
    {}
    void Change2Normal::sprocess(bool vclmodeout) {
        using namespace mode ;

        auto m = get_global_mode() ;
        if(m == Mode::Normal) return ;

        if(m == Mode::Visual) 
            mouse::click(KEYCODE_MOUSE_LEFT) ; //release holding mouse
        else if(is_edi_visual())
            textselect::unselect() ; //release shifting

        //When this function is called, binded key is down.
        //Thus, its key is needed to be up before absorbing key.
        keyabsorber::close_all_ports_with_refresh() ;
        keyabsorber::absorb() ;

        change_mode(Mode::Normal) ;
        if(vclmodeout)
            VirtualCmdLine::msgout("-- GUI NORMAL --") ;
    }
    void Change2Normal::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(true) ;
        }
    }
    void Change2Normal::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess(true) ;
    }


    //Change2Insert
    Change2Insert::Change2Insert()
    : BindedFuncCreator("change_to_insert")
    {}
    void Change2Insert::sprocess(bool vclmodeout) {
        using namespace mode ;
        if(mode::get_global_mode() == mode::Mode::Normal)
            mouse::click(KEYCODE_MOUSE_LEFT) ;

        keyabsorber::close_all_ports() ;
        keyabsorber::unabsorb() ;
        change_mode(Mode::Insert) ;
        if(vclmodeout)
            VirtualCmdLine::msgout("-- GUI INSERT --") ;
    }
    void Change2Insert::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(true) ;
        }
    }
    void Change2Insert::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess(true) ;
    }


    //Change2Visual
    Change2Visual::Change2Visual()
    : BindedFuncCreator("change_to_visual")
    {}

    void Change2Visual::sprocess(bool vclmodeout) {
        using namespace mode ;
        change_mode(Mode::Visual) ;
        if(vclmodeout) VirtualCmdLine::msgout("-- GUI VISUAL --") ;
        mouse::press(KEYCODE_MOUSE_LEFT) ;
    }
    void Change2Visual::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(true) ;
        }
    }
    void Change2Visual::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess(true) ;
    }
}
