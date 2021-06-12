#include "bind/mouse/scroll.hpp"

#include <iostream>
#include <windows.h>

#include "g_params.hpp"
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
        mouse::vscroll(gparams::get_i("yscroll_speed") * repeat_num) ;
    }
    void ScrollUp::sprocess(NTypeLogger& parent_lgr) const {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(parent_lgr.get_head_num()) ;
            pimpl->ksr_.reset() ;
        }
        else if(pimpl->ksr_.is_passed()) {
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
        mouse::vscroll(-gparams::get_i("yscroll_speed") * repeat_num) ;
    }
    void ScrollDown::sprocess(NTypeLogger& parent_lgr) const {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(parent_lgr.get_head_num()) ;
            pimpl->ksr_.reset() ;
        }
        else if(pimpl->ksr_.is_passed()) {
            sprocess(1) ;
        }
    }
    void ScrollDown::sprocess(const CharLogger& UNUSED(parent_lgr)) const {
        sprocess(1) ;
    }


    //ScrollUpHalfPage
    struct ScrollUpHalfPage::Impl {
        IntervalTimer timer_{DELTA_US} ;
    } ;

    ScrollUpHalfPage::ScrollUpHalfPage()
    : BindedFuncCreator("scroll_up_halfpage"),
      pimpl(std::make_unique<Impl>())
    {}

    ScrollUpHalfPage::~ScrollUpHalfPage() noexcept               = default ;
    ScrollUpHalfPage::ScrollUpHalfPage(ScrollUpHalfPage&&)            = default ;
    ScrollUpHalfPage& ScrollUpHalfPage::operator=(ScrollUpHalfPage&&) = default ;

    void ScrollUpHalfPage::sprocess(unsigned int repeat_num) const {
        mouse::vscroll(MAX_Y_POS * 0.5f * repeat_num * \
                gparams::get_f("yscroll_screen_ratio")) ;
    }
    void ScrollUpHalfPage::sprocess(NTypeLogger& parent_lgr) const {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(parent_lgr.get_head_num()) ;
            pimpl->timer_.reset() ;
        }
        else if(pimpl->timer_.is_passed()) {
            sprocess(1) ;
        }
    }
    void ScrollUpHalfPage::sprocess(const CharLogger& UNUSED(parent_lgr)) const {
        sprocess(1) ;
    }


    //ScrollDownHalfPage
    struct ScrollDownHalfPage::Impl {
        IntervalTimer timer_{DELTA_US} ;
    } ;

    ScrollDownHalfPage::ScrollDownHalfPage()
    : BindedFuncCreator("scroll_down_halfpage"),
      pimpl(std::make_unique<Impl>())
    {}

    ScrollDownHalfPage::~ScrollDownHalfPage() noexcept                    = default ;
    ScrollDownHalfPage::ScrollDownHalfPage(ScrollDownHalfPage&&)               = default ;
    ScrollDownHalfPage& ScrollDownHalfPage::operator=(ScrollDownHalfPage&&)    = default ;

    void ScrollDownHalfPage::sprocess(unsigned int repeat_num) const {
        mouse::vscroll(-0.5f * MAX_Y_POS * repeat_num * \
                gparams::get_f("yscroll_screen_ratio")) ;
    }
    void ScrollDownHalfPage::sprocess(NTypeLogger& parent_lgr) const {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(parent_lgr.get_head_num()) ;
            pimpl->timer_.reset() ;
        }
        else if(pimpl->timer_.is_passed()) {
            sprocess(1) ;
        }
    }
    void ScrollDownHalfPage::sprocess(const CharLogger& UNUSED(parent_lgr)) const {
        sprocess(1) ;
    }


    //ScrollUpOnePage
    struct ScrollUpOnePage::Impl {
        IntervalTimer timer_{DELTA_US} ;
    } ;

    ScrollUpOnePage::ScrollUpOnePage()
    : BindedFuncCreator("scroll_up_onepage"),
      pimpl(std::make_unique<Impl>())
    {}

    ScrollUpOnePage::~ScrollUpOnePage() noexcept                  = default ;
    ScrollUpOnePage::ScrollUpOnePage(ScrollUpOnePage&&)              = default ;
    ScrollUpOnePage& ScrollUpOnePage::operator=(ScrollUpOnePage&&)   = default ;

    void ScrollUpOnePage::sprocess(unsigned int repeat_num) const {
        mouse::vscroll(MAX_Y_POS * repeat_num * \
                gparams::get_f("yscroll_screen_ratio")) ;
    }
    void ScrollUpOnePage::sprocess(NTypeLogger& parent_lgr) const {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(parent_lgr.get_head_num()) ;
            pimpl->timer_.reset() ;
        }
        else if(pimpl->timer_.is_passed()) {
            sprocess(1) ;
        }
    }
    void ScrollUpOnePage::sprocess(const CharLogger& UNUSED(parent_lgr)) const {
        sprocess(1) ;
    }


    //ScrollDownOnePage
    struct ScrollDownOnePage::Impl {
        IntervalTimer timer_{DELTA_US} ;
    } ;

    ScrollDownOnePage::ScrollDownOnePage()
    : BindedFuncCreator("scroll_down_onepage"),
      pimpl(std::make_unique<Impl>())
    {}

    ScrollDownOnePage::~ScrollDownOnePage() noexcept                  = default ;
    ScrollDownOnePage::ScrollDownOnePage(ScrollDownOnePage&&)            = default ;
    ScrollDownOnePage& ScrollDownOnePage::operator=(ScrollDownOnePage&&) = default ;

    void ScrollDownOnePage::sprocess(unsigned int repeat_num) const {
        mouse::vscroll(-MAX_Y_POS * repeat_num * \
                gparams::get_f("yscroll_screen_ratio")) ;
    }
    void ScrollDownOnePage::sprocess(NTypeLogger& parent_lgr) const {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(parent_lgr.get_head_num()) ;
            pimpl->timer_.reset() ;
        }
        else if(pimpl->timer_.is_passed()) {
            sprocess(1) ;
        }
    }
    void ScrollDownOnePage::sprocess(const CharLogger& UNUSED(parent_lgr)) const {
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
        mouse::hscroll(-gparams::get_i("xscroll_speed") * repeat_num) ;
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
        mouse::hscroll(gparams::get_f("xscroll_speed") * repeat_num) ;
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


    //ScrollLeftHalfPage
    struct ScrollLeftHalfPage::Impl {
        IntervalTimer timer_{DELTA_US} ;
    } ;

    ScrollLeftHalfPage::ScrollLeftHalfPage()
    : BindedFuncCreator("scroll_left_halfpage"),
      pimpl(std::make_unique<Impl>())
    {}

    ScrollLeftHalfPage::~ScrollLeftHalfPage() noexcept                    = default ;
    ScrollLeftHalfPage::ScrollLeftHalfPage(ScrollLeftHalfPage&&)               = default ;
    ScrollLeftHalfPage& ScrollLeftHalfPage::operator=(ScrollLeftHalfPage&&)    = default ;

    void ScrollLeftHalfPage::sprocess(unsigned int repeat_num) const {
        mouse::hscroll(-0.5f * MAX_X_POS * repeat_num * \
                gparams::get_f("xscroll_screen_ratio")) ;
    }
    void ScrollLeftHalfPage::sprocess(NTypeLogger& parent_lgr) const {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(parent_lgr.get_head_num()) ;
            pimpl->timer_.reset() ;
        }
        else if(pimpl->timer_.is_passed()) {
            sprocess(1) ;
        }
    }
    void ScrollLeftHalfPage::sprocess(const CharLogger& UNUSED(parent_lgr)) const {
        sprocess(1) ;
    }


    //ScrollRightHalfPage
    struct ScrollRightHalfPage::Impl {
        IntervalTimer timer_{DELTA_US} ;
    } ;

    ScrollRightHalfPage::ScrollRightHalfPage()
    : BindedFuncCreator("scroll_right_halfpage"),
      pimpl(std::make_unique<Impl>())
    {}

    ScrollRightHalfPage::~ScrollRightHalfPage() noexcept                  = default ;
    ScrollRightHalfPage::ScrollRightHalfPage(ScrollRightHalfPage&&)            = default ;
    ScrollRightHalfPage& ScrollRightHalfPage::operator=(ScrollRightHalfPage&&) = default ;

    void ScrollRightHalfPage::sprocess(unsigned int repeat_num) const {
        mouse::hscroll(MAX_X_POS * 0.5f * repeat_num * \
                gparams::get_f("xscroll_screen_ratio")) ;
    }
    void ScrollRightHalfPage::sprocess(NTypeLogger& parent_lgr) const {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(parent_lgr.get_head_num()) ;
            pimpl->timer_.reset() ;
        }
        else if(pimpl->timer_.is_passed()) {
            sprocess(1) ;
        }
    }
    void ScrollRightHalfPage::sprocess(const CharLogger& UNUSED(parent_lgr)) const {
        sprocess(1) ;
    }
}
