#include "bind/mouse/scroll.hpp"

#include <iostream>
#include <windows.h>

#include "i_params.hpp"
#include "io/mouse.hpp"
#include "key/ntype_logger.hpp"
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
        KeyStrokeRepeater ksr_{WAIT_MS} ;
    } ;

    ScrollUp::ScrollUp()
    : BindedFuncCreator("scroll_up"),
      pimpl(std::make_unique<Impl>())
    {}

    ScrollUp::~ScrollUp() noexcept            = default ;
    ScrollUp::ScrollUp(ScrollUp&&)            = default ;
    ScrollUp& ScrollUp::operator=(ScrollUp&&) = default ;

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
    : BindedFuncCreator("scroll_down"),
      pimpl(std::make_unique<Impl>())
    {}

    ScrollDown::~ScrollDown() noexcept              = default ;
    ScrollDown::ScrollDown(ScrollDown&&)            = default ;
    ScrollDown& ScrollDown::operator=(ScrollDown&&) = default ;

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
    : BindedFuncCreator("scroll_mid_up"),
      pimpl(std::make_unique<Impl>())
    {}

    ScrollMidUp::~ScrollMidUp() noexcept               = default ;
    ScrollMidUp::ScrollMidUp(ScrollMidUp&&)            = default ;
    ScrollMidUp& ScrollMidUp::operator=(ScrollMidUp&&) = default ;

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
    : BindedFuncCreator("scroll_mid_down"),
      pimpl(std::make_unique<Impl>())
    {}

    ScrollMidDown::~ScrollMidDown() noexcept                    = default ;
    ScrollMidDown::ScrollMidDown(ScrollMidDown&&)               = default ;
    ScrollMidDown& ScrollMidDown::operator=(ScrollMidDown&&)    = default ;

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
    : BindedFuncCreator("scroll_page_up"),
      pimpl(std::make_unique<Impl>())
    {}

    ScrollPageUp::~ScrollPageUp() noexcept                  = default ;
    ScrollPageUp::ScrollPageUp(ScrollPageUp&&)              = default ;
    ScrollPageUp& ScrollPageUp::operator=(ScrollPageUp&&)   = default ;

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
    : BindedFuncCreator("scroll_page_down"),
      pimpl(std::make_unique<Impl>())
    {}

    ScrollPageDown::~ScrollPageDown() noexcept                  = default ;
    ScrollPageDown::ScrollPageDown(ScrollPageDown&&)            = default ;
    ScrollPageDown& ScrollPageDown::operator=(ScrollPageDown&&) = default ;

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
    : BindedFuncCreator("scroll_left"),
      pimpl(std::make_unique<Impl>())
    {}

    ScrollLeft::~ScrollLeft() noexcept              = default ;
    ScrollLeft::ScrollLeft(ScrollLeft&&)            = default ;
    ScrollLeft& ScrollLeft::operator=(ScrollLeft&&) = default ;

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
    : BindedFuncCreator("scroll_right"),
      pimpl(std::make_unique<Impl>())
    {}

    ScrollRight::~ScrollRight() noexcept                = default ;
    ScrollRight::ScrollRight(ScrollRight&&)             = default ;
    ScrollRight& ScrollRight::operator=(ScrollRight&&)  = default ;

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
    : BindedFuncCreator("scroll_mid_left"),
      pimpl(std::make_unique<Impl>())
    {}

    ScrollMidLeft::~ScrollMidLeft() noexcept                    = default ;
    ScrollMidLeft::ScrollMidLeft(ScrollMidLeft&&)               = default ;
    ScrollMidLeft& ScrollMidLeft::operator=(ScrollMidLeft&&)    = default ;

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
    : BindedFuncCreator("scroll_mid_right"),
      pimpl(std::make_unique<Impl>())
    {}

    ScrollMidRight::~ScrollMidRight() noexcept                  = default ;
    ScrollMidRight::ScrollMidRight(ScrollMidRight&&)            = default ;
    ScrollMidRight& ScrollMidRight::operator=(ScrollMidRight&&) = default ;

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
