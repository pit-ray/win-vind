#include "clipboard.hpp"
#include "mouse_eventer.hpp"
#include "keybrd_eventer.hpp"
#include "key_absorber.hpp"
#include "system.hpp"

#include <windows.h>
#include <iostream>

//debug
#include <cstring>

using namespace std ;

namespace ClipboardUtility
{
    inline static bool is_selecting(const HWND hwnd) noexcept {
        DWORD32 start{0}, end{0} ;
        SendMessage(hwnd, EM_GETSEL,
            reinterpret_cast<WPARAM>(&start),
            reinterpret_cast<LPARAM>(&end)) ;

        return end != 0 && start != end ;
    }
}

using namespace ClipboardUtility ;


//CBCopy
const string CBCopy::sname() noexcept
{
    return "cb_copy" ;
}

bool CBCopy::sprocess(const bool first_call)
{
    if(!first_call) return true ;

    System::change_mode(System::Mode::Normal) ;

    if(!MouseEventer::is_up(MouseEventer::Button::LEFT)) {
        return false ;
    }

    //there are cases in which not editable.
    //thus, not use Message Copy
    if(!KeybrdEventer::is_pushup(VKC_LCTRL, VKC_INSERT)) {
        return false ;
    }

    return true ;
}


//CBPaste
const string CBPaste::sname() noexcept
{
    return "cb_paste" ;
}

bool CBPaste::sprocess(const bool first_call)
{
    if(!first_call) return true ;
    
    if(!MouseEventer::is_up(MouseEventer::Button::LEFT)) {
        return false ;
    }

    //not selecting at paste.
    if(!KeybrdEventer::is_pushup(VKC_LSHIFT, VKC_INSERT)) {
        return false ;
    }

    System::change_mode(System::Mode::Normal) ;
    return true ;
}


//CBCut
const string CBCut::sname() noexcept
{
    return "cb_cut" ;
}

bool CBCut::sprocess(const bool first_call)
{
    if(!first_call) return true ;

    if(!MouseEventer::is_up(MouseEventer::Button::LEFT)) {
        return false ;
    }

    if(!KeybrdEventer::is_pushup(VKC_LCTRL, VKC_X)) {
        return false ;
    }

    KeyAbsorber::close() ;
    System::change_mode(System::Mode::Normal) ;
    return true ;
}


//CBDelete
const string CBDelete::sname() noexcept
{
    return "cb_delete" ;
}

bool CBDelete::sprocess(const bool first_call)
{
    if(!first_call) return true ;

    if(!MouseEventer::is_up(MouseEventer::Button::LEFT)) {
        return false ;
    }

    //selecting->cut
    //unselecting->delete
    if(!KeybrdEventer::is_pushup(VKC_LCTRL, VKC_C)) {
        return false ;
    }

    if(KeyAbsorber::is_down(VKC_DELETE)) {
        if(!KeybrdEventer::is_release(VKC_DELETE)) {
            return false ;
        }
    }

    if(!KeybrdEventer::is_pushup(VKC_DELETE)) {
        return false ;
    }

    KeyAbsorber::close() ;
    System::change_mode(System::Mode::Normal) ;
    return true ;
}