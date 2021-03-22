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


    //
    // Resize the width of a foreground window in a foreground monitor.
    // [1] Desired width
    // [2] Foreground Window Structure
    //
    inline static void resize_in_monitor_width(
            LONG desired_width,
            const WindowUtility::ForegroundInfo& fginfo) {

        ScreenMetrics::MonitorInfo minfo ;
        ScreenMetrics::get_monitor_metrics(fginfo.hwnd, minfo) ;

        const auto window_height = ScreenMetrics::height(fginfo.rect) ;
        const auto monitor_width = ScreenMetrics::width(minfo.work_rect) ;

        if(desired_width >= monitor_width) {
            //a maximum width is a width of a foreground monitor.
            WindowUtility::resize(
                    fginfo.hwnd,
                    minfo.work_rect.left, fginfo.rect.top,
                    monitor_width, window_height) ;
        }
        else {
            //If cannot move a lower-right point anymore, it move a upper-left point.
            auto right_max_width = minfo.work_rect.right - fginfo.rect.left ;

            if(desired_width <= right_max_width) {
                WindowUtility::resize(
                        fginfo.hwnd, fginfo.rect.left, fginfo.rect.top,
                        desired_width, window_height) ;
            }
            else {
                auto left_shift_delta = desired_width - right_max_width ;
                WindowUtility::resize(
                        fginfo.hwnd, fginfo.rect.left - left_shift_delta, fginfo.rect.top,
                        desired_width, window_height) ;
            }
        }
    }

    // Resize the height of a foreground window in a foreground monitor.
    // [1] Desired height
    // [2] Foreground Window Structure
    //
    inline static void resize_in_monitor_height(
            LONG desired_height,
            const WindowUtility::ForegroundInfo& fginfo) {

        ScreenMetrics::MonitorInfo minfo ;
        ScreenMetrics::get_monitor_metrics(fginfo.hwnd, minfo) ;

        const auto window_width = ScreenMetrics::width(fginfo.rect) ;
        const auto monitor_height = ScreenMetrics::height(minfo.work_rect) ;

        if(desired_height >= monitor_height) {
            //a maximum height is a height of a foreground monitor.
            WindowUtility::resize(
                    fginfo.hwnd,
                    fginfo.rect.left, minfo.work_rect.top,
                    window_width, monitor_height) ;
        }
        else {
            //If cannot move a lower-right point anymore, it move a upper-left point.
            auto lower_max_height = minfo.work_rect.bottom - fginfo.rect.top ;

            if(desired_height <= lower_max_height) {
                WindowUtility::resize(
                        fginfo.hwnd, fginfo.rect.left, fginfo.rect.top,
                        window_width, desired_height) ;
            }
            else {
                auto left_shift_delta = desired_height - lower_max_height ;
                WindowUtility::resize(
                        fginfo.hwnd, fginfo.rect.left, fginfo.rect.top - left_shift_delta,
                        window_width, desired_height) ;
            }
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

    ResizeWindowCommon::resize_in_monitor_width(width, fginfo) ;
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

    auto width = ScreenMetrics::width(fginfo.rect) + \
        ResizeWindowCommon::compute_resize_delta(
            repeat_num, parent_charlgr, "window_width_delta") ;

    ResizeWindowCommon::resize_in_monitor_width(width, fginfo) ;
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

    ResizeWindowCommon::resize_in_monitor_height(height, fginfo) ;
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

    auto height = ScreenMetrics::height(fginfo.rect) + \
        ResizeWindowCommon::compute_resize_delta(
            repeat_num, parent_charlgr, "window_height_delta") ;

    ResizeWindowCommon::resize_in_monitor_height(height, fginfo) ;
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
