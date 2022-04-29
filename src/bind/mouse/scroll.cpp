#include "scroll.hpp"

#include <windows.h>

#include "core/ntypelogger.hpp"
#include "core/settable.hpp"
#include "util/debug.hpp"
#include "util/def.hpp"
#include "util/interval_timer.hpp"
#include "util/keystroke_repeater.hpp"
#include "util/mouse.hpp"
#include "util/screen_metrics.hpp"
#include "util/winwrap.hpp"


namespace
{
    constexpr auto DELTA_US = 30 ;
    constexpr auto WAIT_MS  = 30 ;

    constexpr auto SCROLL_FACTOR_FROM_COMMAND = 10 ;
}


namespace vind
{
    namespace bind
    {
        //ScrollUp
        struct ScrollUp::Impl {
            util::KeyStrokeRepeater ksr_{WAIT_MS} ;
        } ;

        ScrollUp::ScrollUp()
        : BindedFuncVoid("scroll_up"),
          pimpl(std::make_unique<Impl>())
        {}

        ScrollUp::~ScrollUp() noexcept            = default ;
        ScrollUp::ScrollUp(ScrollUp&&)            = default ;
        ScrollUp& ScrollUp::operator=(ScrollUp&&) = default ;

        void ScrollUp::sprocess(unsigned int count) const {
            auto& settable = core::SetTable::get_instance() ;
            util::vscroll(
                    settable.get("vscroll_speed").get<int>() * count) ;
        }
        void ScrollUp::sprocess(core::NTypeLogger& parent_lgr) const {
            if(!parent_lgr.is_long_pressing()) {
                sprocess(parent_lgr.get_head_num()) ;
                pimpl->ksr_.reset() ;
            }
            else if(pimpl->ksr_.is_passed()) {
                sprocess(1) ;
            }
        }
        void ScrollUp::sprocess(const core::CharLogger& UNUSED(parent_lgr)) const {
            sprocess(1) ;
        }


        //ScrollDown
        struct ScrollDown::Impl {
            util::KeyStrokeRepeater ksr_{WAIT_MS} ;
        } ;

        ScrollDown::ScrollDown()
        : BindedFuncVoid("scroll_down"),
          pimpl(std::make_unique<Impl>())
        {}

        ScrollDown::~ScrollDown() noexcept              = default ;
        ScrollDown::ScrollDown(ScrollDown&&)            = default ;
        ScrollDown& ScrollDown::operator=(ScrollDown&&) = default ;

        void ScrollDown::sprocess(unsigned int count) const {
            auto& settable = core::SetTable::get_instance() ;
            util::vscroll(
                    -1 * settable.get("vscroll_speed").get<int>() * count) ;
        }
        void ScrollDown::sprocess(core::NTypeLogger& parent_lgr) const {
            if(!parent_lgr.is_long_pressing()) {
                sprocess(parent_lgr.get_head_num()) ;
                pimpl->ksr_.reset() ;
            }
            else if(pimpl->ksr_.is_passed()) {
                sprocess(1) ;
            }
        }
        void ScrollDown::sprocess(const core::CharLogger& UNUSED(parent_lgr)) const {
            sprocess(1) ;
        }


        //ScrollUpHalfPage
        struct ScrollUpHalfPage::Impl {
            util::IntervalTimer timer_{DELTA_US} ;
        } ;

        ScrollUpHalfPage::ScrollUpHalfPage()
        : BindedFuncVoid("scroll_up_halfpage"),
          pimpl(std::make_unique<Impl>())
        {}

        ScrollUpHalfPage::~ScrollUpHalfPage() noexcept               = default ;
        ScrollUpHalfPage::ScrollUpHalfPage(ScrollUpHalfPage&&)            = default ;
        ScrollUpHalfPage& ScrollUpHalfPage::operator=(ScrollUpHalfPage&&) = default ;

        void ScrollUpHalfPage::sprocess(unsigned int count) const {
            auto& settable = core::SetTable::get_instance() ;
            util::MonitorInfo m_info ;
            util::get_monitor_metrics(util::get_cursor_pos(), m_info) ;
            util::vscroll(0.5f * static_cast<float>(m_info.rect.height() * count) * \
                    settable.get("vscroll_pageratio").get<float>()) ;
        }
        void ScrollUpHalfPage::sprocess(core::NTypeLogger& parent_lgr) const {
            if(!parent_lgr.is_long_pressing()) {
                sprocess(parent_lgr.get_head_num()) ;
                pimpl->timer_.reset() ;
            }
            else if(pimpl->timer_.is_passed()) {
                sprocess(1) ;
            }
        }
        void ScrollUpHalfPage::sprocess(const core::CharLogger& UNUSED(parent_lgr)) const {
            sprocess(SCROLL_FACTOR_FROM_COMMAND) ;
        }


        //ScrollDownHalfPage
        struct ScrollDownHalfPage::Impl {
            util::IntervalTimer timer_{DELTA_US} ;
        } ;

        ScrollDownHalfPage::ScrollDownHalfPage()
        : BindedFuncVoid("scroll_down_halfpage"),
          pimpl(std::make_unique<Impl>())
        {}

        ScrollDownHalfPage::~ScrollDownHalfPage() noexcept                    = default ;
        ScrollDownHalfPage::ScrollDownHalfPage(ScrollDownHalfPage&&)               = default ;
        ScrollDownHalfPage& ScrollDownHalfPage::operator=(ScrollDownHalfPage&&)    = default ;

        void ScrollDownHalfPage::sprocess(unsigned int count) const {
            auto& settable = core::SetTable::get_instance() ;
            util::MonitorInfo m_info ;
            util::get_monitor_metrics(util::get_cursor_pos(), m_info) ;
            util::vscroll(-0.5f * static_cast<float>(m_info.rect.height() * count) * \
                    settable.get("vscroll_pageratio").get<float>()) ;
        }
        void ScrollDownHalfPage::sprocess(core::NTypeLogger& parent_lgr) const {
            if(!parent_lgr.is_long_pressing()) {
                sprocess(parent_lgr.get_head_num()) ;
                pimpl->timer_.reset() ;
            }
            else if(pimpl->timer_.is_passed()) {
                sprocess(1) ;
            }
        }
        void ScrollDownHalfPage::sprocess(const core::CharLogger& UNUSED(parent_lgr)) const {
            sprocess(SCROLL_FACTOR_FROM_COMMAND) ;
        }


        //ScrollUpOnePage
        struct ScrollUpOnePage::Impl {
            util::IntervalTimer timer_{DELTA_US} ;
        } ;

        ScrollUpOnePage::ScrollUpOnePage()
        : BindedFuncVoid("scroll_up_onepage"),
          pimpl(std::make_unique<Impl>())
        {}

        ScrollUpOnePage::~ScrollUpOnePage() noexcept                  = default ;
        ScrollUpOnePage::ScrollUpOnePage(ScrollUpOnePage&&)              = default ;
        ScrollUpOnePage& ScrollUpOnePage::operator=(ScrollUpOnePage&&)   = default ;

        void ScrollUpOnePage::sprocess(unsigned int count) const {
            auto& settable = core::SetTable::get_instance() ;
            util::MonitorInfo m_info ;
            util::get_monitor_metrics(util::get_cursor_pos(), m_info) ;
            util::vscroll(static_cast<float>(m_info.rect.height() * count) * \
                    settable.get("vscroll_pageratio").get<float>()) ;
        }
        void ScrollUpOnePage::sprocess(core::NTypeLogger& parent_lgr) const {
            if(!parent_lgr.is_long_pressing()) {
                sprocess(parent_lgr.get_head_num()) ;
                pimpl->timer_.reset() ;
            }
            else if(pimpl->timer_.is_passed()) {
                sprocess(1) ;
            }
        }
        void ScrollUpOnePage::sprocess(const core::CharLogger& UNUSED(parent_lgr)) const {
            sprocess(SCROLL_FACTOR_FROM_COMMAND) ;
        }


        //ScrollDownOnePage
        struct ScrollDownOnePage::Impl {
            util::IntervalTimer timer_{DELTA_US} ;
        } ;

        ScrollDownOnePage::ScrollDownOnePage()
        : BindedFuncVoid("scroll_down_onepage"),
          pimpl(std::make_unique<Impl>())
        {}

        ScrollDownOnePage::~ScrollDownOnePage() noexcept                  = default ;
        ScrollDownOnePage::ScrollDownOnePage(ScrollDownOnePage&&)            = default ;
        ScrollDownOnePage& ScrollDownOnePage::operator=(ScrollDownOnePage&&) = default ;

        void ScrollDownOnePage::sprocess(unsigned int count) const {
            auto& settable = core::SetTable::get_instance() ;
            util::MonitorInfo m_info ;
            util::get_monitor_metrics(util::get_cursor_pos(), m_info) ;
            util::vscroll(static_cast<float>(m_info.rect.height() * count) * \
                    -1 * settable.get("vscroll_pageratio").get<float>()) ;
        }
        void ScrollDownOnePage::sprocess(core::NTypeLogger& parent_lgr) const {
            if(!parent_lgr.is_long_pressing()) {
                sprocess(parent_lgr.get_head_num()) ;
                pimpl->timer_.reset() ;
            }
            else if(pimpl->timer_.is_passed()) {
                sprocess(1) ;
            }
        }
        void ScrollDownOnePage::sprocess(const core::CharLogger& UNUSED(parent_lgr)) const {
            sprocess(SCROLL_FACTOR_FROM_COMMAND) ;
        }


        //Horizontal
        //ScrollLeft
        struct ScrollLeft::Impl {
            util::IntervalTimer timer_{DELTA_US} ;
        } ;

        ScrollLeft::ScrollLeft()
        : BindedFuncVoid("scroll_left"),
          pimpl(std::make_unique<Impl>())
        {}

        ScrollLeft::~ScrollLeft() noexcept              = default ;
        ScrollLeft::ScrollLeft(ScrollLeft&&)            = default ;
        ScrollLeft& ScrollLeft::operator=(ScrollLeft&&) = default ;

        void ScrollLeft::sprocess(unsigned int count) const {
            auto& settable = core::SetTable::get_instance() ;
            util::hscroll(
                    -1 * settable.get("hscroll_speed").get<int>() * count) ;
        }
        void ScrollLeft::sprocess(core::NTypeLogger& parent_lgr) const {
            if(!parent_lgr.is_long_pressing()) {
                sprocess(parent_lgr.get_head_num()) ;
                pimpl->timer_.reset() ;
            }
            else if(pimpl->timer_.is_passed()) {
                sprocess(1) ;
            }
        }
        void ScrollLeft::sprocess(const core::CharLogger& UNUSED(parent_lgr)) const {
            sprocess(SCROLL_FACTOR_FROM_COMMAND) ;
        }


        //ScrollRight
        struct ScrollRight::Impl {
            util::IntervalTimer timer_{DELTA_US} ;
        } ;

        ScrollRight::ScrollRight()
        : BindedFuncVoid("scroll_right"),
          pimpl(std::make_unique<Impl>())
        {}

        ScrollRight::~ScrollRight() noexcept                = default ;
        ScrollRight::ScrollRight(ScrollRight&&)             = default ;
        ScrollRight& ScrollRight::operator=(ScrollRight&&)  = default ;

        void ScrollRight::sprocess(unsigned int count) const {
            auto& settable = core::SetTable::get_instance() ;
            util::hscroll(
                    settable.get("hscroll_speed").get<float>() * count) ;
        }
        void ScrollRight::sprocess(core::NTypeLogger& parent_lgr) const {
            if(!parent_lgr.is_long_pressing()) {
                sprocess(parent_lgr.get_head_num()) ;
                pimpl->timer_.reset() ;
            }
            else if(pimpl->timer_.is_passed()) {
                sprocess(1) ;
            }
        }
        void ScrollRight::sprocess(const core::CharLogger& UNUSED(parent_lgr)) const {
            sprocess(SCROLL_FACTOR_FROM_COMMAND) ;
        }


        //ScrollLeftHalfPage
        struct ScrollLeftHalfPage::Impl {
            util::IntervalTimer timer_{DELTA_US} ;
        } ;

        ScrollLeftHalfPage::ScrollLeftHalfPage()
        : BindedFuncVoid("scroll_left_halfpage"),
          pimpl(std::make_unique<Impl>())
        {}

        ScrollLeftHalfPage::~ScrollLeftHalfPage() noexcept                    = default ;
        ScrollLeftHalfPage::ScrollLeftHalfPage(ScrollLeftHalfPage&&)               = default ;
        ScrollLeftHalfPage& ScrollLeftHalfPage::operator=(ScrollLeftHalfPage&&)    = default ;

        void ScrollLeftHalfPage::sprocess(unsigned int count) const {
            auto& settable = core::SetTable::get_instance() ;
            util::MonitorInfo m_info ;
            util::get_monitor_metrics(util::get_cursor_pos(), m_info) ;
            util::hscroll(-0.5f * static_cast<float>(m_info.rect.width() * count) * \
                    settable.get("hscroll_pageratio").get<float>()) ;
        }
        void ScrollLeftHalfPage::sprocess(core::NTypeLogger& parent_lgr) const {
            if(!parent_lgr.is_long_pressing()) {
                sprocess(parent_lgr.get_head_num()) ;
                pimpl->timer_.reset() ;
            }
            else if(pimpl->timer_.is_passed()) {
                sprocess(1) ;
            }
        }
        void ScrollLeftHalfPage::sprocess(const core::CharLogger& UNUSED(parent_lgr)) const {
            sprocess(SCROLL_FACTOR_FROM_COMMAND) ;
        }


        //ScrollRightHalfPage
        struct ScrollRightHalfPage::Impl {
            util::IntervalTimer timer_{DELTA_US} ;
        } ;

        ScrollRightHalfPage::ScrollRightHalfPage()
        : BindedFuncVoid("scroll_right_halfpage"),
          pimpl(std::make_unique<Impl>())
        {}

        ScrollRightHalfPage::~ScrollRightHalfPage() noexcept                  = default ;
        ScrollRightHalfPage::ScrollRightHalfPage(ScrollRightHalfPage&&)            = default ;
        ScrollRightHalfPage& ScrollRightHalfPage::operator=(ScrollRightHalfPage&&) = default ;

        void ScrollRightHalfPage::sprocess(unsigned int count) const {
            auto& settable = core::SetTable::get_instance() ;
            util::MonitorInfo m_info ;
            util::get_monitor_metrics(util::get_cursor_pos(), m_info) ;
            util::hscroll(0.5f * static_cast<float>(m_info.rect.width() * count) * \
                    settable.get("hscroll_pageratio").get<float>()) ;
        }
        void ScrollRightHalfPage::sprocess(core::NTypeLogger& parent_lgr) const {
            if(!parent_lgr.is_long_pressing()) {
                sprocess(parent_lgr.get_head_num()) ;
                pimpl->timer_.reset() ;
            }
            else if(pimpl->timer_.is_passed()) {
                sprocess(1) ;
            }
        }
        void ScrollRightHalfPage::sprocess(const core::CharLogger& UNUSED(parent_lgr)) const {
            sprocess(SCROLL_FACTOR_FROM_COMMAND) ;
        }
    }
}
