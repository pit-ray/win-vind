#include "scroll.hpp"

#include <windows.h>
#include <iostream>

#include "i_params.hpp"
#include "io/mouse.hpp"
#include "time/interval_timer.hpp"
#include "time/keystroke_repeater.hpp"
#include "util/def.hpp"

namespace
{
    const auto MAX_X_POS = GetSystemMetrics(SM_CXSCREEN) ;
    const auto MAX_Y_POS = GetSystemMetrics(SM_CYSCREEN) ;

    constexpr auto DELTA_US = 30 ;
    constexpr auto WAIT_MS  = 30 ;
}


namespace vind
{
    //ScrollUp
    struct ScrollUp::Impl {
        KeyStrokeRepeater ksr{WAIT_MS} ;
    } ;

    ScrollUp::ScrollUp()
    : pimpl(std::make_unique<Impl>())
    {}

    ScrollUp::~ScrollUp() noexcept            = default ;
    ScrollUp::ScrollUp(ScrollUp&&)            = default ;
    ScrollUp& ScrollUp::operator=(ScrollUp&&) = default ;

    const std::string ScrollUp::sname() noexcept {
        return "scroll_up" ;
    }

    void ScrollUp::sprocess(
            bool first_call,
            unsigned int repeat_num,
            KeycodeLogger* const UNUSED(parent_vkclgr),
            const CharLogger* const UNUSED(parent_charlgr)) const {
        auto scroll = [first_call, repeat_num] {
            mouse::vscroll(iparams::get_i("yscroll_speed") \
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
    struct ScrollDown::Impl {
        KeyStrokeRepeater ksr{WAIT_MS} ;
    } ;

    ScrollDown::ScrollDown()
    : pimpl(std::make_unique<Impl>())
    {}

    ScrollDown::~ScrollDown() noexcept              = default ;
    ScrollDown::ScrollDown(ScrollDown&&)            = default ;
    ScrollDown& ScrollDown::operator=(ScrollDown&&) = default ;

    const std::string ScrollDown::sname() noexcept {
        return "scroll_down" ;
    }

    void ScrollDown::sprocess(
            bool first_call,
            unsigned int repeat_num,
            KeycodeLogger* const UNUSED(parent_vkclgr),
            const CharLogger* const UNUSED(parent_charlgr)) const {
        auto scroll = [first_call, repeat_num] {
            mouse::vscroll(-iparams::get_i("yscroll_speed") \
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
    struct ScrollMidUp::Impl {
        IntervalTimer timer{DELTA_US} ;
    } ;

    ScrollMidUp::ScrollMidUp()
    : pimpl(std::make_unique<Impl>())
    {}

    ScrollMidUp::~ScrollMidUp() noexcept               = default ;
    ScrollMidUp::ScrollMidUp(ScrollMidUp&&)            = default ;
    ScrollMidUp& ScrollMidUp::operator=(ScrollMidUp&&) = default ;

    const std::string ScrollMidUp::sname() noexcept {
        return "scroll_mid_up" ;
    }

    void ScrollMidUp::sprocess(
            bool first_call,
            unsigned int repeat_num,
            KeycodeLogger* const UNUSED(parent_vkclgr),
            const CharLogger* const UNUSED(parent_charlgr)) const {
        if(first_call) {
            pimpl->timer.reset() ;
        }
        if(!pimpl->timer.is_passed()) {
            return ;
        }
        mouse::vscroll(iparams::get_f("yscroll_screen_ratio") \
                * MAX_Y_POS * 0.5f * (first_call ? repeat_num : 1)) ;
    }


    //ScrollMidDown
    struct ScrollMidDown::Impl {
        IntervalTimer timer{DELTA_US} ;
    } ;

    ScrollMidDown::ScrollMidDown()
    : pimpl(std::make_unique<Impl>())
    {}

    ScrollMidDown::~ScrollMidDown() noexcept                    = default ;
    ScrollMidDown::ScrollMidDown(ScrollMidDown&&)               = default ;
    ScrollMidDown& ScrollMidDown::operator=(ScrollMidDown&&)    = default ;

    const std::string ScrollMidDown::sname() noexcept {
        return "scroll_mid_down" ;
    }

    void ScrollMidDown::sprocess(
            bool first_call,
            unsigned int repeat_num,
            KeycodeLogger* const UNUSED(parent_vkclgr),
            const CharLogger* const UNUSED(parent_charlgr)) const {
        if(first_call) {
            pimpl->timer.reset() ;
        }
        if(!pimpl->timer.is_passed()) {
            return ;
        }
        mouse::vscroll(iparams::get_f("yscroll_screen_ratio") \
                * -0.5f * MAX_Y_POS * (first_call ? repeat_num: 1)) ;
    }


    //ScrollPageUp
    struct ScrollPageUp::Impl {
        IntervalTimer timer{DELTA_US} ;
    } ;

    ScrollPageUp::ScrollPageUp()
    : pimpl(std::make_unique<Impl>())
    {}

    ScrollPageUp::~ScrollPageUp() noexcept                  = default ;
    ScrollPageUp::ScrollPageUp(ScrollPageUp&&)              = default ;
    ScrollPageUp& ScrollPageUp::operator=(ScrollPageUp&&)   = default ;

    const std::string ScrollPageUp::sname() noexcept {
        return "scroll_page_up" ;
    }

    void ScrollPageUp::sprocess(
            bool first_call,
            unsigned int repeat_num,
            KeycodeLogger* const UNUSED(parent_vkclgr),
            const CharLogger* const UNUSED(parent_charlgr)) const {
        if(first_call) {
            pimpl->timer.reset() ;
        }
        if(!pimpl->timer.is_passed()) {
            return ;
        }
        mouse::vscroll(iparams::get_f("yscroll_screen_ratio") \
                * MAX_Y_POS * (first_call ? repeat_num : 1)) ;
    }


    //ScrollPageDown
    struct ScrollPageDown::Impl {
        IntervalTimer timer{DELTA_US} ;
    } ;

    ScrollPageDown::ScrollPageDown()
    : pimpl(std::make_unique<Impl>())
    {}

    ScrollPageDown::~ScrollPageDown() noexcept                  = default ;
    ScrollPageDown::ScrollPageDown(ScrollPageDown&&)            = default ;
    ScrollPageDown& ScrollPageDown::operator=(ScrollPageDown&&) = default ;

    const std::string ScrollPageDown::sname() noexcept {
        return "scroll_page_down" ;
    }

    void ScrollPageDown::sprocess(
            bool first_call,
            unsigned int repeat_num,
            KeycodeLogger* const UNUSED(parent_vkclgr),
            const CharLogger* const UNUSED(parent_charlgr)) const {
        if(first_call) {
            pimpl->timer.reset() ;
        }
        if(!pimpl->timer.is_passed()) {
            return ;
        }
        mouse::vscroll(iparams::get_f("yscroll_screen_ratio") \
                * -MAX_Y_POS * (first_call ? repeat_num : 1)) ;
    }


    //Horizontal
    //ScrollLeft
    struct ScrollLeft::Impl {
        IntervalTimer timer{DELTA_US} ;
    } ;

    ScrollLeft::ScrollLeft()
    : pimpl(std::make_unique<Impl>())
    {}

    ScrollLeft::~ScrollLeft() noexcept              = default ;
    ScrollLeft::ScrollLeft(ScrollLeft&&)            = default ;
    ScrollLeft& ScrollLeft::operator=(ScrollLeft&&) = default ;

    const std::string ScrollLeft::sname() noexcept {
        return "scroll_left" ;
    }

    void ScrollLeft::sprocess(
            bool first_call,
            unsigned int repeat_num,
            KeycodeLogger* const UNUSED(parent_vkclgr),
            const CharLogger* const UNUSED(parent_charlgr)) const {
        if(first_call) {
            pimpl->timer.reset() ;
        }
        if(!pimpl->timer.is_passed()) {
            return ;
        }
        mouse::hscroll(-iparams::get_i("xscroll_speed") \
                * (first_call ? repeat_num : 1)) ;
    }


    //ScrollRight
    struct ScrollRight::Impl {
        IntervalTimer timer{DELTA_US} ;
    } ;

    ScrollRight::ScrollRight()
    : pimpl(std::make_unique<Impl>())
    {}

    ScrollRight::~ScrollRight() noexcept                = default ;
    ScrollRight::ScrollRight(ScrollRight&&)             = default ;
    ScrollRight& ScrollRight::operator=(ScrollRight&&)  = default ;

    const std::string ScrollRight::sname() noexcept {
        return "scroll_right" ;
    }

    void ScrollRight::sprocess(
            bool first_call,
            unsigned int repeat_num,
            KeycodeLogger* const UNUSED(parent_vkclgr),
            const CharLogger* const UNUSED(parent_charlgr)) const {
        if(first_call) {
            pimpl->timer.reset() ;
        }
        if(!pimpl->timer.is_passed()) {
            return ;
        }
        mouse::hscroll(iparams::get_f("xscroll_speed") \
                * (first_call ? repeat_num : 1)) ;
    }


    //ScrollMidLeft
    struct ScrollMidLeft::Impl {
        IntervalTimer timer{DELTA_US} ;
    } ;

    ScrollMidLeft::ScrollMidLeft()
    : pimpl(std::make_unique<Impl>())
    {}

    ScrollMidLeft::~ScrollMidLeft() noexcept                    = default ;
    ScrollMidLeft::ScrollMidLeft(ScrollMidLeft&&)               = default ;
    ScrollMidLeft& ScrollMidLeft::operator=(ScrollMidLeft&&)    = default ;

    const std::string ScrollMidLeft::sname() noexcept {
        return "scroll_mid_left" ;
    }

    void ScrollMidLeft::sprocess(
            bool first_call,
            unsigned int repeat_num,
            KeycodeLogger* const UNUSED(parent_vkclgr),
            const CharLogger* const UNUSED(parent_charlgr)) const {
        if(first_call) {
            pimpl->timer.reset() ;
        }
        if(!pimpl->timer.is_passed()) {
            return ;
        }
        mouse::hscroll(iparams::get_f("xscroll_screen_ratio") \
                * -0.5f * MAX_X_POS * (first_call ? repeat_num : 1)) ;
    }


    //ScrollMidRight
    struct ScrollMidRight::Impl {
        IntervalTimer timer{DELTA_US} ;
    } ;

    ScrollMidRight::ScrollMidRight()
    : pimpl(std::make_unique<Impl>())
    {}

    ScrollMidRight::~ScrollMidRight() noexcept                  = default ;
    ScrollMidRight::ScrollMidRight(ScrollMidRight&&)            = default ;
    ScrollMidRight& ScrollMidRight::operator=(ScrollMidRight&&) = default ;

    const std::string ScrollMidRight::sname() noexcept {
        return "scroll_mid_right" ;
    }

    void ScrollMidRight::sprocess(
            bool first_call,
            unsigned int repeat_num,
            KeycodeLogger* const UNUSED(parent_vkclgr),
            const CharLogger* const UNUSED(parent_charlgr)) const {
        if(first_call) {
            pimpl->timer.reset() ;
        }
        if(!pimpl->timer.is_passed()) {
            return ;
        }
        mouse::hscroll(iparams::get_f("xscroll_screen_ratio") \
                * MAX_X_POS * 0.5f * (first_call ? repeat_num : 1)) ;
    }
}