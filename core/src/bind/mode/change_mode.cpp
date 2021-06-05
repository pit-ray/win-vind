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
        if(m == Mode::GUI_NORMAL) {
            return ;
        }

        if(m == Mode::GUI_VISUAL) {
            mouse::click(KEYCODE_MOUSE_LEFT) ; //release holding mouse
        }
        else if(m == Mode::EDI_VISUAL) {
            textselect::unselect() ; //release shifting
        }

        //When this function is called, binded key is down.
        //Thus, its key is needed to be up before absorbing key.
        keyabsorber::close_all_ports_with_refresh() ;
        keyabsorber::absorb() ;

        set_global_mode(Mode::GUI_NORMAL) ;
        if(vclmodeout) {
            VirtualCmdLine::reset() ;
            VirtualCmdLine::msgout("-- GUI NORMAL --") ;
        }
    }
    void Change2Normal::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(true) ;
        }
    }
    void Change2Normal::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess(true) ;
    }


    //Change2Resident
    Change2Resident::Change2Resident()
    : BindedFuncCreator("change_to_resident")
    {}
    void Change2Resident::sprocess(bool vclmodeout) {
        keyabsorber::close_all_ports() ;
        keyabsorber::unabsorb() ;
        mode::set_global_mode(mode::Mode::RESIDENT) ;
        if(vclmodeout) {
            VirtualCmdLine::reset() ;
            VirtualCmdLine::msgout("-- RESIDENT --") ;
        }
    }
    void Change2Resident::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(true) ;
        }
    }
    void Change2Resident::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess(true) ;
    }


    //Change2Visual
    Change2Visual::Change2Visual()
    : BindedFuncCreator("change_to_visual")
    {}

    void Change2Visual::sprocess(bool vclmodeout) {
        using namespace mode ;
        set_global_mode(Mode::GUI_VISUAL) ;
        if(vclmodeout) {
            VirtualCmdLine::reset() ;
            VirtualCmdLine::msgout("-- GUI VISUAL --") ;
        }
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

    //Change2EdiNormal
    Change2EdiNormal::Change2EdiNormal()
    : BindedFuncCreator("change_to_edi_normal")
    {}
    void Change2EdiNormal::sprocess(bool vclmodeout) {
        using namespace mode ;
        auto mode = get_global_mode() ;
        if(mode == Mode::GUI_NORMAL) {
            mouse::click(KEYCODE_MOUSE_LEFT) ;
        }
        if(mode == Mode::EDI_NORMAL) {
            return ;
        }
        if(mode == Mode::EDI_VISUAL) {
            textselect::unselect() ;
        }

        keyabsorber::close_all_ports_with_refresh() ;
        keyabsorber::absorb() ;

        set_global_mode(Mode::EDI_NORMAL) ;
        if(vclmodeout) {
            VirtualCmdLine::reset() ;
            VirtualCmdLine::msgout("-- EDI NORMAL --") ;
        }
    }
    void Change2EdiNormal::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(true) ;
        }
    }
    void Change2EdiNormal::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess(true) ;
    }


    //Change2Insert
    Change2Insert::Change2Insert()
    : BindedFuncCreator("change_to_insert")
    {}

    void Change2Insert::sprocess(bool vclmodeout) {
        using namespace mode ;
        keyabsorber::close_all_ports() ;
        keyabsorber::unabsorb() ;
        set_global_mode(Mode::INSERT) ;
        if(vclmodeout) {
            VirtualCmdLine::reset() ;
            VirtualCmdLine::msgout("-- INSERT --") ;
        }
    }
    void Change2Insert::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(true) ;
        }
    }
    void Change2Insert::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess(true) ;
    }


    //Change2EdiVisual
    Change2EdiVisual::Change2EdiVisual()
    : BindedFuncCreator("change_to_edi_visual")
    {}
    void Change2EdiVisual::sprocess(bool vclmodeout) {
        using namespace mode ;

        textselect::select_words() ;
        set_global_mode(Mode::EDI_VISUAL) ;
        if(vclmodeout) {
            VirtualCmdLine::reset() ;
            VirtualCmdLine::msgout("-- EDI VISUAL --") ;
        }
    }
    void Change2EdiVisual::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(true) ;
        }
    }
    void Change2EdiVisual::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess(true) ;
    }


    //Change2EdiLineVisual
    Change2EdiLineVisual::Change2EdiLineVisual()
    : BindedFuncCreator("change_to_edi_line_visual")
    {}
    void Change2EdiLineVisual::sprocess(bool vclmodeout) {
        using namespace mode ;

        textselect::select_line_EOL2BOL() ;
        set_global_mode(Mode::EDI_VISUAL, ModeFlags::VISUAL_LINE) ;
        if(vclmodeout) {
            VirtualCmdLine::reset() ;
            VirtualCmdLine::msgout("-- EDI VISUAL LINE--") ;
        }
    }
    void Change2EdiLineVisual::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(true) ;
        }
    }
    void Change2EdiLineVisual::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess(true) ;
    }
}
