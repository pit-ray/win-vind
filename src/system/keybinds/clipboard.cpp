#include "clipboard.hpp"

#include <windows.h>
#include <iostream>

//debug
#include <cstring>

#include "mouse_eventer.hpp"
#include "keybrd_eventer.hpp"
#include "mode_manager.hpp"
#include "key_absorber.hpp"

using namespace std ;

//CBCopy
const string CBCopy::sname() noexcept
{
    return "cb_copy" ;
}

bool CBCopy::sprocess(const bool first_call)
{
    if(!first_call) return true ;

    if(!MouseEventer::is_up(MouseEventer::Button::LEFT)) {
        return false ;
    }

    //there are cases in which not editable.
    //thus, not use Message Copy
    if(!KeybrdEventer::is_pushup(VKC_LCTRL, VKC_INSERT)) {
        return false ;
    }

    ModeManager::change_mode(ModeManager::Mode::Normal) ;

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

    ModeManager::change_mode(ModeManager::Mode::Normal) ;

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

    ModeManager::change_mode(ModeManager::Mode::Normal) ;

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


//CBBackSpace
const string CBBackSpace::sname() noexcept
{
    return "cb_back_space" ;
}

bool CBBackSpace::sprocess(const bool first_call)
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

    if(!KeybrdEventer::is_pushup(VKC_BKSPACE)) {
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