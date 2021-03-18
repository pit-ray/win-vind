#include "resize_window.hpp"

#include "i_params.hpp"
#include "screen_metrics.hpp"
#include "utility.hpp"
#include "virtual_cmd_line.hpp"
#include "window_utility.hpp"

namespace ResizeWindowCommon
{
    inline static auto compute_resize_delta(
            const unsigned int repeat_num,
            const CharLogger* const parent_charlgr,
            const std::string& param_name) {

        if(parent_charlgr != nullptr) {
            return static_cast<LONG>(KeyLoggerUtility::extract_num(parent_charlgr->to_str())) ;
        }
        else {
            return static_cast<LONG>(iParams::get_i(param_name) * repeat_num) ;
        }
    }
}

//ResizeWindowWidth
const std::string ResizeWindowWidth::sname() noexcept
{
    return "resize_window_width" ;
}

void ResizeWindowWidth::sprocess(
        const bool first_call,
        const unsigned int UNUSED(repeat_num),
        VKCLogger* const UNUSED(parent_vkclgr),
        const CharLogger* const parent_charlgr)
{
    if(!first_call) return ;

    WindowUtility::ForegroundInfo fginfo ;

    auto str = parent_charlgr->to_str() ;
    if(str.empty()) return ;

    auto width = static_cast<LONG>(KeyLoggerUtility::extract_num(str)) ;

    ScreenMetrics::MonitorInfo minfo ;
    ScreenMetrics::get_monitor_metrics(fginfo.hwnd, minfo) ;

    const auto window_height = ScreenMetrics::height(fginfo.rect) ;
    const auto monitor_width = ScreenMetrics::width(minfo.work_rect) ;

    if(width >= monitor_width) {
        WindowUtility::resize(
                fginfo.hwnd,
                minfo.work_rect.left, fginfo.rect.top,
                monitor_width, window_height) ;
    }
    else {
        auto right_max_width = minfo.work_rect.right - fginfo.rect.left ;

        if(width <= right_max_width) {
            WindowUtility::resize(
                    fginfo.hwnd, fginfo.rect.left, fginfo.rect.top,
                    width, window_height) ;
        }
        else {
            auto left_shift_delta = width - right_max_width ;
            WindowUtility::resize(
                    fginfo.hwnd, fginfo.rect.left - left_shift_delta, fginfo.rect.top,
                    width, window_height) ;
        }
    }
}

//IncreaseWindowWidth
const std::string IncreaseWindowWidth::sname() noexcept
{
    return "increase_window_width" ;
}

void IncreaseWindowWidth::sprocess(
        const bool first_call,
        const unsigned int repeat_num,
        VKCLogger* const UNUSED(parent_vkclgr),
        const CharLogger* const parent_charlgr)
{
    if(!first_call) return ;

    WindowUtility::ForegroundInfo fginfo ;

    auto width = ScreenMetrics::width(fginfo.rect) ;
    width += ResizeWindowCommon::compute_resize_delta(
            repeat_num, parent_charlgr, "window_width_delta") ;

    ScreenMetrics::MonitorInfo minfo ;
    ScreenMetrics::get_monitor_metrics(fginfo.hwnd, minfo) ;

    auto max_width = minfo.work_rect.right - fginfo.rect.left ;
    if(max_width < width) {
        width = max_width ;
    }

    WindowUtility::resize(
            fginfo.hwnd, fginfo.rect.left, fginfo.rect.top,
            width, ScreenMetrics::height(fginfo.rect)) ;
}

//DecreaseWindowWidth
const std::string DecreaseWindowWidth::sname() noexcept
{
    return "decrease_window_width" ;
}

void DecreaseWindowWidth::sprocess(
        const bool first_call,
        const unsigned int repeat_num,
        VKCLogger* const UNUSED(parent_vkclgr),
        const CharLogger* const parent_charlgr)
{
    if(!first_call) return ;

    WindowUtility::ForegroundInfo fginfo ;

    const auto delta = ResizeWindowCommon::compute_resize_delta(
            repeat_num, parent_charlgr, "window_width_delta") ;

    auto width = ScreenMetrics::width(fginfo.rect) - delta ;
    if(width <= 0) { 
        VirtualCmdLine::msgout("e: Width below zero") ;
        return ;
    }

    WindowUtility::resize(
            fginfo.hwnd, fginfo.rect.left, fginfo.rect.top,
            width, ScreenMetrics::height(fginfo.rect)) ;
}

//ResizeWindowHeight
const std::string ResizeWindowHeight::sname() noexcept
{
    return "resize_window_height" ;
}

void ResizeWindowHeight::sprocess(
        const bool first_call,
        const unsigned int UNUSED(repeat_num),
        VKCLogger* const UNUSED(parent_vkclgr),
        const CharLogger* const parent_charlgr)
{
    if(!first_call) return ;

    WindowUtility::ForegroundInfo fginfo ;

    auto str = parent_charlgr->to_str() ;
    if(str.empty()) return ;

    auto height = static_cast<LONG>(KeyLoggerUtility::extract_num(str)) ;

    ScreenMetrics::MonitorInfo minfo ;
    ScreenMetrics::get_monitor_metrics(fginfo.hwnd, minfo) ;

    auto max_height = minfo.work_rect.bottom - fginfo.rect.top ;
    if(max_height < height) {
        height = max_height ;
    }

    WindowUtility::resize(
            fginfo.hwnd, fginfo.rect.left, fginfo.rect.top,
            ScreenMetrics::width(fginfo.rect), height) ;
}

//IncreaseWindowHeight
const std::string IncreaseWindowHeight::sname() noexcept
{
    return "increase_window_height" ;
}

void IncreaseWindowHeight::sprocess(
        const bool first_call,
        const unsigned int repeat_num,
        VKCLogger* const UNUSED(parent_vkclgr),
        const CharLogger* const parent_charlgr)
{
    if(!first_call) return ;

    WindowUtility::ForegroundInfo fginfo ;

    auto height = ScreenMetrics::height(fginfo.rect) ;
    height += ResizeWindowCommon::compute_resize_delta(
            repeat_num, parent_charlgr, "window_height_delta") ;

    ScreenMetrics::MonitorInfo minfo ;
    ScreenMetrics::get_monitor_metrics(fginfo.hwnd, minfo) ;

    auto max_height = minfo.work_rect.bottom - fginfo.rect.top ;
    if(max_height < height) {
        height = max_height ;
    }

    WindowUtility::resize(
            fginfo.hwnd, fginfo.rect.left, fginfo.rect.top,
            ScreenMetrics::width(fginfo.rect), height) ;
}

//DecreaseWindowHeight
const std::string DecreaseWindowHeight::sname() noexcept
{
    return "decrease_window_height" ;
}

void DecreaseWindowHeight::sprocess(
        const bool first_call,
        const unsigned int repeat_num,
        VKCLogger* const UNUSED(parent_vkclgr),
        const CharLogger* const parent_charlgr)
{
    if(!first_call) return ;

    WindowUtility::ForegroundInfo fginfo ;

    const auto delta = ResizeWindowCommon::compute_resize_delta(
            repeat_num, parent_charlgr, "window_height_delta") ;

    auto height = ScreenMetrics::height(fginfo.rect) - delta ;
    if(height <= 0) { 
        VirtualCmdLine::msgout("e: Height below zero") ;
        return ;
    }

    WindowUtility::resize(
            fginfo.hwnd, fginfo.rect.left, fginfo.rect.top,
            ScreenMetrics::width(fginfo.rect), height) ;
}
