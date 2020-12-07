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

template <typename T1, typename T2>
inline static void _common_process(
        const bool first_call,
        const unsigned int repeat_num,
        KeyStrokeRepeater& ksr,
        T1&& v_press_proc,
        T2&& n_press_proc)
{
    using ModeManager::is_edi_visual ;

    if(repeat_num == 1) {
        if(first_call) {
            ksr.reset() ;

            if(is_edi_visual()) v_press_proc() ;
            else n_press_proc() ;

            return ;
        }
        if(!ksr.is_pressed()) {
            return ;
        }

        if(is_edi_visual()) v_press_proc() ;
        else n_press_proc() ;
    }

    if(!first_call) return ;
    if(is_edi_visual()) {
        for(unsigned int i = 0 ; i < repeat_num ; i ++)
            v_press_proc() ;
    }
    else {
        for(unsigned int i = 0 ; i < repeat_num ; i ++)
            n_press_proc() ;
    }
}

//EdiMoveCaretLeft
struct EdiMoveCaretLeft::Impl
{
    KeyStrokeRepeater ksr{} ;
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
bool EdiMoveCaretLeft::is_for_moving_caret() const noexcept
{
    return true ;
}
void EdiMoveCaretLeft::sprocess(
        const bool first_call,
        const unsigned int repeat_num,
        KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr)) const
{
    using KeybrdEventer::pushup ;
    _common_process(first_call, repeat_num, pimpl->ksr,
            [] {pushup(VKC_LSHIFT, VKC_LEFT) ;},
            [] {pushup(VKC_LEFT) ;}) ;
}


//EdiMoveCaretRight
struct EdiMoveCaretRight::Impl
{
    KeyStrokeRepeater ksr{} ;
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
bool EdiMoveCaretRight::is_for_moving_caret() const noexcept
{
    return true ;
}
void EdiMoveCaretRight::sprocess(
        const bool first_call,
        const unsigned int repeat_num,
        KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr)) const
{
    using KeybrdEventer::pushup ;
    _common_process(first_call, repeat_num, pimpl->ksr,
            [] {pushup(VKC_LSHIFT, VKC_RIGHT) ;},
            [] {pushup(VKC_RIGHT) ;}) ;
}



//EdiMoveCaretUp
struct EdiMoveCaretUp::Impl
{
    KeyStrokeRepeater ksr{} ;
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
bool EdiMoveCaretUp::is_for_moving_caret() const noexcept
{
    return true ;
}
void EdiMoveCaretUp::sprocess(
        const bool first_call,
        const unsigned int repeat_num,
        KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr)) const
{
    auto v_press = [] {
        if(SimplTextSelecter::is_first_line_selection())
            SimplTextSelecter::select_line_EOL2BOL() ;

        KeybrdEventer::pushup(VKC_LSHIFT, VKC_UP) ;
        //SimplTextSelecter::moving_update() ;
    } ;
    auto n_press = [] {KeybrdEventer::pushup(VKC_UP) ;} ;

    _common_process(first_call, repeat_num, pimpl->ksr, v_press, n_press) ;
}



//EdiMoveCaretDown
struct EdiMoveCaretDown::Impl
{
    KeyStrokeRepeater ksr{} ;
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
bool EdiMoveCaretDown::is_for_moving_caret() const noexcept
{
    return true ;
}
void EdiMoveCaretDown::sprocess(
        const bool first_call,
        const unsigned int repeat_num,
        KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr)) const
{
    auto v_press = [] {
        if(SimplTextSelecter::is_first_line_selection())
            SimplTextSelecter::select_line_BOL2EOL() ;

        KeybrdEventer::pushup(VKC_LSHIFT, VKC_DOWN) ;

        //If call EdiMoveCaretDown after EdiMoveCaretUp,
        //inner variables of moving_update() are dedicated to EOL2BOL.
        //so we cannot move caret down.
        //SimplTextSelecter::moving_update() ;
    } ;
    auto n_press = [] {KeybrdEventer::pushup(VKC_DOWN) ;} ;

    _common_process(first_call, repeat_num, pimpl->ksr, v_press, n_press) ;
}


//EdiMoveCaretNwordsForward
struct EdiNMoveCaretwordsForward::Impl
{
    KeyStrokeRepeater ksr{} ;
} ;

EdiNMoveCaretwordsForward::EdiNMoveCaretwordsForward()
: pimpl(make_unique<Impl>())
{}

EdiNMoveCaretwordsForward::~EdiNMoveCaretwordsForward() noexcept                             = default ;
EdiNMoveCaretwordsForward::EdiNMoveCaretwordsForward(EdiNMoveCaretwordsForward&&)            = default ;
EdiNMoveCaretwordsForward& EdiNMoveCaretwordsForward::operator=(EdiNMoveCaretwordsForward&&) = default ;

const string EdiNMoveCaretwordsForward::sname() noexcept
{
    return "edi_n_move_caret_words_forward" ;
}
bool EdiNMoveCaretwordsForward::is_for_moving_caret() const noexcept
{
    return true ;
}
void EdiNMoveCaretwordsForward::sprocess(
        const bool first_call,
        const unsigned int repeat_num,
        KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr)) const
{
    using KeybrdEventer::pushup ;
    _common_process(first_call, repeat_num, pimpl->ksr,
        [] {pushup(VKC_LSHIFT, VKC_LCTRL, VKC_RIGHT) ;},
        [] {pushup(VKC_LCTRL, VKC_RIGHT) ;}) ;
}


//EdiMoveCaretNwordsBackward
struct EdiNMoveCaretwordsBackward::Impl
{
    KeyStrokeRepeater ksr{} ;
} ;

EdiNMoveCaretwordsBackward::EdiNMoveCaretwordsBackward()
: pimpl(make_unique<Impl>())
{}

EdiNMoveCaretwordsBackward::~EdiNMoveCaretwordsBackward() noexcept                              = default ;
EdiNMoveCaretwordsBackward::EdiNMoveCaretwordsBackward(EdiNMoveCaretwordsBackward&&)            = default ;
EdiNMoveCaretwordsBackward& EdiNMoveCaretwordsBackward::operator=(EdiNMoveCaretwordsBackward&&) = default ;

const string EdiNMoveCaretwordsBackward::sname() noexcept
{
    return "edi_n_move_caret_words_backward" ;
}
bool EdiNMoveCaretwordsBackward::is_for_moving_caret() const noexcept
{
    return true ;
}
void EdiNMoveCaretwordsBackward::sprocess(
        const bool first_call,
        const unsigned int repeat_num,
        KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr)) const
{
    using KeybrdEventer::pushup ;
    _common_process(first_call, repeat_num, pimpl->ksr,
            [] {pushup(VKC_LSHIFT, VKC_LCTRL, VKC_LEFT) ;},
            [] {pushup(VKC_LCTRL, VKC_LEFT) ;}) ;
}


//EdiMoveCaretNWORDSForward
struct EdiNMoveCaretWORDSForward::Impl
{
    KeyStrokeRepeater ksr{} ;
} ;

EdiNMoveCaretWORDSForward::EdiNMoveCaretWORDSForward()
: pimpl(make_unique<Impl>())
{}

EdiNMoveCaretWORDSForward::~EdiNMoveCaretWORDSForward() noexcept                             = default ;
EdiNMoveCaretWORDSForward::EdiNMoveCaretWORDSForward(EdiNMoveCaretWORDSForward&&)            = default ;
EdiNMoveCaretWORDSForward& EdiNMoveCaretWORDSForward::operator=(EdiNMoveCaretWORDSForward&&) = default ;

const string EdiNMoveCaretWORDSForward::sname() noexcept
{
    return "edi_n_move_caret_WORDS_forward" ;
}
bool EdiNMoveCaretWORDSForward::is_for_moving_caret() const noexcept
{
    return true ;
}
void EdiNMoveCaretWORDSForward::sprocess(
        const bool first_call,
        const unsigned int repeat_num,
        KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr)) const
{
    using KeybrdEventer::pushup ;
    _common_process(first_call, repeat_num, pimpl->ksr,
            [] {pushup(VKC_LSHIFT, VKC_LCTRL, VKC_RIGHT) ;},
            [] {pushup(VKC_LCTRL, VKC_RIGHT) ;}) ;
}


//EdiMoveCaretNWORDSBackward
struct EdiNMoveCaretWORDSBackward::Impl
{
    KeyStrokeRepeater ksr{} ;
} ;

EdiNMoveCaretWORDSBackward::EdiNMoveCaretWORDSBackward()
: pimpl(make_unique<Impl>())
{}

EdiNMoveCaretWORDSBackward::~EdiNMoveCaretWORDSBackward() noexcept                              = default ;
EdiNMoveCaretWORDSBackward::EdiNMoveCaretWORDSBackward(EdiNMoveCaretWORDSBackward&&)            = default ;
EdiNMoveCaretWORDSBackward& EdiNMoveCaretWORDSBackward::operator=(EdiNMoveCaretWORDSBackward&&) = default ;

const string EdiNMoveCaretWORDSBackward::sname() noexcept
{
    return "edi_n_move_caret_WORDS_backward" ;
}
bool EdiNMoveCaretWORDSBackward::is_for_moving_caret() const noexcept
{
    return true ;
}
void EdiNMoveCaretWORDSBackward::sprocess(
        const bool first_call,
        const unsigned int repeat_num,
        KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr)) const
{
    using KeybrdEventer::pushup ;
    _common_process(first_call, repeat_num, pimpl->ksr,
            [] {pushup(VKC_LSHIFT, VKC_LCTRL, VKC_LEFT) ;},
            [] {pushup(VKC_LCTRL, VKC_LEFT) ;}) ;
}
