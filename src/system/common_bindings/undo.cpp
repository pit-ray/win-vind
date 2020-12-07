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

SCRedo::~SCRedo() noexcept          = default ;
SCRedo::SCRedo(SCRedo&&)            = default ;
SCRedo& SCRedo::operator=(SCRedo&&) = default ;
const string SCRedo::sname() noexcept
{
    return "sc_redo" ;
}

void SCRedo::sprocess(
        const bool first_call,
        const unsigned int repeat_num,
        KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr)) const
{
    auto redo = [] {KeybrdEventer::pushup(VKC_LCTRL, VKC_Y) ;} ;
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
    if(!first_call) return ;
    for(unsigned int i = 0 ; i < repeat_num ; i ++) redo() ;
}


//SCUndo
struct SCUndo::Impl
{
    KeyStrokeRepeater ksr{} ;
} ;

SCUndo::SCUndo()
: pimpl(std::make_unique<Impl>())
{}

SCUndo::~SCUndo() noexcept          = default ;
SCUndo::SCUndo(SCUndo&&)            = default ;
SCUndo& SCUndo::operator=(SCUndo&&) = default ;
const string SCUndo::sname() noexcept
{
    return "sc_undo" ;
}
void SCUndo::sprocess(
        const bool first_call,
        const unsigned int repeat_num,
        KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr)) const
{
    auto undo = [] {KeybrdEventer::pushup(VKC_LCTRL, VKC_Z) ;} ;
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

    //repeat_num > 1
    if(!first_call) return ;
    for(unsigned int i = 0 ; i < repeat_num ; i ++) {
        undo() ;
    }
}
