#include "undo.hpp"

#include <windows.h>

#include <iostream>

#include "keybrd_eventer.hpp"
#include "keystroke_repeater.hpp"
#include "mouse_eventer.hpp"
#include "utility.hpp"
using namespace std ;

//SCRedo
struct SCRedo::Impl
{
    KeyStrokeRepeater ksr{} ;
} ;

SCRedo::SCRedo()
: pimpl(std::make_unique<Impl>())
{}

SCRedo::~SCRedo() noexcept = default ;
SCRedo::SCRedo(SCRedo&&) noexcept = default ;
SCRedo& SCRedo::operator=(SCRedo&&) noexcept = default ;
const string SCRedo::sname() noexcept
{
    return "sc_redo" ;
}

bool SCRedo::sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const UNUSED(parent_logger)) const
{
    auto redo = [] {
        return KeybrdEventer::pushup(VKC_LCTRL, VKC_Y) ;
    } ;
    if(repeat_num == 1) {
        if(first_call) {
            pimpl->ksr.reset() ;
            return redo() ;
        }
        if(pimpl->ksr.is_pressed()) return redo() ;
        return true ;
    }

    //repeat_num > 1
    if(!first_call) return true ;
    for(unsigned int i = 0 ; i < repeat_num ; i ++) {
        if(!redo()) return false ;
    }
    return true ;
}


//SCUndo
struct SCUndo::Impl
{
    KeyStrokeRepeater ksr{} ;
} ;

SCUndo::SCUndo()
: pimpl(std::make_unique<Impl>())
{}

SCUndo::~SCUndo() noexcept = default ;
SCUndo::SCUndo(SCUndo&&) noexcept = default ;
SCUndo& SCUndo::operator=(SCUndo&&) noexcept = default ;
const string SCUndo::sname() noexcept
{
    return "sc_undo" ;
}
bool SCUndo::sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const UNUSED(parent_logger)) const
{
    auto undo = [] {
       return KeybrdEventer::pushup(VKC_LCTRL, VKC_Z) ;
    } ;

    if(repeat_num == 1) {
        if(first_call) {
            pimpl->ksr.reset() ;
            return undo() ;
        }
        if(pimpl->ksr.is_pressed()) {
            return undo() ;
        }
        return true ;
    }

    //repeat_num > 1
    if(!first_call) return true ;
    for(unsigned int i = 0 ; i < repeat_num ; i ++) {
        if(!undo()) return false ;
    }
    return true ;
}
