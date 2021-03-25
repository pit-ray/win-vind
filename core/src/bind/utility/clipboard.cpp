#include "clipboard.hpp"

#include <windows.h>
#include <iostream>

#include "emu/edi_change_mode.hpp"
#include "io/keybrd_eventer.hpp"
#include "io/mouse_eventer.hpp"
#include "key/key_absorber.hpp"
#include "mode/change_mode.hpp"
#include "mode_manager.hpp"

namespace vind
{
    //CBCopy
    const std::string CBCopy::sname() noexcept {
        return "cb_copy" ;
    }

    void CBCopy::sprocess(
            const bool first_call,
            const unsigned int UNUSED(repeat_num),
            VKCLogger* const UNUSED(parent_vkclgr),
            const CharLogger* const UNUSED(parent_charlgr)) {
        if(!first_call) return ;
        MouseEventer::release(VKC_MOUSE_LEFT) ;

        //there are cases in which not editable.
        //thus, not use Message Copy
        KeybrdEventer::pushup(VKC_LCTRL, VKC_C) ;
    }


    //CBPaste
    const std::string CBPaste::sname() noexcept {
        return "cb_paste" ;
    }

    void CBPaste::sprocess(
            const bool first_call,
            const unsigned int UNUSED(repeat_num),
            VKCLogger* const UNUSED(parent_vkclgr),
            const CharLogger* const UNUSED(parent_charlgr)) {
        if(!first_call) return ;

        MouseEventer::release(VKC_MOUSE_LEFT) ;

        //not selecting at paste.
        KeybrdEventer::pushup(VKC_LCTRL, VKC_V) ;
    }


    //CBCut
    const std::string CBCut::sname() noexcept {
        return "cb_cut" ;
    }

    void CBCut::sprocess(
            const bool first_call,
            const unsigned int UNUSED(repeat_num),
            VKCLogger* const UNUSED(parent_vkclgr),
            const CharLogger* const UNUSED(parent_charlgr)) {
        if(!first_call) return ;

        MouseEventer::release(VKC_MOUSE_LEFT) ;
        KeybrdEventer::pushup(VKC_LCTRL, VKC_X) ;
    }


    //CBDelete
    const std::string CBDelete::sname() noexcept {
        return "cb_delete" ;
    }

    void CBDelete::sprocess(
            const bool first_call,
            const unsigned int UNUSED(repeat_num),
            VKCLogger* const UNUSED(parent_vkclgr),
            const CharLogger* const UNUSED(parent_charlgr)) {
        if(!first_call) return ;
        MouseEventer::release(VKC_MOUSE_LEFT) ;

        //selecting->cut
        //unselecting->delete
        KeybrdEventer::pushup(VKC_LCTRL, VKC_C) ;
        KeybrdEventer::pushup(VKC_DELETE) ;
    }


    //CBBackSpace
    const std::string CBBackSpace::sname() noexcept {
        return "cb_back_space" ;
    }

    void CBBackSpace::sprocess(
            const bool first_call,
            const unsigned int UNUSED(repeat_num),
            VKCLogger* const UNUSED(parent_vkclgr),
            const CharLogger* const UNUSED(parent_charlgr)) {
        if(!first_call) return ;
        MouseEventer::release(VKC_MOUSE_LEFT) ;

        //selecting->cut
        //unselecting->delete
        KeybrdEventer::pushup(VKC_LCTRL, VKC_C) ;
        KeybrdEventer::pushup(VKC_BKSPACE) ;
    }
}
