#include "undo.hpp"

#include <windows.h>

#include <iostream>

#include "io/keybrd.hpp"
#include "io/mouse.hpp"
#include "time/keystroke_repeater.hpp"
#include "util/def.hpp"

namespace vind
{
    //SCRedo
    struct SCRedo::Impl {
        KeyStrokeRepeater ksr{} ;
    } ;

    SCRedo::SCRedo()
    : pimpl(std::make_unique<Impl>())
    {}

    SCRedo::~SCRedo() noexcept          = default ;
    SCRedo::SCRedo(SCRedo&&)            = default ;
    SCRedo& SCRedo::operator=(SCRedo&&) = default ;
    const std::string SCRedo::sname() noexcept {
        return "sc_redo" ;
    }

    void SCRedo::sprocess(
            bool first_call,
            unsigned int repeat_num,
            KeycodeLogger* const UNUSED(parent_keycodelgr),
            const CharLogger* const UNUSED(parent_charlgr)) const {
        auto redo = [] {keybrd::pushup(KEYCODE_LCTRL, KEYCODE_Y) ;} ;
        if(repeat_num == 1) {
            if(first_call) {
                pimpl->ksr.reset() ;
                redo() ;
                return ;
            }
            if(pimpl->ksr.is_pressed()) {
                redo() ;
                return ;
            }
            return ;
        }

        //repeat_num > 1
        if(first_call) {
            for(unsigned int i = 0 ; i < repeat_num ; i ++) {
                redo() ;
            }
            pimpl->ksr.reset() ;
        }
    }


    //SCUndo
    struct SCUndo::Impl {
        KeyStrokeRepeater ksr{} ;
    } ;

    SCUndo::SCUndo()
    : pimpl(std::make_unique<Impl>())
    {}

    SCUndo::~SCUndo() noexcept          = default ;
    SCUndo::SCUndo(SCUndo&&)            = default ;
    SCUndo& SCUndo::operator=(SCUndo&&) = default ;
    const std::string SCUndo::sname() noexcept {
        return "sc_undo" ;
    }
    void SCUndo::sprocess(
            bool first_call,
            unsigned int repeat_num,
            KeycodeLogger* const UNUSED(parent_keycodelgr),
            const CharLogger* const UNUSED(parent_charlgr)) const {
        auto undo = [] {keybrd::pushup(KEYCODE_LCTRL, KEYCODE_Z) ;} ;
        if(repeat_num == 1) {
            if(first_call) {
                pimpl->ksr.reset() ;
                undo() ;
                return ;
            }
            if(pimpl->ksr.is_pressed()) {
                undo() ;
                return ;
            }
            return ;
        }

        //repeat_num >= 2
        if(first_call) {
            for(unsigned int i = 0 ; i < repeat_num ; i ++) {
                undo() ;
            }
            pimpl->ksr.reset() ;
        }
    }
}
