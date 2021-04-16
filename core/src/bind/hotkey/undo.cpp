#include "bind/hotkey/undo.hpp"

#include <windows.h>

#include <iostream>

#include "io/keybrd.hpp"
#include "io/mouse.hpp"
#include "time/keystroke_repeater.hpp"
#include "util/def.hpp"
#include "bind/base/ntype_logger.hpp"

namespace vind
{
    //SCRedo
    struct SCRedo::Impl {
        KeyStrokeRepeater ksr{} ;
    } ;

    SCRedo::SCRedo()
    : BindedFuncCreator("sc_redo"),
      pimpl(std::make_unique<Impl>())
    {}

    SCRedo::~SCRedo() noexcept          = default ;
    SCRedo::SCRedo(SCRedo&&)            = default ;
    SCRedo& SCRedo::operator=(SCRedo&&) = default ;

    void SCRedo::sprocess(unsigned int repeat_num) const {
        for(decltype(repeat_num) i = 0 ; i < repeat_num ; i ++) {
            keybrd::pushup(KEYCODE_LCTRL, KEYCODE_Y) ;
        }
    }
    void SCRedo::sprocess(NTypeLogger& parent_lgr) const {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(parent_lgr.get_head_num()) ;
            pimpl->ksr.reset() ;
        }
        else if(pimpl->ksr.is_pressed()) {
            sprocess(1) ;
        }
    }
    void SCRedo::sprocess(const CharLogger& UNUSED(parent_lgr)) const {
        sprocess() ;
    }


    //SCUndo
    struct SCUndo::Impl {
        KeyStrokeRepeater ksr{} ;
    } ;

    SCUndo::SCUndo()
    : BindedFuncCreator("sc_undo"),
      pimpl(std::make_unique<Impl>())
    {}

    SCUndo::~SCUndo() noexcept          = default ;
    SCUndo::SCUndo(SCUndo&&)            = default ;
    SCUndo& SCUndo::operator=(SCUndo&&) = default ;

    void SCUndo::sprocess(unsigned int repeat_num) const {
        for(decltype(repeat_num) i = 0 ; i < repeat_num ; i ++) {
            keybrd::pushup(KEYCODE_LCTRL, KEYCODE_Z) ;
        }
    }
    void SCUndo::sprocess(NTypeLogger& parent_lgr) const {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(parent_lgr.get_head_num()) ;
            pimpl->ksr.reset() ;
        }
        else if(pimpl->ksr.is_pressed()) {
            sprocess(1) ;
        }
    }
    void SCUndo::sprocess(const CharLogger& UNUSED(parent_lgr)) const {
        sprocess() ;
    }
}
