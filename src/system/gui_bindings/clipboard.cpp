#include "clipboard.hpp"

#include <windows.h>
#include <iostream>

//debug
#include <cstring>

#include "mouse_eventer.hpp"
#include "keybrd_eventer.hpp"
#include "mode_manager.hpp"
#include "key_absorber.hpp"
#include "change_mode.hpp"
#include "editor_bindings/edi_change_mode.hpp"

using namespace std ;

//CBCopy
const string CBCopy::sname() noexcept
{
    return "cb_copy" ;
}

void CBCopy::sprocess(
        const bool first_call,
        const unsigned int repeat_num,
        const KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr))
{
    if(!first_call) return ;

    MouseEventer::release(VKC_MOUSE_LEFT) ;

    //there are cases in which not editable.
    //thus, not use Message Copy
    KeybrdEventer::pushup(VKC_LCTRL, VKC_INSERT) ;
}


//CBPaste
const string CBPaste::sname() noexcept
{
    return "cb_paste" ;
}

void CBPaste::sprocess(
        const bool first_call,
        const unsigned int repeat_num,
        const KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr))
{
    if(!first_call) return ;

    MouseEventer::release(VKC_MOUSE_LEFT) ;

    //not selecting at paste.
    KeybrdEventer::pushup(VKC_LSHIFT, VKC_INSERT) ;
    Change2Normal::sprocess(true, 1, nullptr, nullptr) ;
}


//CBCut
const string CBCut::sname() noexcept
{
    return "cb_cut" ;
}

void CBCut::sprocess(
        const bool first_call,
        const unsigned int repeat_num,
        const KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr))
{
    if(!first_call) return ;

    MouseEventer::release(VKC_MOUSE_LEFT) ;
    KeybrdEventer::pushup(VKC_LCTRL, VKC_X) ;
    Change2Normal::sprocess(true, 1, nullptr, nullptr) ;
}


//CBDelete
const string CBDelete::sname() noexcept
{
    return "cb_delete" ;
}

void CBDelete::sprocess(
        const bool first_call,
        const unsigned int repeat_num,
        const KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr))
{
    if(!first_call) return ;
    MouseEventer::release(VKC_MOUSE_LEFT) ;

    //selecting->cut
    //unselecting->delete
    KeybrdEventer::pushup(VKC_LCTRL, VKC_C) ;
    KeybrdEventer::pushup(VKC_DELETE) ;

    using namespace ModeManager ;
    if(is_editor()) {
        Change2EdiNormal::sprocess(true, 1, nullptr, nullptr) ;
    }
    else {
        Change2Normal::sprocess(true, 1, nullptr, nullptr) ;
    }
}


//CBBackSpace
const string CBBackSpace::sname() noexcept
{
    return "cb_back_space" ;
}

void CBBackSpace::sprocess(
        const bool first_call,
        const unsigned int repeat_num,
        const KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr))
{
    if(!first_call) return ;

    MouseEventer::release(VKC_MOUSE_LEFT) ;

    //selecting->cut
    //unselecting->delete
    KeybrdEventer::pushup(VKC_LCTRL, VKC_C) ;

    KeybrdEventer::pushup(VKC_BKSPACE) ;

    using namespace ModeManager ;
    if(is_editor()) {
        Change2EdiNormal::sprocess(true, 1, nullptr, nullptr) ;
    }
    else {
        Change2Normal::sprocess(true, 1, nullptr, nullptr) ;
    }
}
