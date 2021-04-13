#include "bind/mouse/scroll.hpp"

#include <windows.h>
#include <iostream>

#include "coreio/i_params.hpp"
#include "io/mouse.hpp"
#include "time/interval_timer.hpp"
#include "time/keystroke_repeater.hpp"
#include "util/def.hpp"
#include "bind/base/ntype_logger.hpp"

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
        KeyStrokeRepeater ksr_{WAIT_MS} ;
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

    void ScrollUp::sprocess(unsigned int repeat_num) const {
        mouse::vscroll(iparams::get_i("yscroll_speed") * repeat_num) ;
    }
    void ScrollUp::sprocess(NTypeLogger& parent_lgr) const {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(parent_lgr.get_head_num()) ;
            pimpl->ksr_.reset() ;
        }
        else if(pimpl->ksr_.is_pressed()) {
            sprocess(1) ;
        }
    }
    void ScrollUp::sprocess(const CharLogger& UNUSED(parent_lgr)) const {
        sprocess(1) ;
    }


    //ScrollDown
    struct ScrollDown::Impl {
        KeyStrokeRepeater ksr_{WAIT_MS} ;
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

    void ScrollDown::sprocess(unsigned int repeat_num) const {
        mouse::vscroll(-iparams::get_i("yscroll_speed") * repeat_num) ;
    }
    void ScrollDown::sprocess(NTypeLogger& parent_lgr) const {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(parent_lgr.get_head_num()) ;
            pimpl->ksr_.reset() ;
        }
        else if(pimpl->ksr_.is_pressed()) {
            sprocess(1) ;
        }
    }
    void ScrollDown::sprocess(const CharLogger& UNUSED(parent_lgr)) const {
        sprocess(1) ;
    }


    //ScrollMidUp
    struct ScrollMidUp::Impl {
        IntervalTimer timer_{DELTA_US} ;
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

    void ScrollMidUp::sprocess(unsigned int repeat_num) const {
        mouse::vscroll(MAX_Y_POS * 0.5f * repeat_num * \
                iparams::get_f("yscroll_screen_ratio")) ;
    }
    void ScrollMidUp::sprocess(NTypeLogger& parent_lgr) const {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(parent_lgr.get_head_num()) ;
            pimpl->timer_.reset() ;
        }
        else if(pimpl->timer_.is_passed()) {
            sprocess(1) ;
        }
    }
    void ScrollMidUp::sprocess(const CharLogger& UNUSED(parent_lgr)) const {
        sprocess(1) ;
    }


    //ScrollMidDown
    struct ScrollMidDown::Impl {
        IntervalTimer timer_{DELTA_US} ;
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

    void ScrollMidDown::sprocess(unsigned int repeat_num) const {
        mouse::vscroll(-0.5f * MAX_Y_POS * repeat_num * \
                iparams::get_f("yscroll_screen_ratio")) ;
    }
    void ScrollMidDown::sprocess(NTypeLogger& parent_lgr) const {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(parent_lgr.get_head_num()) ;
            pimpl->timer_.reset() ;
        }
        else if(pimpl->timer_.is_passed()) {
            sprocess(1) ;
        }
    }
    void ScrollMidDown::sprocess(const CharLogger& UNUSED(parent_lgr)) const {
        sprocess(1) ;
    }


    //ScrollPageUp
    struct ScrollPageUp::Impl {
        IntervalTimer timer_{DELTA_US} ;
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

    void ScrollPageUp::sprocess(unsigned int repeat_num) const {
        mouse::vscroll(MAX_Y_POS * repeat_num * \
                iparams::get_f("yscroll_screen_ratio")) ;
    }
    void ScrollPageUp::sprocess(NTypeLogger& parent_lgr) const {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(parent_lgr.get_head_num()) ;
            pimpl->timer_.reset() ;
        }
        else if(pimpl->timer_.is_passed()) {
            sprocess(1) ;
        }
    }
    void ScrollPageUp::sprocess(const CharLogger& UNUSED(parent_lgr)) const {
        sprocess(1) ;
    }


    //ScrollPageDown
    struct ScrollPageDown::Impl {
        IntervalTimer timer_{DELTA_US} ;
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

    void ScrollPageDown::sprocess(unsigned int repeat_num) const {
        mouse::vscroll(-MAX_Y_POS * repeat_num * \
                iparams::get_f("yscroll_screen_ratio")) ;
    }
    void ScrollPageDown::sprocess(NTypeLogger& parent_lgr) const {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(parent_lgr.get_head_num()) ;
            pimpl->timer_.reset() ;
        }
        else if(pimpl->timer_.is_passed()) {
            sprocess(1) ;
        }
    }
    void ScrollPageDown::sprocess(const CharLogger& UNUSED(parent_lgr)) const {
        sprocess(1) ;
    }


    //Horizontal
    //ScrollLeft
    struct ScrollLeft::Impl {
        IntervalTimer timer_{DELTA_US} ;
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

    void ScrollLeft::sprocess(unsigned int repeat_num) const {
        mouse::hscroll(-iparams::get_i("xscroll_speed") * repeat_num) ;
    }
    void ScrollLeft::sprocess(NTypeLogger& parent_lgr) const {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(parent_lgr.get_head_num()) ;
            pimpl->timer_.reset() ;
        }
        else if(pimpl->timer_.is_passed()) {
            sprocess(1) ;
        }
    }
    void ScrollLeft::sprocess(const CharLogger& UNUSED(parent_lgr)) const {
        sprocess(1) ;
    }


    //ScrollRight
    struct ScrollRight::Impl {
        IntervalTimer timer_{DELTA_US} ;
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

    void ScrollRight::sprocess(unsigned int repeat_num) const {
        mouse::hscroll(iparams::get_f("xscroll_speed") * repeat_num) ;
    }
    void ScrollRight::sprocess(NTypeLogger& parent_lgr) const {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(parent_lgr.get_head_num()) ;
            pimpl->timer_.reset() ;
        }
        else if(pimpl->timer_.is_passed()) {
            sprocess(1) ;
        }
    }
    void ScrollRight::sprocess(const CharLogger& UNUSED(parent_lgr)) const {
        sprocess(1) ;
    }


    //ScrollMidLeft
    struct ScrollMidLeft::Impl {
        IntervalTimer timer_{DELTA_US} ;
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

    void ScrollMidLeft::sprocess(unsigned int repeat_num) const {
        mouse::hscroll(-0.5f * MAX_X_POS * repeat_num * \
                iparams::get_f("xscroll_screen_ratio")) ;
    }
    void ScrollMidLeft::sprocess(NTypeLogger& parent_lgr) const {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(parent_lgr.get_head_num()) ;
            pimpl->timer_.reset() ;
        }
        else if(pimpl->timer_.is_passed()) {
            sprocess(1) ;
        }
    }
    void ScrollMidLeft::sprocess(const CharLogger& UNUSED(parent_lgr)) const {
        sprocess(1) ;
    }


    //ScrollMidRight
    struct ScrollMidRight::Impl {
        IntervalTimer timer_{DELTA_US} ;
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

    void ScrollMidRight::sprocess(unsigned int repeat_num) const {
        mouse::hscroll(MAX_X_POS * 0.5f * repeat_num * \
                iparams::get_f("xscroll_screen_ratio")) ;
    }
    void ScrollMidRight::sprocess(NTypeLogger& parent_lgr) const {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(parent_lgr.get_head_num()) ;
            pimpl->timer_.reset() ;
        }
        else if(pimpl->timer_.is_passed()) {
            sprocess(1) ;
        }
    }
    void ScrollMidRight::sprocess(const CharLogger& UNUSED(parent_lgr)) const {
        sprocess(1) ;
    }
}
