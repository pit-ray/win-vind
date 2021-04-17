#include "bind/emu/edi_change_mode.hpp"

#include <iostream>
#include <windows.h>

#include "bind/base/mode.hpp"
#include "io/keybrd.hpp"
#include "io/mouse.hpp"
#include "key/key_absorber.hpp"
#include "opt/virtual_cmd_line.hpp"
#include "text/simple_text_selecter.hpp"
#include "util/def.hpp"

#include "bind/base/ntype_logger.hpp"


namespace vind
{
    //Change2EdiNormal
    Change2EdiNormal::Change2EdiNormal()
    : BindedFuncCreator("change_to_edi_normal")
    {}
    void Change2EdiNormal::sprocess(bool vclmodeout) {
        if(!mode::is_editor())
            mouse::click(KEYCODE_MOUSE_LEFT) ;

        using namespace mode ;
        if(get_global_mode() == Mode::EdiNormal) return ;

        if(is_edi_visual())
            textselect::unselect() ;

        keyabsorber::close_all_ports_with_refresh() ;
        keyabsorber::absorb() ;

        change_mode(Mode::EdiNormal) ;
        if(vclmodeout)
            VirtualCmdLine::msgout("-- EDI NORMAL --") ;

    }
    void Change2EdiNormal::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(true) ;
        }
    }
    void Change2EdiNormal::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess(true) ;
    }


    //Change2EdiInsert
    Change2EdiInsert::Change2EdiInsert()
    : BindedFuncCreator("change_to_edi_insert")
    {}

    void Change2EdiInsert::sprocess(bool vclmodeout) {
        using namespace mode ;
        keyabsorber::close_all_ports() ;
        keyabsorber::unabsorb() ;
        change_mode(Mode::EdiInsert) ;
        if(vclmodeout)
            VirtualCmdLine::msgout("-- EDI INSERT --") ;
    }
    void Change2EdiInsert::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(true) ;
        }
    }
    void Change2EdiInsert::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess(true) ;
    }


    //Change2EdiBOLInsert
    Change2EdiBOLInsert::Change2EdiBOLInsert()
    : BindedFuncCreator("change_to_edi_BOLinsert")
    {}
    void Change2EdiBOLInsert::sprocess(bool vclmodeout) {
        keybrd::pushup(KEYCODE_HOME) ;
        Change2EdiInsert::sprocess(vclmodeout) ;
    }
    void Change2EdiBOLInsert::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(true) ;
        }
    }
    void Change2EdiBOLInsert::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess(true) ;
    }


    //Change2EdiBkInsert
    Change2EdiBkInsert::Change2EdiBkInsert()
    : BindedFuncCreator("change_to_edi_bkinsert")
    {}
    void Change2EdiBkInsert::sprocess(bool vclmodeout) {
        keybrd::pushup(KEYCODE_RIGHT) ;
        Change2EdiInsert::sprocess(vclmodeout) ;
    }
    void Change2EdiBkInsert::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(true) ;
        }
    }
    void Change2EdiBkInsert::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess(true) ;
    }


    //Change2EdiEOLInsert
    Change2EdiEOLInsert::Change2EdiEOLInsert()
    : BindedFuncCreator("change_to_edi_EOLinsert")
    {}
    void Change2EdiEOLInsert::sprocess(bool vclmodeout) {
        keybrd::pushup(KEYCODE_END) ;
        Change2EdiInsert::sprocess(vclmodeout) ;
    }
    void Change2EdiEOLInsert::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(true) ;
        }
    }
    void Change2EdiEOLInsert::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess(true) ;
    }


    //Change2EdiNlInsertBelow
    Change2EdiNlInsertBelow::Change2EdiNlInsertBelow()
    : BindedFuncCreator("change_to_edi_nlinsert_below")
    {}
    void Change2EdiNlInsertBelow::sprocess(bool vclmodeout) {
        keybrd::pushup(KEYCODE_END) ;
        keybrd::pushup(KEYCODE_ENTER) ;
        Change2EdiInsert::sprocess(vclmodeout) ;
    }
    void Change2EdiNlInsertBelow::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(true) ;
        }
    }
    void Change2EdiNlInsertBelow::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess(true) ;
    }


    //Change2EdiNlInsertAbove
    Change2EdiNlInsertAbove::Change2EdiNlInsertAbove()
    : BindedFuncCreator("change_to_edi_nlinsert_above")
    {}
    void Change2EdiNlInsertAbove::sprocess(bool vclmodeout) {
        keybrd::pushup(KEYCODE_HOME) ;
        keybrd::pushup(KEYCODE_ENTER) ;
        keybrd::pushup(KEYCODE_UP) ;
        Change2EdiInsert::sprocess(vclmodeout) ;
    }
    void Change2EdiNlInsertAbove::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(true) ;
        }
    }
    void Change2EdiNlInsertAbove::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess(true) ;
    }


    //Change2EdiVisual
    Change2EdiVisual::Change2EdiVisual()
    : BindedFuncCreator("change_to_edi_visual")
    {}
    void Change2EdiVisual::sprocess(bool vclmodeout) {
        using namespace mode ;

        textselect::select_words() ;
        change_mode(Mode::EdiVisual) ;
        if(vclmodeout)
            VirtualCmdLine::msgout("-- EDI VISUAL --") ;
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
        change_mode(Mode::EdiLineVisual) ;
        if(vclmodeout)
            VirtualCmdLine::msgout("-- EDI VISUAL LINE--") ;
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
