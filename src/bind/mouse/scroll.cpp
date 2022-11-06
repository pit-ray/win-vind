#include "scroll.hpp"

#include <windows.h>

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
        ScrollUp::ScrollUp()
        : BindedFuncVoid("scroll_up")
        {}
        void ScrollUp::sprocess(
                std::uint16_t count,
                const std::string& UNUSED(args)) {
            auto& settable = core::SetTable::get_instance() ;
            util::vscroll(settable.get("vscroll_speed").get<int>() * count) ;
        }

        //ScrollDown
        ScrollDown::ScrollDown()
        : BindedFuncVoid("scroll_down")
        {}
        void ScrollDown::sprocess(
                std::uint16_t count,
                const std::string& UNUSED(args)) {
            auto& settable = core::SetTable::get_instance() ;
            util::vscroll(-settable.get("vscroll_speed").get<int>() * count) ;
        }

        //ScrollUpHalfPage
        ScrollUpHalfPage::ScrollUpHalfPage()
        : BindedFuncVoid("scroll_up_halfpage")
        {}
        void ScrollUpHalfPage::sprocess(
                std::uint16_t count,
                const std::string& UNUSED(args)) {
            auto& settable = core::SetTable::get_instance() ;
            util::MonitorInfo m_info ;
            util::get_monitor_metrics(util::get_cursor_pos(), m_info) ;
            util::vscroll(0.5f * static_cast<float>(m_info.rect.height() * count) * \
                    settable.get("vscroll_pageratio").get<float>()) ;
        }

        //ScrollDownHalfPage
        ScrollDownHalfPage::ScrollDownHalfPage()
        : BindedFuncVoid("scroll_down_halfpage")
        {}
        void ScrollDownHalfPage::sprocess(
                std::uint16_t count,
                const std::string& UNUSED(args)) {
            auto& settable = core::SetTable::get_instance() ;
            util::MonitorInfo m_info ;
            util::get_monitor_metrics(util::get_cursor_pos(), m_info) ;
            util::vscroll(-0.5f * static_cast<float>(m_info.rect.height() * count) * \
                    settable.get("vscroll_pageratio").get<float>()) ;
        }

        //ScrollUpOnePage
        ScrollUpOnePage::ScrollUpOnePage()
        : BindedFuncVoid("scroll_up_onepage")
        {}
        void ScrollUpOnePage::sprocess(
                std::uint16_t count,
                const std::string& UNUSED(args)) {
            auto& settable = core::SetTable::get_instance() ;
            util::MonitorInfo m_info ;
            util::get_monitor_metrics(util::get_cursor_pos(), m_info) ;
            util::vscroll(static_cast<float>(m_info.rect.height() * count) * \
                    settable.get("vscroll_pageratio").get<float>()) ;
        }

        //ScrollDownOnePage
        ScrollDownOnePage::ScrollDownOnePage()
        : BindedFuncVoid("scroll_down_onepage")
        {}
        void ScrollDownOnePage::sprocess(
                std::uint16_t count,
                const std::string& UNUSED(args)) {
            auto& settable = core::SetTable::get_instance() ;
            util::MonitorInfo m_info ;
            util::get_monitor_metrics(util::get_cursor_pos(), m_info) ;
            util::vscroll(static_cast<float>(m_info.rect.height() * count) * \
                    -1 * settable.get("vscroll_pageratio").get<float>()) ;
        }

        //Horizontal
        //ScrollLeft
        ScrollLeft::ScrollLeft()
        : BindedFuncVoid("scroll_left")
        {}
        void ScrollLeft::sprocess(
                std::uint16_t count,
                const std::string& UNUSED(args)) {
            auto& settable = core::SetTable::get_instance() ;
            util::hscroll(-settable.get("hscroll_speed").get<int>() * count) ;
        }

        //ScrollRight
        ScrollRight::ScrollRight()
        : BindedFuncVoid("scroll_right")
        {}
        void ScrollRight::sprocess(
                std::uint16_t count,
                const std::string& UNUSED(args)) {
            auto& settable = core::SetTable::get_instance() ;
            util::hscroll(settable.get("hscroll_speed").get<float>() * count) ;
        }

        //ScrollLeftHalfPage
        ScrollLeftHalfPage::ScrollLeftHalfPage()
        : BindedFuncVoid("scroll_left_halfpage")
        {}
        void ScrollLeftHalfPage::sprocess(
                std::uint16_t count,
                const std::string& UNUSED(args)) {
            auto& settable = core::SetTable::get_instance() ;
            util::MonitorInfo m_info ;
            util::get_monitor_metrics(util::get_cursor_pos(), m_info) ;
            util::hscroll(-0.5f * static_cast<float>(m_info.rect.width() * count) * \
                    settable.get("hscroll_pageratio").get<float>()) ;
        }

        //ScrollRightHalfPage
        ScrollRightHalfPage::ScrollRightHalfPage()
        : BindedFuncVoid("scroll_right_halfpage")
        {}
        void ScrollRightHalfPage::sprocess(
                std::uint16_t count,
                const std::string& UNUSED(args)) {
            auto& settable = core::SetTable::get_instance() ;
            util::MonitorInfo m_info ;
            util::get_monitor_metrics(util::get_cursor_pos(), m_info) ;
            util::hscroll(0.5f * static_cast<float>(m_info.rect.width() * count) * \
                    settable.get("hscroll_pageratio").get<float>()) ;
        }
    }
}
