#include "scroll.hpp"

#include <windows.h>
#include <iostream>

#include "mouse_eventer.hpp"
#include "interval_timer.hpp"
#include "keystroke_repeater.hpp"
#include "i_params.hpp"
#include "utility.hpp"

using namespace std ;

namespace ScrollConstant
{
    static const auto MAX_X_POS = GetSystemMetrics(SM_CXSCREEN) ;
    static const auto MAX_Y_POS = GetSystemMetrics(SM_CYSCREEN) ;

    constexpr static auto DELTA_US = 30 ;
    constexpr static auto WAIT_MS  = 30 ;
}

using namespace ScrollConstant ;

//ScrollUp
struct ScrollUp::Impl
{
    KeyStrokeRepeater ksr{WAIT_MS} ;
} ;

ScrollUp::ScrollUp()
: pimpl(make_unique<Impl>())
{}

ScrollUp::~ScrollUp() noexcept            = default ;
ScrollUp::ScrollUp(ScrollUp&&)            = default ;
ScrollUp& ScrollUp::operator=(ScrollUp&&) = default ;

const string ScrollUp::sname() noexcept
{
    return "scroll_up" ;
}

void ScrollUp::sprocess(
        const bool first_call,
        const unsigned int repeat_num,
        KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr)) const
{
    auto scroll = [first_call, repeat_num] {
        MouseEventer::vscroll(iParams::get_i("yscroll_speed") \
                * (first_call ? repeat_num : 1)) ;
    } ;

    if(first_call) {
        pimpl->ksr.reset() ;
        scroll() ;
    }
    if(pimpl->ksr.is_pressed()) {
        scroll() ;
    }
}


//ScrollDown
struct ScrollDown::Impl
{
    KeyStrokeRepeater ksr{WAIT_MS} ;
} ;

ScrollDown::ScrollDown()
: pimpl(make_unique<Impl>())
{}

ScrollDown::~ScrollDown() noexcept              = default ;
ScrollDown::ScrollDown(ScrollDown&&)            = default ;
ScrollDown& ScrollDown::operator=(ScrollDown&&) = default ;

const string ScrollDown::sname() noexcept
{
    return "scroll_down" ;
}

void ScrollDown::sprocess(
        const bool first_call,
        const unsigned int repeat_num,
        KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr)) const
{
    auto scroll = [first_call, repeat_num] {
        MouseEventer::vscroll(-iParams::get_i("yscroll_speed") \
                * (first_call ? repeat_num : 1)) ;
    } ;

    if(first_call) {
        pimpl->ksr.reset() ;
        scroll() ;
    }
    if(pimpl->ksr.is_pressed()) {
        scroll() ;
    }
}


//ScrollMidUp
struct ScrollMidUp::Impl
{
    IntervalTimer timer{DELTA_US} ;
} ;

ScrollMidUp::ScrollMidUp()
: pimpl(make_unique<Impl>())
{}

ScrollMidUp::~ScrollMidUp() noexcept               = default ;
ScrollMidUp::ScrollMidUp(ScrollMidUp&&)            = default ;
ScrollMidUp& ScrollMidUp::operator=(ScrollMidUp&&) = default ;

const string ScrollMidUp::sname() noexcept
{
    return "scroll_mid_up" ;
}

void ScrollMidUp::sprocess(
        const bool first_call,
        const unsigned int repeat_num,
        KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr)) const
{
    if(first_call) {
        pimpl->timer.reset() ;
    }
    if(!pimpl->timer.is_passed()) {
        return ;
    }
    MouseEventer::vscroll(iParams::get_f("yscroll_screen_ratio") \
            * MAX_Y_POS * 0.5f * (first_call ? repeat_num : 1)) ;
}


//ScrollMidDown
struct ScrollMidDown::Impl
{
    IntervalTimer timer{DELTA_US} ;
} ;

ScrollMidDown::ScrollMidDown()
: pimpl(make_unique<Impl>())
{}

ScrollMidDown::~ScrollMidDown() noexcept                    = default ;
ScrollMidDown::ScrollMidDown(ScrollMidDown&&)               = default ;
ScrollMidDown& ScrollMidDown::operator=(ScrollMidDown&&)    = default ;

const string ScrollMidDown::sname() noexcept
{
    return "scroll_mid_down" ;
}

void ScrollMidDown::sprocess(
        const bool first_call,
        const unsigned int repeat_num,
        KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr)) const
{
    if(first_call) {
        pimpl->timer.reset() ;
    }
    if(!pimpl->timer.is_passed()) {
        return ;
    }
    MouseEventer::vscroll(iParams::get_f("yscroll_screen_ratio") \
            * -0.5f * MAX_Y_POS * (first_call ? repeat_num: 1)) ;
}


//ScrollPageUp
struct ScrollPageUp::Impl
{
    IntervalTimer timer{DELTA_US} ;
} ;

ScrollPageUp::ScrollPageUp()
: pimpl(make_unique<Impl>())
{}

ScrollPageUp::~ScrollPageUp() noexcept                  = default ;
ScrollPageUp::ScrollPageUp(ScrollPageUp&&)              = default ;
ScrollPageUp& ScrollPageUp::operator=(ScrollPageUp&&)   = default ;

const string ScrollPageUp::sname() noexcept
{
    return "scroll_page_up" ;
}

void ScrollPageUp::sprocess(
        const bool first_call,
        const unsigned int repeat_num,
        KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr)) const
{
    if(first_call) {
        pimpl->timer.reset() ;
    }
    if(!pimpl->timer.is_passed()) {
        return ;
    }
    MouseEventer::vscroll(iParams::get_f("yscroll_screen_ratio") \
            * MAX_Y_POS * (first_call ? repeat_num : 1)) ;
}


//ScrollPageDown
struct ScrollPageDown::Impl
{
    IntervalTimer timer{DELTA_US} ;
} ;

ScrollPageDown::ScrollPageDown()
: pimpl(make_unique<Impl>())
{}

ScrollPageDown::~ScrollPageDown() noexcept                  = default ;
ScrollPageDown::ScrollPageDown(ScrollPageDown&&)            = default ;
ScrollPageDown& ScrollPageDown::operator=(ScrollPageDown&&) = default ;

const string ScrollPageDown::sname() noexcept
{
    return "scroll_page_down" ;
}

void ScrollPageDown::sprocess(
        const bool first_call,
        const unsigned int repeat_num,
        KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr)) const
{
    if(first_call) {
        pimpl->timer.reset() ;
    }
    if(!pimpl->timer.is_passed()) {
        return ;
    }
    MouseEventer::vscroll(iParams::get_f("yscroll_screen_ratio") \
            * -MAX_Y_POS * (first_call ? repeat_num : 1)) ;
}


//Horizontal
//ScrollLeft
struct ScrollLeft::Impl
{
    IntervalTimer timer{DELTA_US} ;
} ;

ScrollLeft::ScrollLeft()
: pimpl(make_unique<Impl>())
{}

ScrollLeft::~ScrollLeft() noexcept              = default ;
ScrollLeft::ScrollLeft(ScrollLeft&&)            = default ;
ScrollLeft& ScrollLeft::operator=(ScrollLeft&&) = default ;

const string ScrollLeft::sname() noexcept
{
    return "scroll_left" ;
}

void ScrollLeft::sprocess(
        const bool first_call,
        const unsigned int repeat_num,
        KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr)) const
{
    if(first_call) {
        pimpl->timer.reset() ;
    }
    if(!pimpl->timer.is_passed()) {
        return ;
    }
    MouseEventer::hscroll(-iParams::get_i("xscroll_speed") \
            * (first_call ? repeat_num : 1)) ;
}


//ScrollRight
struct ScrollRight::Impl
{
    IntervalTimer timer{DELTA_US} ;
} ;

ScrollRight::ScrollRight()
: pimpl(make_unique<Impl>())
{}

ScrollRight::~ScrollRight() noexcept                = default ;
ScrollRight::ScrollRight(ScrollRight&&)             = default ;
ScrollRight& ScrollRight::operator=(ScrollRight&&)  = default ;

const string ScrollRight::sname() noexcept
{
    return "scroll_right" ;
}

void ScrollRight::sprocess(
        const bool first_call,
        const unsigned int repeat_num,
        KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr)) const
{
    if(first_call) {
        pimpl->timer.reset() ;
    }
    if(!pimpl->timer.is_passed()) {
        return ;
    }
    MouseEventer::hscroll(iParams::get_f("xscroll_speed") \
            * (first_call ? repeat_num : 1)) ;
}


//ScrollMidLeft
struct ScrollMidLeft::Impl
{
    IntervalTimer timer{DELTA_US} ;
} ;

ScrollMidLeft::ScrollMidLeft()
: pimpl(make_unique<Impl>())
{}

ScrollMidLeft::~ScrollMidLeft() noexcept                    = default ;
ScrollMidLeft::ScrollMidLeft(ScrollMidLeft&&)               = default ;
ScrollMidLeft& ScrollMidLeft::operator=(ScrollMidLeft&&)    = default ;

const string ScrollMidLeft::sname() noexcept
{
    return "scroll_mid_left" ;
}

void ScrollMidLeft::sprocess(
        const bool first_call,
        const unsigned int repeat_num,
        KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr)) const
{
    if(first_call) {
        pimpl->timer.reset() ;
    }
    if(!pimpl->timer.is_passed()) {
        return ;
    }
    MouseEventer::hscroll(iParams::get_f("xscroll_screen_ratio") \
            * -0.5f * MAX_X_POS * (first_call ? repeat_num : 1)) ;
}


//ScrollMidRight
struct ScrollMidRight::Impl
{
    IntervalTimer timer{DELTA_US} ;
} ;

ScrollMidRight::ScrollMidRight()
: pimpl(make_unique<Impl>())
{}

ScrollMidRight::~ScrollMidRight() noexcept                  = default ;
ScrollMidRight::ScrollMidRight(ScrollMidRight&&)            = default ;
ScrollMidRight& ScrollMidRight::operator=(ScrollMidRight&&) = default ;

const string ScrollMidRight::sname() noexcept
{
    return "scroll_mid_right" ;
}

void ScrollMidRight::sprocess(
        const bool first_call,
        const unsigned int repeat_num,
        KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr)) const
{
    if(first_call) {
        pimpl->timer.reset() ;
    }
    if(!pimpl->timer.is_passed()) {
        return ;
    }
    MouseEventer::hscroll(iParams::get_f("xscroll_screen_ratio") \
            * MAX_X_POS * 0.5f * (first_call ? repeat_num : 1)) ;
}
