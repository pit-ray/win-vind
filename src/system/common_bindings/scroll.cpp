#include "scroll.hpp"

#include <windows.h>
#include <iostream>

#include "mouse_eventer.hpp"
#include "interval_timer.hpp"
#include "i_params.hpp"
#include "utility.hpp"

using namespace std ;

namespace ScrollConstant
{
    static const auto MAX_X_POS = GetSystemMetrics(SM_CXSCREEN) ;
    static const auto MAX_Y_POS = GetSystemMetrics(SM_CYSCREEN) ;

    constexpr static auto DELTA_US = 30 ;
}

using namespace ScrollConstant ;

//Vertical
//ScrollUp
struct ScrollUp::Impl
{
    IntervalTimer timer{30} ;
} ;

ScrollUp::ScrollUp()
: pimpl(make_unique<Impl>())
{}

ScrollUp::~ScrollUp() noexcept = default ;

ScrollUp::ScrollUp(ScrollUp&&) = default ;
ScrollUp& ScrollUp::operator=(ScrollUp&&) = default ;

const string ScrollUp::sname() noexcept
{
    return "scroll_up" ;
}

bool ScrollUp::sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const UNUSED(parent_logger)) const
{
    if(repeat_num == 1) {
        if(first_call) {
            pimpl->timer.reset() ;
        }
        if(!pimpl->timer.is_passed()) {
            return true ;
        }
        return MouseEventer::vscroll(iParams::get_i("yscroll_speed")) ;
    }

    if(!first_call) return true ;
    //repeat_num > 1
    for(unsigned int i = 0 ; i < repeat_num ; i ++) {
        if(!MouseEventer::vscroll(iParams::get_i("yscroll_speed"))) {
            return false ;
        }
    }
    return true ;
}


//ScrollDown
struct ScrollDown::Impl
{
    IntervalTimer timer{DELTA_US} ;
} ;

ScrollDown::ScrollDown()
: pimpl(make_unique<Impl>())
{}

ScrollDown::~ScrollDown() noexcept = default ;

ScrollDown::ScrollDown(ScrollDown&&) = default ;
ScrollDown& ScrollDown::operator=(ScrollDown&&) = default ;

const string ScrollDown::sname() noexcept
{
    return "scroll_down" ;
}

bool ScrollDown::sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const UNUSED(parent_logger)) const
{
    if(repeat_num == 1) {
        if(first_call) {
            pimpl->timer.reset() ;
        }

        if(!pimpl->timer.is_passed()) {
            return true ;
        }

        return MouseEventer::vscroll(-iParams::get_i("yscroll_speed")) ;
    }

    //repeat_num > 1
    if(!first_call) return true ;
    for(unsigned int i = 0 ; i < repeat_num ; i ++) {
       if(!MouseEventer::vscroll(-iParams::get_i("yscroll_speed"))) {
           return false ;
       }
    }
    return true ;
}


//ScrollMidUp
struct ScrollMidUp::Impl
{
    IntervalTimer timer{DELTA_US} ;
} ;

ScrollMidUp::ScrollMidUp()
: pimpl(make_unique<Impl>())
{}

ScrollMidUp::~ScrollMidUp() noexcept = default ;

ScrollMidUp::ScrollMidUp(ScrollMidUp&&) = default ;
ScrollMidUp& ScrollMidUp::operator=(ScrollMidUp&&) = default ;

const string ScrollMidUp::sname() noexcept
{
    return "scroll_mid_up" ;
}

bool ScrollMidUp::sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const UNUSED(parent_logger)) const
{
    static const auto delta = MAX_Y_POS  * 0.5f ;

    if(repeat_num == 1) {
        if(first_call) {
            pimpl->timer.reset() ;
        }

        if(!pimpl->timer.is_passed()) {
            return true ;
        }
        return MouseEventer::vscroll(delta * iParams::get_f("yscroll_screen_ratio")) ;
    }

    if(!first_call) return true ;
    for(unsigned int i = 0 ; i < repeat_num ; i ++) {
        if(!MouseEventer::vscroll(delta * iParams::get_f("yscroll_screen_ratio"))) {
            return false ;
        }
    }
    return true ;
}


//ScrollMidDown
struct ScrollMidDown::Impl
{
    IntervalTimer timer{DELTA_US} ;
} ;

ScrollMidDown::ScrollMidDown()
: pimpl(make_unique<Impl>())
{}

ScrollMidDown::~ScrollMidDown() noexcept = default ;

ScrollMidDown::ScrollMidDown(ScrollMidDown&&) = default ;
ScrollMidDown& ScrollMidDown::operator=(ScrollMidDown&&) = default ;

const string ScrollMidDown::sname() noexcept
{
    return "scroll_mid_down" ;
}

bool ScrollMidDown::sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const UNUSED(parent_logger)) const
{
    static const auto delta = MAX_Y_POS * 0.5f ;
    if(repeat_num == 1) {
        if(first_call) {
            pimpl->timer.reset() ;
        }
        if(!pimpl->timer.is_passed()) {
            return true ;
        }
        return MouseEventer::vscroll(-delta * iParams::get_f("yscroll_screen_ratio")) ;
    }

    //repeat_num > 1
    if(!first_call) return true ;
    for(unsigned int i = 0 ; i < repeat_num ; i ++) {
        if(!MouseEventer::vscroll(-delta * iParams::get_f("yscroll_screen_ratio"))) {
            return false ;
        }
    }
    return true ;
}


//ScrollPageUp
struct ScrollPageUp::Impl
{
    IntervalTimer timer{DELTA_US} ;
} ;

ScrollPageUp::ScrollPageUp()
: pimpl(make_unique<Impl>())
{}

ScrollPageUp::~ScrollPageUp() noexcept = default ;

ScrollPageUp::ScrollPageUp(ScrollPageUp&&) = default ;
ScrollPageUp& ScrollPageUp::operator=(ScrollPageUp&&) = default ;

const string ScrollPageUp::sname() noexcept
{
    return "scroll_page_up" ;
}

bool ScrollPageUp::sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const UNUSED(parent_logger)) const
{
    if(repeat_num == 1) {
        if(first_call) {
            pimpl->timer.reset() ;
        }
        if(!pimpl->timer.is_passed()) {
            return true ;
        }
        return MouseEventer::vscroll(MAX_Y_POS * iParams::get_f("yscroll_screen_ratio")) ;
    }

    if(!first_call) return true ;
    for(unsigned int i = 0 ; i < repeat_num ; i ++) {
        if(!MouseEventer::vscroll(MAX_Y_POS * iParams::get_f("yscroll_screen_ratio"))) {
            return false ;
        }
    }
    return true ;
}


//ScrollPageDown
struct ScrollPageDown::Impl
{
    IntervalTimer timer{DELTA_US} ;
} ;

ScrollPageDown::ScrollPageDown()
: pimpl(make_unique<Impl>())
{}

ScrollPageDown::~ScrollPageDown() noexcept = default ;

ScrollPageDown::ScrollPageDown(ScrollPageDown&&) = default ;
ScrollPageDown& ScrollPageDown::operator=(ScrollPageDown&&) = default ;

const string ScrollPageDown::sname() noexcept
{
    return "scroll_page_down" ;
}

bool ScrollPageDown::sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger) const
{
    if(repeat_num == 1) {
        if(first_call) {
            pimpl->timer.reset() ;
        }
        if(!pimpl->timer.is_passed()) {
            return true ;
        }
        return MouseEventer::vscroll(-MAX_Y_POS * iParams::get_f("yscroll_screen_ratio")) ;
    }

    //repeat_num > 1
    if(!first_call) return true ;
    for(unsigned int i = 0 ; i < repeat_num ; i ++) {
        if(!MouseEventer::vscroll(-MAX_Y_POS * iParams::get_f("yscroll_screen_ratio"))) {
            return false ;
        }
    }
    return true ;
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

ScrollLeft::~ScrollLeft() noexcept = default ;

ScrollLeft::ScrollLeft(ScrollLeft&&) = default ;
ScrollLeft& ScrollLeft::operator=(ScrollLeft&&) = default ;

const string ScrollLeft::sname() noexcept
{
    return "scroll_left" ;
}

bool ScrollLeft::sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const UNUSED(parent_logger)) const
{
    if(repeat_num == 1) {
        if(first_call) {
            pimpl->timer.reset() ;
        }
        if(!pimpl->timer.is_passed()) {
            return true ;
        }
        return MouseEventer::hscroll(-iParams::get_i("xscroll_speed")) ;
    }
    
    //repeat_num > 1
    if(!first_call) return true ;
    for(unsigned int i = 0 ; i < repeat_num ; i ++) {
        if(!MouseEventer::hscroll(-iParams::get_i("xscroll_speed"))) {
            return false ;
        }
    }
    return true ;
}


//ScrollRight
struct ScrollRight::Impl
{
    IntervalTimer timer{DELTA_US} ;
} ;

ScrollRight::ScrollRight()
: pimpl(make_unique<Impl>())
{}

ScrollRight::~ScrollRight() noexcept = default ;

ScrollRight::ScrollRight(ScrollRight&&) = default ;
ScrollRight& ScrollRight::operator=(ScrollRight&&) = default ;

const string ScrollRight::sname() noexcept
{
    return "scroll_right" ;
}

bool ScrollRight::sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const UNUSED(parent_logger)) const
{
    if(repeat_num == 1) {
        if(first_call) {
            pimpl->timer.reset() ;
        }
        if(!pimpl->timer.is_passed()) {
            return true ;
        }
        return MouseEventer::hscroll(iParams::get_f("xscroll_speed")) ;
    }

    //repeat_num > 1
    if(!first_call) return true ;
    for(unsigned int i = 0 ; i < repeat_num ; i ++) {
        if(!MouseEventer::hscroll(iParams::get_f("xscroll_speed"))) {
            return false ;
        }
    }
    return true ;
}


//ScrollMidLeft
struct ScrollMidLeft::Impl
{
    IntervalTimer timer{DELTA_US} ;
} ;

ScrollMidLeft::ScrollMidLeft()
: pimpl(make_unique<Impl>())
{}

ScrollMidLeft::~ScrollMidLeft() noexcept = default ;

ScrollMidLeft::ScrollMidLeft(ScrollMidLeft&&) = default ;
ScrollMidLeft& ScrollMidLeft::operator=(ScrollMidLeft&&) = default ;

const string ScrollMidLeft::sname() noexcept
{
    return "scroll_mid_left" ;
}

bool ScrollMidLeft::sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const UNUSED(parent_logger)) const
{
    static const auto delta = MAX_X_POS * 0.5f ;
    if(!repeat_num) {
        if(first_call) {
            pimpl->timer.reset() ;
        }
        if(!pimpl->timer.is_passed()) {
            return true ;
        }
        return MouseEventer::hscroll(-delta * iParams::get_f("xscroll_screen_ratio")) ;
    }

    //repeat_num > 1
    if(!first_call) return true ;
    for(unsigned int i = 0 ; i < repeat_num ; i ++) {
        if(!MouseEventer::hscroll(-delta * iParams::get_f("xscroll_screen_ratio"))) {
            return false ;
        }
    }
    return true ;
}


//ScrollMidRight
struct ScrollMidRight::Impl
{
    IntervalTimer timer{DELTA_US} ;
} ;

ScrollMidRight::ScrollMidRight()
: pimpl(make_unique<Impl>())
{}

ScrollMidRight::~ScrollMidRight() noexcept = default ;

ScrollMidRight::ScrollMidRight(ScrollMidRight&&) = default ;
ScrollMidRight& ScrollMidRight::operator=(ScrollMidRight&&) = default ;

const string ScrollMidRight::sname() noexcept
{
    return "scroll_mid_right" ;
}

bool ScrollMidRight::sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const UNUSED(parent_logger)) const
{
    static const auto delta = MAX_X_POS * 0.5f ;
    if(repeat_num == 1) {
        if(first_call) {
            pimpl->timer.reset() ;
        }
        if(!pimpl->timer.is_passed()) {
            return true ;
        }
        return MouseEventer::hscroll(delta * iParams::get_f("xscroll_screen_ratio")) ;
    }

    //repeat_num > 1
    if(!first_call) return true ;
    for(unsigned int i = 0 ; i < repeat_num ; i ++) {
        if(!MouseEventer::hscroll(delta * iParams::get_f("xscroll_screen_ratio"))) {
            return false ;
        }
    }
    return true ;
}
