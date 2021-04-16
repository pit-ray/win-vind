#include "bind/hotkey/clipboard.hpp"

#include <windows.h>
#include <iostream>

#include "bind/base/char_logger.hpp"
#include "bind/base/mode.hpp"

#include "bind/base/ntype_logger.hpp"
#include "util/def.hpp"

#include "bind/emu/edi_change_mode.hpp"
#include "bind/mode/change_mode.hpp"
#include "io/keybrd.hpp"
#include "io/mouse.hpp"
#include "key/key_absorber.hpp"

namespace vind
{
    //CBCopy
    CBCopy::CBCopy()
    : BindedFuncCreator("cb_copy")
    {}
    void CBCopy::sprocess() {
        mouse::release(KEYCODE_MOUSE_LEFT) ;

        //there are cases in which not editable.
        //thus, not use Message Copy
        keybrd::pushup(KEYCODE_LCTRL, KEYCODE_C) ;
    }
    void CBCopy::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess() ;
        }
    }
    void CBCopy::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess() ;
    }


    //CBPaste
    CBPaste::CBPaste()
    : BindedFuncCreator("cb_paste")
    {}
    void CBPaste::sprocess() {
        mouse::release(KEYCODE_MOUSE_LEFT) ;

        //not selecting at paste.
        keybrd::pushup(KEYCODE_LCTRL, KEYCODE_V) ;
    }
    void CBPaste::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess() ;
        }
    }
    void CBPaste::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess() ;
    }


    //CBCut
    CBCut::CBCut()
    : BindedFuncCreator("cb_cut")
    {}
    void CBCut::sprocess() {
        mouse::release(KEYCODE_MOUSE_LEFT) ;
        keybrd::pushup(KEYCODE_LCTRL, KEYCODE_X) ;
    }
    void CBCut::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess() ;
        }
    }
    void CBCut::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess() ;
    }


    //CBDelete
    CBDelete::CBDelete()
    : BindedFuncCreator("cb_delete")
    {}
    void CBDelete::sprocess() {
        mouse::release(KEYCODE_MOUSE_LEFT) ;

        //selecting->cut
        //unselecting->delete
        keybrd::pushup(KEYCODE_LCTRL, KEYCODE_C) ;
        keybrd::pushup(KEYCODE_DELETE) ;
    }
    void CBDelete::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess() ;
        }
    }
    void CBDelete::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess() ;
    }


    //CBBackSpace
    CBBackSpace::CBBackSpace()
    : BindedFuncCreator("cb_back_space")
    {}

    void CBBackSpace::sprocess() {
        mouse::release(KEYCODE_MOUSE_LEFT) ;

        //selecting->cut
        //unselecting->delete
        keybrd::pushup(KEYCODE_LCTRL, KEYCODE_C) ;
        keybrd::pushup(KEYCODE_BKSPACE) ;
    }
    void CBBackSpace::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess() ;
        }
    }
    void CBBackSpace::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess() ;
    }
}
