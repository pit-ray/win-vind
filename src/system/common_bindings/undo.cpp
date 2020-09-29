#include "undo.hpp"

#include <windows.h>

#include <iostream>

#include "keybrd_eventer.hpp"
#include "keystroke_repeater.hpp"
#include "mouse_eventer.hpp"
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

bool SCRedo::sprocess(const bool first_call) const
{
    auto redo = [] {
        return KeybrdEventer::pushup(VKC_LCTRL, VKC_Y) ;
    } ;

    if(first_call) {
        pimpl->ksr.reset() ;
        return redo() ;
    }
    if(pimpl->ksr.is_pressed()) {
        return redo() ;
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
bool SCUndo::sprocess(const bool first_call) const
{
    auto undo = [] {
       return KeybrdEventer::pushup(VKC_LCTRL, VKC_Z) ;
    } ;

    if(first_call) {
        pimpl->ksr.reset() ;
        return undo() ;
    }
    if(pimpl->ksr.is_pressed()) {
        return undo() ;
    }
    return true ;
}
