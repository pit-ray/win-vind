#include "edi_move_caret.hpp"

#include "interval_timer.hpp"
#include "key_absorber.hpp"
#include "keybrd_eventer.hpp"
#include "keystroke_repeater.hpp"
#include "mode_manager.hpp"
#include "msg_logger.hpp"
#include "simpl_text_selecter.hpp"
#include "utility.hpp"

using namespace std ;

//EdiMoveCaretLeft
struct EdiMoveCaretLeft::Impl
{
    KeyStrokeRepeater calcer{} ;
} ;

EdiMoveCaretLeft::EdiMoveCaretLeft()
: pimpl(make_unique<Impl>())
{}

EdiMoveCaretLeft::~EdiMoveCaretLeft() noexcept                    = default ;
EdiMoveCaretLeft::EdiMoveCaretLeft(EdiMoveCaretLeft&&)            = default ;
EdiMoveCaretLeft& EdiMoveCaretLeft::operator=(EdiMoveCaretLeft&&) = default ;

const string EdiMoveCaretLeft::sname() noexcept
{
    return "edi_move_caret_left" ;
}

void EdiMoveCaretLeft::sprocess(
        const bool first_call,
        const unsigned int repeat_num,
        const KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr)) const
{
    auto press = [] {
        if(ModeManager::is_edi_visual()) {
            KeybrdEventer::pushup(VKC_LSHIFT, VKC_LEFT) ;
        }
        KeybrdEventer::pushup(VKC_LEFT) ;
    } ;

    if(first_call) {
        pimpl->calcer.reset() ;
        press() ;
        return ;
    }
    if(!pimpl->calcer.is_pressed()) {
        return ;
    }
    press() ;
}


//EdiMoveCaretRight
struct EdiMoveCaretRight::Impl
{
    KeyStrokeRepeater calcer{} ;
} ;

EdiMoveCaretRight::EdiMoveCaretRight()
: pimpl(make_unique<Impl>())
{}

EdiMoveCaretRight::~EdiMoveCaretRight() noexcept                     = default ;
EdiMoveCaretRight::EdiMoveCaretRight(EdiMoveCaretRight&&)            = default ;
EdiMoveCaretRight& EdiMoveCaretRight::operator=(EdiMoveCaretRight&&) = default ;

const string EdiMoveCaretRight::sname() noexcept
{
    return "edi_move_caret_right" ;
}

void EdiMoveCaretRight::sprocess(
        const bool first_call,
        const unsigned int repeat_num,
        const KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr)) const
{
    auto press = [] {
        if(ModeManager::is_edi_visual()) {
            KeybrdEventer::pushup(VKC_LSHIFT, VKC_RIGHT) ;
        }
        else {
            KeybrdEventer::pushup(VKC_RIGHT) ;
        }
    } ;

    if(first_call) {
        pimpl->calcer.reset() ;
        press() ;
        return ;
    }

    if(!pimpl->calcer.is_pressed()) {
        return ;
    }
    press() ;
}



//EdiMoveCaretUp
struct EdiMoveCaretUp::Impl
{
    KeyStrokeRepeater calcer{} ;
} ;

EdiMoveCaretUp::EdiMoveCaretUp()
: pimpl(make_unique<Impl>())
{}

EdiMoveCaretUp::~EdiMoveCaretUp() noexcept                  = default ;
EdiMoveCaretUp::EdiMoveCaretUp(EdiMoveCaretUp&&)            = default ;
EdiMoveCaretUp& EdiMoveCaretUp::operator=(EdiMoveCaretUp&&) = default ;

const string EdiMoveCaretUp::sname() noexcept
{
    return "edi_move_caret_up" ;
}

void EdiMoveCaretUp::sprocess(
        const bool first_call,
        const unsigned int repeat_num,
        const KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr)) const
{
    auto press = [] {
        if(ModeManager::is_edi_visual()) {
            if(SimplTextSelecter::is_first_line_selection()) {
                SimplTextSelecter::select_line_EOL2BOL() ;
            }

            KeybrdEventer::pushup(VKC_LSHIFT, VKC_UP) ;
            SimplTextSelecter::moving_update() ;
        }
        else {
            KeybrdEventer::pushup(VKC_UP) ;
        }
    } ;

    if(first_call) {
        pimpl->calcer.reset() ;
        press() ;
        return ;
    }
    if(!pimpl->calcer.is_pressed()) {
        return ;
    }
    press() ;
}



//EdiMoveCaretDown
struct EdiMoveCaretDown::Impl
{
    KeyStrokeRepeater calcer{} ;
} ;

EdiMoveCaretDown::EdiMoveCaretDown()
: pimpl(make_unique<Impl>())
{}

EdiMoveCaretDown::~EdiMoveCaretDown() noexcept                    = default ;
EdiMoveCaretDown::EdiMoveCaretDown(EdiMoveCaretDown&&)            = default ;
EdiMoveCaretDown& EdiMoveCaretDown::operator=(EdiMoveCaretDown&&) = default ;

const string EdiMoveCaretDown::sname() noexcept
{
    return "edi_move_caret_down" ;
}

void EdiMoveCaretDown::sprocess(
        const bool first_call,
        const unsigned int repeat_num,
        const KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr)) const
{
    auto press = [] {
        if(ModeManager::is_edi_visual()) {
            if(SimplTextSelecter::is_first_line_selection()) {
                SimplTextSelecter::select_line_BOL2EOL() ;
            }

            KeybrdEventer::pushup(VKC_LSHIFT, VKC_DOWN) ;
            SimplTextSelecter::moving_update() ;
        }
        else {
            KeybrdEventer::pushup(VKC_DOWN) ;
        }
    } ;

    if(first_call) {
        pimpl->calcer.reset() ;
        press() ;
        return ;
    }
    if(!pimpl->calcer.is_pressed()) {
        return ;
    }
    press() ;
}


//EdiMoveCaretNwordsForward
const string EdiNMoveCaretwordsForward::sname() noexcept
{
    return "edi_n_move_caret_words_forward" ;
}

void EdiNMoveCaretwordsForward::sprocess(
        const bool first_call,
        const unsigned int repeat_num,
        const KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr))
{
    if(!first_call) return ;
    if(ModeManager::is_edi_visual()) {
        KeybrdEventer::pushup(VKC_LSHIFT, VKC_LCTRL, VKC_RIGHT) ;
    }
    else {
        KeybrdEventer::pushup(VKC_LCTRL, VKC_RIGHT) ;
    }
}


//EdiMoveCaretNwordsBackward
const string EdiNMoveCaretwordsBackward::sname() noexcept
{
    return "edi_n_move_caret_words_backward" ;
}

void EdiNMoveCaretwordsBackward::sprocess(
        const bool first_call,
        const unsigned int repeat_num,
        const KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr))
{
    if(!first_call) return ;
    if(ModeManager::is_edi_visual()) {
        KeybrdEventer::pushup(VKC_LSHIFT, VKC_LCTRL, VKC_LEFT) ;
    }
    else {
        KeybrdEventer::pushup(VKC_LCTRL, VKC_LEFT) ;
    }
}


//EdiMoveCaretNWORDSForward
const string EdiNMoveCaretWORDSForward::sname() noexcept
{
    return "edi_n_move_caret_WORDS_forward" ;
}

void EdiNMoveCaretWORDSForward::sprocess(
        const bool first_call,
        const unsigned int repeat_num,
        const KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr))
{
    if(!first_call) return ;
    if(ModeManager::is_edi_visual()) {
        KeybrdEventer::pushup(VKC_LSHIFT, VKC_LCTRL, VKC_RIGHT) ;
    }
    else {
        KeybrdEventer::pushup(VKC_LCTRL, VKC_RIGHT) ;
    }
}


//EdiMoveCaretNWORDSBackward
const string EdiNMoveCaretWORDSBackward::sname() noexcept
{
    return "edi_n_move_caret_WORDS_backward" ;
}

void EdiNMoveCaretWORDSBackward::sprocess(
        const bool first_call,
        const unsigned int repeat_num,
        const KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr))
{
    if(!first_call) return ;
    if(ModeManager::is_edi_visual()) {
        KeybrdEventer::pushup(VKC_LSHIFT, VKC_LCTRL, VKC_LEFT) ;
    }
    else {
        KeybrdEventer::pushup(VKC_LCTRL, VKC_LEFT) ;
    }
}
