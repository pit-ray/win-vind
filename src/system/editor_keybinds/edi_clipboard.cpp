#include "clipboard.hpp"

#include <windows.h>
#include <iostream>

//debug
#include <cstring>

#include "mouse_eventer.hpp"
#include "keybrd_eventer.hpp"
#include "mode_manager.hpp"
#include "key_absorber.hpp"
#include "alternative_text_selecter.hpp"


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

    inline static bool is_unselect_and_change_mode() {
        using namespace ModeManager ;
        if(is_editor()) {
            change_mode(Mode::EdiNormal) ;
            if(!TextSelecter::is_unselect()) {
                return false ;
            }
        }
        else {
            change_mode(Mode::Normal) ;
            if(!MouseEventer::is_up(MouseEventer::Button::LEFT)) {
                return false ;
            }
        }

        return true ;
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

    //there are cases in which not editable.
    //thus, not use Message Copy
    if(!KeybrdEventer::is_pushup(VKC_LCTRL, VKC_INSERT)) {
        return false ;
    }

    if(!is_unselect_and_change_mode()) {
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

    //not selecting at paste.
    if(!KeybrdEventer::is_pushup(VKC_LSHIFT, VKC_INSERT)) {
        return false ;
    }


    if(!is_unselect_and_change_mode()) {
        return false ;
    }

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

    if(!KeybrdEventer::is_pushup(VKC_LCTRL, VKC_X)) {
        return false ;
    }

    if(!is_unselect_and_change_mode()) {
        return false ;
    }


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

    //selecting->cut
    //unselecting->delete
    if(!KeybrdEventer::is_pushup(VKC_LCTRL, VKC_C)) {
        return false ;
    }

    if(!KeybrdEventer::is_pushup(VKC_DELETE)) {
        return false ;
    }

    using namespace ModeManager ;
    if(is_editor()) {
        change_mode(Mode::EdiNormal) ;
    }
    else {
        change_mode(Mode::Normal) ;
    }

    return true ;
}