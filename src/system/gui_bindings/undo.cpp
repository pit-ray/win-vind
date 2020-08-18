#include "undo.hpp"
#include "mouse_eventer.hpp"
#include "keybrd_eventer.hpp"
#include <windows.h>
#include <richedit.h>

#include <iostream>
using namespace std ;

//SCRedo
const string SCRedo::sname() noexcept
{
    return "sc_redo" ;
}

bool SCRedo::sprocess(const bool first_call)
{
    if(!first_call) return true ;
    if(!KeybrdEventer::pressup(VKC_LCTRL, VKC_Y)) {
        return false ;
    }
    return true ;
}


//SCUndo
const string SCUndo::sname() noexcept
{
    return "sc_undo" ;
}

bool SCUndo::sprocess(const bool first_call)
{
    if(!first_call) return true ;
    if(!KeybrdEventer::pressup(VKC_LCTRL, VKC_Z)) {
        return false ;
    }
    return true ;
}