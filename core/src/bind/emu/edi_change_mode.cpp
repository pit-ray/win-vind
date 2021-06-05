#include "bind/emu/edi_change_mode.hpp"

#include <iostream>
#include <windows.h>

#include "bind/emu/simple_text_selecter.hpp"
#include "bind/mode/change_mode.hpp"
#include "io/keybrd.hpp"
#include "io/mouse.hpp"
#include "key/key_absorber.hpp"
#include "mode.hpp"
#include "opt/virtual_cmd_line.hpp"
#include "util/def.hpp"

#include "key/ntype_logger.hpp"


namespace vind
{
    //Change2EdiBOLInsert
    Change2EdiBOLInsert::Change2EdiBOLInsert()
    : BindedFuncCreator("change_to_edi_BOLinsert")
    {}
    void Change2EdiBOLInsert::sprocess(bool vclmodeout) {
        keybrd::pushup(KEYCODE_HOME) ;
        Change2Insert::sprocess(vclmodeout) ;
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
        Change2Insert::sprocess(vclmodeout) ;
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
        Change2Insert::sprocess(vclmodeout) ;
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
        Change2Insert::sprocess(vclmodeout) ;
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
        Change2Insert::sprocess(vclmodeout) ;
    }
    void Change2EdiNlInsertAbove::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(true) ;
        }
    }
    void Change2EdiNlInsertAbove::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess(true) ;
    }
}
