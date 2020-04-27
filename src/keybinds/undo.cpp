#include "undo.hpp"
#include "mouse_eventer.hpp"
#include "keybrd_eventer.hpp"
#include "system.hpp"
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
    /*
    POINT pos ;
    if(!GetCursorPos(&pos)) return false ;
    const auto hwnd = WindowFromPoint(pos) ;
    SendMessage(hwnd, EM_REDO, 0, 0) ;
    */
    if(!KeybrdEventer::is_pushup(VKC_LCTRL, VKC_Y)) {
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
    /*
    POINT pos ;
    if(!GetCursorPos(&pos)) return false ;
    const auto hwnd = WindowFromPoint(pos) ;
    SendMessage(hwnd, EM_UNDO, 0, 0) ;
    */
    if(!KeybrdEventer::is_pushup(VKC_LCTRL, VKC_Z)) {
        return false ;
    }
    return true ;
}