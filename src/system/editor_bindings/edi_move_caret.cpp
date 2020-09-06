#include "edi_move_caret.hpp"

#include "keybrd_eventer.hpp"
#include "interval_timer.hpp"
#include "msg_logger.hpp"
#include "key_absorber.hpp"
#include "simpl_text_selecter.hpp"
#include "mode_manager.hpp"
#include "keystroke_repeater.hpp"

using namespace std ;

//EdiMoveCaretLeft
struct EdiMoveCaretLeft::Impl
{
    KeyStrokeRepeater calcer{} ;
} ;

EdiMoveCaretLeft::EdiMoveCaretLeft()
: pimpl(make_unique<Impl>())
{}

EdiMoveCaretLeft::~EdiMoveCaretLeft() noexcept = default ;

EdiMoveCaretLeft::EdiMoveCaretLeft(EdiMoveCaretLeft&&) noexcept = default ;
EdiMoveCaretLeft& EdiMoveCaretLeft::operator=(EdiMoveCaretLeft&&) noexcept = default ;

const string EdiMoveCaretLeft::sname() noexcept
{
    return "edi_move_caret_left" ;
}

bool EdiMoveCaretLeft::sprocess(const bool first_call) const
{
    auto press = [] {
        if(ModeManager::is_edi_visual()) {
            return KeybrdEventer::pushup(VKC_LSHIFT, VKC_LEFT) ;
        }
        return KeybrdEventer::pushup(VKC_LEFT) ;
    } ;

    if(first_call) {
        pimpl->calcer.reset() ;
        return press() ;
    }

    if(!pimpl->calcer.is_pressed()) {
        return true ;
    }

    return press() ;
}


//EdiMoveCaretRight
struct EdiMoveCaretRight::Impl
{
    KeyStrokeRepeater calcer{} ;
} ;

EdiMoveCaretRight::EdiMoveCaretRight()
: pimpl(make_unique<Impl>())
{}

EdiMoveCaretRight::~EdiMoveCaretRight() noexcept = default ;

EdiMoveCaretRight::EdiMoveCaretRight(EdiMoveCaretRight&&) noexcept = default ;
EdiMoveCaretRight& EdiMoveCaretRight::operator=(EdiMoveCaretRight&&) noexcept = default ;

const string EdiMoveCaretRight::sname() noexcept
{
    return "edi_move_caret_right" ;
}

bool EdiMoveCaretRight::sprocess(const bool first_call) const
{
    auto press = [] {
        if(ModeManager::is_edi_visual()) {
            return KeybrdEventer::pushup(VKC_LSHIFT, VKC_RIGHT) ;
        }
        return KeybrdEventer::pushup(VKC_RIGHT) ;
    } ;

    if(first_call) {
        pimpl->calcer.reset() ;
        return press() ;
    }

    if(!pimpl->calcer.is_pressed()) {
        return true ;
    }
    return press() ;
}



//EdiMoveCaretUp
struct EdiMoveCaretUp::Impl
{
    KeyStrokeRepeater calcer{} ;
} ;

EdiMoveCaretUp::EdiMoveCaretUp()
: pimpl(make_unique<Impl>())
{}

EdiMoveCaretUp::~EdiMoveCaretUp() noexcept = default ;

EdiMoveCaretUp::EdiMoveCaretUp(EdiMoveCaretUp&&) noexcept = default ;
EdiMoveCaretUp& EdiMoveCaretUp::operator=(EdiMoveCaretUp&&) noexcept = default ;

const string EdiMoveCaretUp::sname() noexcept
{
    return "edi_move_caret_up" ;
}

bool EdiMoveCaretUp::sprocess(const bool first_call) const
{
    auto press = [] {
        if(ModeManager::is_edi_visual()) {
            if(SimplTextSelecter::is_first_line_selection()) {
                if(!SimplTextSelecter::select_line_EOL2BOL()) {
                    return false ;
                }
            }

            if(!KeybrdEventer::pushup(VKC_LSHIFT, VKC_UP)) {
                return false ;
            }
            return SimplTextSelecter::moving_update() ;
        }
        return KeybrdEventer::pushup(VKC_UP) ;
    } ;

    if(first_call) {
        pimpl->calcer.reset() ;
        return press() ;
    }
    if(!pimpl->calcer.is_pressed()) {
        return true ;
    }
    return press() ;
}



//EdiMoveCaretDown
struct EdiMoveCaretDown::Impl
{
    KeyStrokeRepeater calcer{} ;
} ;

EdiMoveCaretDown::EdiMoveCaretDown()
: pimpl(make_unique<Impl>())
{}

EdiMoveCaretDown::~EdiMoveCaretDown() noexcept = default ;

EdiMoveCaretDown::EdiMoveCaretDown(EdiMoveCaretDown&&) noexcept = default ;
EdiMoveCaretDown& EdiMoveCaretDown::operator=(EdiMoveCaretDown&&) noexcept = default ;

const string EdiMoveCaretDown::sname() noexcept
{
    return "edi_move_caret_down" ;
}

bool EdiMoveCaretDown::sprocess(const bool first_call) const
{
    auto press = [] {
        if(ModeManager::is_edi_visual()) {
            if(SimplTextSelecter::is_first_line_selection()) {
                if(!SimplTextSelecter::select_line_BOL2EOL()) {
                    return false ;
                }
            }

            if(!KeybrdEventer::pushup(VKC_LSHIFT, VKC_DOWN)) {
                return false ;
            }
            return SimplTextSelecter::moving_update() ;
        }
        return KeybrdEventer::pushup(VKC_DOWN) ;
    } ;

    if(first_call) {
        pimpl->calcer.reset() ;
        return press() ;
    }
    if(!pimpl->calcer.is_pressed()) {
        return true ;
    }
    return press() ;
}


//EdiMoveCaretNwordsForward
const string EdiNMoveCaretwordsForward::sname() noexcept
{
    return "edi_n_move_caret_words_forward" ;
}

bool EdiNMoveCaretwordsForward::sprocess(const bool first_call)
{
    if(!first_call) return true ;
    if(ModeManager::is_edi_visual()) {
        return KeybrdEventer::pushup(VKC_LSHIFT, VKC_LCTRL, VKC_RIGHT) ;
    }
    return KeybrdEventer::pushup(VKC_LCTRL, VKC_RIGHT) ;
}


//EdiMoveCaretNwordsBackward
const string EdiNMoveCaretwordsBackward::sname() noexcept
{
    return "edi_n_move_caret_words_backward" ;
}

bool EdiNMoveCaretwordsBackward::sprocess(const bool first_call)
{
    if(!first_call) return true ;
    if(ModeManager::is_edi_visual()) {
        return KeybrdEventer::pushup(VKC_LSHIFT, VKC_LCTRL, VKC_LEFT) ;
    }
    return KeybrdEventer::pushup(VKC_LCTRL, VKC_LEFT) ;
}


//EdiMoveCaretNWORDSForward
const string EdiNMoveCaretWORDSForward::sname() noexcept
{
    return "edi_n_move_caret_WORDS_forward" ;
}

bool EdiNMoveCaretWORDSForward::sprocess(const bool first_call)
{
    if(!first_call) return true ;
    if(ModeManager::is_edi_visual()) {
        return KeybrdEventer::pushup(VKC_LSHIFT, VKC_LCTRL, VKC_RIGHT) ;
    }
    return KeybrdEventer::pushup(VKC_LCTRL, VKC_RIGHT) ;
}


//EdiMoveCaretNWORDSBackward
const string EdiNMoveCaretWORDSBackward::sname() noexcept
{
    return "edi_n_move_caret_WORDS_backward" ;
}

bool EdiNMoveCaretWORDSBackward::sprocess(const bool first_call)
{
    if(!first_call) return true ;
    if(ModeManager::is_edi_visual()) {
        return KeybrdEventer::pushup(VKC_LSHIFT, VKC_LCTRL, VKC_LEFT) ;
    }
    return KeybrdEventer::pushup(VKC_LCTRL, VKC_LEFT) ;
}