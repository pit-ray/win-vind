#include "bind/emu/edi_change_mode.hpp"

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

#if defined(DEBUG)
#include <iostream>
#endif


namespace vind
{
    //ToInsertBOL
    ToInsertBOL::ToInsertBOL()
    : BindedFuncCreator("to_insert_BOL")
    {}
    void ToInsertBOL::sprocess(bool vclmodeout) {
        keybrd::pushup(KEYCODE_HOME) ;
        ToInsert::sprocess(vclmodeout) ;
    }
    void ToInsertBOL::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(true) ;
        }
    }
    void ToInsertBOL::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess(true) ;
    }


    //Change2EdiBkInsert
    Change2EdiBkInsert::Change2EdiBkInsert()
    : BindedFuncCreator("to_insert_append")
    {}
    void Change2EdiBkInsert::sprocess(bool vclmodeout) {
        keybrd::pushup(KEYCODE_RIGHT) ;
        ToInsert::sprocess(vclmodeout) ;
    }
    void Change2EdiBkInsert::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(true) ;
        }
    }
    void Change2EdiBkInsert::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess(true) ;
    }


    //ToInsertEOL
    ToInsertEOL::ToInsertEOL()
    : BindedFuncCreator("to_insert_EOL")
    {}
    void ToInsertEOL::sprocess(bool vclmodeout) {
        keybrd::pushup(KEYCODE_END) ;
        ToInsert::sprocess(vclmodeout) ;
    }
    void ToInsertEOL::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(true) ;
        }
    }
    void ToInsertEOL::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess(true) ;
    }


    //ToInsertNLBelow
    ToInsertNLBelow::ToInsertNLBelow()
    : BindedFuncCreator("to_insert_nlbelow")
    {}
    void ToInsertNLBelow::sprocess(bool vclmodeout) {
        keybrd::pushup(KEYCODE_END) ;
        keybrd::pushup(KEYCODE_ENTER) ;
        ToInsert::sprocess(vclmodeout) ;
    }
    void ToInsertNLBelow::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(true) ;
        }
    }
    void ToInsertNLBelow::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess(true) ;
    }


    //ToInsertNLAbove
    ToInsertNLAbove::ToInsertNLAbove()
    : BindedFuncCreator("to_insert_nlabove")
    {}
    void ToInsertNLAbove::sprocess(bool vclmodeout) {
        keybrd::pushup(KEYCODE_HOME) ;
        keybrd::pushup(KEYCODE_ENTER) ;
        keybrd::pushup(KEYCODE_UP) ;
        ToInsert::sprocess(vclmodeout) ;
    }
    void ToInsertNLAbove::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(true) ;
        }
    }
    void ToInsertNLAbove::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess(true) ;
    }
}
