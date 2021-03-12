#include "resize_window.hpp"

#include <windows.h>
#include <psapi.h>

#include <functional>
#include <map>
#include <unordered_map>

#include "i_params.hpp"
#include "jump_cursor.hpp"
#include "keybrd_eventer.hpp"
#include "msg_logger.hpp"
#include "screen_metrics.hpp"
#include "utility.hpp"
#include "virtual_cmd_line.hpp"
#include "window_ctrl.hpp"

//MaximizeCurrentWindow
const std::string MaximizeCurrentWindow::sname() noexcept
{
    return "maximize_current_window" ;
}

void MaximizeCurrentWindow::sprocess(
        const bool first_call,
        const unsigned int repeat_num,
        VKCLogger* const UNUSED(parent_vkclgr),
        const CharLogger* const UNUSED(parent_charlgr))
{
    if(!first_call) return ;
    for(unsigned int i = 0 ; i < repeat_num ; i ++) {
        KeybrdEventer::pushup(VKC_LWIN, VKC_UP) ;
    }
}


//MinimizeCurrentWindow
const std::string MinimizeCurrentWindow::sname() noexcept
{
    return "minimize_current_window" ;
}

void MinimizeCurrentWindow::sprocess(
        const bool first_call,
        const unsigned int UNUSED(repeat_num),
        VKCLogger* const UNUSED(parent_vkclgr),
        const CharLogger* const UNUSED(parent_charlgr))
{
    if(!first_call) return ;
    KeybrdEventer::pushup(VKC_LWIN, VKC_DOWN) ;
}

namespace ResizeWindow {
    inline static void resize(
            HWND hwnd,
            LONG left,
            LONG top,
            LONG width,
            LONG height) {

        if(!MoveWindow(hwnd, left, top, width, height, TRUE)) {
            throw RUNTIME_EXCEPT("Could not change window size") ;
        }

        RECT rect ;
        if(!GetWindowRect(hwnd, &rect)) {
            throw RUNTIME_EXCEPT("Could not get a rectangle of a window.") ;
        }

        if(ScreenMetrics::width(rect) != width || ScreenMetrics::height(rect) != height) {
            //If a window is Chromium browser (e.g. GoogleChrome or Microsoft Edge) and when it is full screen,
            //could not resize its size, so cancel full screen.

            ScreenMetrics::MonitorInfo minfo ;
            ScreenMetrics::get_monitor_metrics(hwnd, minfo) ;

            //Whether it is a full screen ?
            if(!ScreenMetrics::is_bigger_than(rect, minfo.work_rect)) {
                return ;
            }

            if(!SetForegroundWindow(hwnd)) {
                throw RUNTIME_EXCEPT("Could not set a foreground window.") ;
            }

            //minimize it once
            KeybrdEventer::pushup(VKC_LWIN, VKC_DOWN) ;
            Sleep(50) ; //50ms

            if(!MoveWindow(hwnd, left, top, width, height, TRUE)) {
                throw RUNTIME_EXCEPT("Could not change window size in twice.") ;
            }
        }

        Jump2ActiveWindow::sprocess(true, 1, nullptr, nullptr) ;
    }

    inline static void snap_foreground_window(
            const std::function<void(RECT&, const RECT&)>& calc_half_size,
            const std::function<POINT(const RECT&)>& next_monitor_pos) {

        auto hwnd = GetForegroundWindow() ;
        if(hwnd == NULL) {
            throw RUNTIME_EXCEPT("There is not the foreground window.") ;
        }

        ScreenMetrics::MonitorInfo minfo ;
        ScreenMetrics::get_monitor_metrics(hwnd, minfo) ;

        RECT half_rect ;
        calc_half_size(half_rect, minfo.work_rect) ;

        RECT cur_rect ;
        if(!GetWindowRect(hwnd, &cur_rect)) {
            throw RUNTIME_EXCEPT("Could not get a rectangle of a foreground window.") ;
        }

        if(ScreenMetrics::is_equel(cur_rect, half_rect)) {
            ScreenMetrics::get_monitor_metrics(next_monitor_pos(minfo.rect), minfo) ;

            calc_half_size(half_rect, minfo.work_rect) ;
        }

        ResizeWindow::resize(
                hwnd,
                half_rect.left,
                half_rect.top,
                ScreenMetrics::width(half_rect),
                ScreenMetrics::height(half_rect)) ;
    }
}

//SnapCurrentWindow2Left
const std::string SnapCurrentWindow2Left::sname() noexcept
{
    return "snap_current_window_to_left" ;
}

void SnapCurrentWindow2Left::sprocess(
        const bool first_call,
        const unsigned int UNUSED(repeat_num),
        VKCLogger* const UNUSED(parent_vkclgr),
        const CharLogger* const UNUSED(parent_charlgr))
{
    if(!first_call) return ;

    auto calc_half_size = [] (RECT& rect, const RECT& mrect) {
        rect.left   = mrect.left ;
        rect.top    = mrect.top ;
        rect.right  = rect.left + ScreenMetrics::width(mrect) / 2 ;
        rect.bottom = mrect.bottom ;
    } ;

    auto next_monitor_pos = [] (const RECT& rect) {
        return POINT{rect.left - 100, ScreenMetrics::center_y(rect)} ;
    } ;

    ResizeWindow::snap_foreground_window(calc_half_size, next_monitor_pos) ;
}


//SnapCurrentWindow2Right
const std::string SnapCurrentWindow2Right::sname() noexcept
{
    return "snap_current_window_to_right" ;
}

void SnapCurrentWindow2Right::sprocess(
        const bool first_call,
        const unsigned int UNUSED(repeat_num),
        VKCLogger* const UNUSED(parent_vkclgr),
        const CharLogger* const UNUSED(parent_charlgr))
{
    if(!first_call) return ;

    auto calc_half_size = [] (RECT& rect, const RECT& mrect) {
        rect.left   = mrect.left + ScreenMetrics::width(mrect) / 2 ;
        rect.top    = mrect.top ;
        rect.right  = mrect.right ;
        rect.bottom = mrect.bottom ;
    } ;

    auto next_monitor_pos = [] (const RECT& rect) {
        return POINT{rect.right + 100, ScreenMetrics::center_y(rect)} ;
    } ;

    ResizeWindow::snap_foreground_window(calc_half_size, next_monitor_pos) ;
}


//SnapCurrentWindow2Top
const std::string SnapCurrentWindow2Top::sname() noexcept
{
    return "snap_current_window_to_top" ;
}

void SnapCurrentWindow2Top::sprocess(
        const bool first_call,
        const unsigned int UNUSED(repeat_num),
        VKCLogger* const UNUSED(parent_vkclgr),
        const CharLogger* const UNUSED(parent_charlgr))
{
    if(!first_call) return ;

    auto calc_half_size = [] (RECT& rect, const RECT& mrect) {
        rect.left   = mrect.left ;
        rect.top    = mrect.top ;
        rect.right  = mrect.right ;
        rect.bottom = rect.top + ScreenMetrics::height(mrect) / 2 ;
    } ;

    auto next_monitor_pos = [] (const RECT& rect) {
        return POINT{ScreenMetrics::center_x(rect), rect.top - 100} ;
    } ;

    ResizeWindow::snap_foreground_window(calc_half_size, next_monitor_pos) ;
}

//SnapCurrentWindow2Bottom
const std::string SnapCurrentWindow2Bottom::sname() noexcept
{
    return "snap_current_window_to_bottom" ;
}

void SnapCurrentWindow2Bottom::sprocess(
        const bool first_call,
        const unsigned int UNUSED(repeat_num),
        VKCLogger* const UNUSED(parent_vkclgr),
        const CharLogger* const UNUSED(parent_charlgr))
{
    if(!first_call) return ;

    auto calc_half_size = [] (RECT& rect, const RECT& mrect) {
        rect.left   = mrect.left ;
        rect.top    = mrect.top + ScreenMetrics::height(mrect) / 2 ;
        rect.right  = mrect.right ;
        rect.bottom = mrect.bottom ;
    } ;

    auto next_monitor_pos = [] (const RECT& rect) {
        return POINT{ScreenMetrics::center_x(rect), rect.bottom + 100} ;
    } ;

    ResizeWindow::snap_foreground_window(calc_half_size, next_monitor_pos) ;
}


namespace ResizeWindow
{
    static std::unordered_map<HMONITOR, RECT> g_mrects ;

    using ordered_hwnd_t = std::map<SIZE_T, HWND> ;
    static std::unordered_map<HMONITOR, ordered_hwnd_t> g_m_ordered_hwnd ;

    static BOOL CALLBACK EnumWindowsProcForArrangement(HWND hwnd, LPARAM UNUSED(lparam)) {
        if(!WindowCtrl::is_visible_hwnd(hwnd)) {
            return TRUE ; //continue
        }

        RECT rect ;
        if(!GetWindowRect(hwnd, &rect)) {
            return TRUE ; //continue
        }

        if(!WindowCtrl::is_window_mode(hwnd, rect)) {
            return TRUE ; //continue
        }

        //Is existed in work area?
        ScreenMetrics::MonitorInfo minfo ;
        ScreenMetrics::get_monitor_metrics(hwnd, minfo) ;
        if(ScreenMetrics::is_out_of_range(rect, minfo.work_rect)) {
            return TRUE ;
        }

        // make a unique identifier and its priority
        DWORD proc_id = 0 ;
        GetWindowThreadProcessId(hwnd, &proc_id) ;

        HANDLE hproc = OpenProcess(
                PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
                FALSE, proc_id) ;
        if(!hproc) {
            return TRUE ; //has not permission
        }

        PROCESS_MEMORY_COUNTERS pmc ;
        if(!GetProcessMemoryInfo(hproc, &pmc, sizeof(pmc))) {
            CloseHandle(hproc) ;
            std::stringstream ss ;
            ss << "Could not get the memory infomation of " << hwnd ;
            ERROR_PRINT(ss.str()) ;
            return FALSE ; //break
        }
        CloseHandle(hproc) ;

        //a key is unique and described the priority.
        auto& ordered_hwnd = g_m_ordered_hwnd[minfo.hmonitor] ;
        ordered_hwnd[pmc.WorkingSetSize + ordered_hwnd.size()] = hwnd ;

        g_mrects[minfo.hmonitor] = std::move(minfo.work_rect) ;

        return TRUE ;
    }

    inline static void assign_local_area_in_monitors(std::unordered_map<HWND, RECT>& rects) {
        for(auto& mr : g_mrects) {
            const auto& hmonitor = mr.first ;
            const auto& mrect    = mr.second ;

            const auto& ordered_hwnd = g_m_ordered_hwnd[hmonitor] ;

            //Its priority is the highest (based on the memory use).
            auto itr = ordered_hwnd.crbegin() ;

            auto pre_hwnd = itr->second ;
            rects[pre_hwnd] = mrect ;

            itr ++ ;

            while(itr != ordered_hwnd.crend()) {
                const auto hwnd = itr->second ;

                auto& pre_rect = rects[pre_hwnd] ;
                auto rect = pre_rect ;

                const auto pre_w = ScreenMetrics::width(pre_rect) ;
                const auto pre_h = ScreenMetrics::height(pre_rect) ;
                if(pre_w > pre_h) {
                    pre_rect.right -= pre_w / 2 ;
                    rect.left      += pre_w / 2 ;
                }
                else {
                    pre_rect.bottom -= pre_h / 2 ;
                    rect.top        += pre_h / 2 ;
                }
                rects[hwnd] = std::move(rect) ;

                pre_hwnd = hwnd ;
                itr ++ ;
            }
        }
    }

    inline static void batch_resize(const std::unordered_map<HWND, RECT>& rects) {
        //Resize each windows
        for(const auto& hr : rects) {
            const auto hwnd = hr.first ;
            const auto rect = hr.second ;
            resize(hwnd, rect.left, rect.top,
                    ScreenMetrics::width(rect), ScreenMetrics::height(rect)) ;
        }
    }
}

//ArrangeWindows
const std::string ArrangeWindows::sname() noexcept
{
    return "arrange_windows" ;
}

void ArrangeWindows::sprocess(
        const bool first_call,
        const unsigned int UNUSED(repeat_num),
        VKCLogger* const UNUSED(parent_vkclgr),
        const CharLogger* const UNUSED(parent_charlgr))
{
    if(!first_call) return ;

    auto hwnd = GetForegroundWindow() ;
    if(hwnd == NULL) {
        throw RUNTIME_EXCEPT("There is not a foreground window.") ;
    }

    //Search visible windows
    using namespace ResizeWindow ;
    g_m_ordered_hwnd.clear() ;
    g_mrects.clear() ;

    if(!EnumWindows(EnumWindowsProcForArrangement, 0)) {
        throw RUNTIME_EXCEPT("Could not enumerate all top-level windows on the screen.") ;
    }

    if(g_m_ordered_hwnd.empty() || g_mrects.empty()) {
        return ;
    }

    std::unordered_map<HWND, RECT> rects ;
    assign_local_area_in_monitors(rects) ;
    batch_resize(rects) ;

    if(!SetForegroundWindow(hwnd)) {
        throw RUNTIME_EXCEPT("Could not set the foreground window.") ;
    }
}

namespace ResizeWindow
{
    inline static void change_order_of_arranged_windows(const std::function<void(ordered_hwnd_t&)>& sort_proc) {
        POINT pos ;
        if(!GetCursorPos(&pos)) {
            throw RUNTIME_EXCEPT("Could not get a position of a mouse cursor.") ;
        }

        const auto hmonitor = MonitorFromPoint(pos, MONITOR_DEFAULTTONEAREST) ;
        MONITORINFO minfo ;
        minfo.cbSize = sizeof(MONITORINFO) ;
        if(!GetMonitorInfo(hmonitor, &minfo)) {
            throw RUNTIME_EXCEPT("Could not get monitor infomation.") ;
        }

        if(g_m_ordered_hwnd.empty()) {
            return ;
        }

        //rotate-shift a value in hwnd maps
        auto& ordered_hwnd = g_m_ordered_hwnd.at(hmonitor) ;
        if(ordered_hwnd.empty()) {
            return ;
        }

        sort_proc(ordered_hwnd) ;

        std::unordered_map<HWND, RECT> rects ;
        assign_local_area_in_monitors(rects) ;
        batch_resize(rects) ;
    }
}

//RotateWindow
const std::string RotateWindows::sname() noexcept
{
    return "rotate_windows" ;
}

void RotateWindows::sprocess(
        const bool first_call,
        const unsigned int repeat_num,
        VKCLogger* const UNUSED(parent_vkclgr),
        const CharLogger* const UNUSED(parent_charlgr))
{
    if(!first_call) return ;

    auto sort = [repeat_num] (ResizeWindow::ordered_hwnd_t& ordered_hwnd) {
        for(unsigned int i = 0 ; i < repeat_num ; i ++) {
            auto itr     = ordered_hwnd.rbegin() ;
            auto pre_itr = itr ;
            const auto head_hwnd = itr->second ;
            itr ++ ;
            while(itr != ordered_hwnd.rend()) {
                pre_itr->second = itr->second ;
                pre_itr = itr ;
                itr ++ ;
            }
            pre_itr->second = head_hwnd ;
        }
    } ;

    ResizeWindow::change_order_of_arranged_windows(sort) ;
}

//RotateWindowsInReverse
const std::string RotateWindowsInReverse::sname() noexcept
{
    return "rotate_windows_in_reverse" ;
}

void RotateWindowsInReverse::sprocess(
        const bool first_call,
        const unsigned int repeat_num,
        VKCLogger* const UNUSED(parent_vkclgr),
        const CharLogger* const UNUSED(parent_charlgr))
{
    if(!first_call) return ;

    auto sort = [repeat_num] (ResizeWindow::ordered_hwnd_t& ordered_hwnd) {
        for(unsigned int i = 0 ; i < repeat_num ; i ++) {
            auto itr     = ordered_hwnd.begin() ;
            auto pre_itr = itr ;
            const auto head_hwnd = itr->second ;
            itr ++ ;
            while(itr != ordered_hwnd.end()) {
                pre_itr->second = itr->second ;
                pre_itr = itr ;
                itr ++ ;
            }
            pre_itr->second = head_hwnd ;
        }
    } ;

    ResizeWindow::change_order_of_arranged_windows(sort) ;
}

namespace ResizeWindow
{
    static std::map<LONG, HWND> g_near_hwnds ;

    struct ForegroundInfo {
        HWND hwnd ;
        HMONITOR hmonitor ;
        RECT rect ;

        explicit ForegroundInfo()
        : hwnd(NULL),
          hmonitor(NULL),
          rect{0, 0, 0, 0}
        {
            hwnd = GetForegroundWindow() ;
            if(hwnd == NULL) {
                throw RUNTIME_EXCEPT("Could not get a position of a mouse cursor.") ;
            }

            hmonitor = MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST) ;

            if(!GetWindowRect(hwnd, &rect)) {
                throw RUNTIME_EXCEPT("Could not get a rectangle of a foreground window.") ;
            }
        }
    } ;

    static BOOL CALLBACK EnumWindowsProcForExchange(HWND hwnd, LPARAM lparam) {
        const auto fginfo = reinterpret_cast<ForegroundInfo*>(lparam) ;

        if(fginfo->hwnd == hwnd) {
            return TRUE ;
        }

        if(!WindowCtrl::is_visible_hwnd(hwnd)) {
            return TRUE ; //continue
        }

        RECT rect ;
        if(!GetWindowRect(hwnd, &rect)) {
            return TRUE ;
        }

        if(!WindowCtrl::is_window_mode(hwnd, rect)) {
            return TRUE ; //continue
        }

        //Is in range of work area
        ScreenMetrics::MonitorInfo minfo ;
        ScreenMetrics::get_monitor_metrics(hwnd, minfo) ;
        if(ScreenMetrics::is_out_of_range(rect, minfo.work_rect)) {
            return TRUE ;
        }

        g_near_hwnds[ScreenMetrics::l2_distance_nosq(rect, fginfo->rect) / 100] = hwnd ;
        return TRUE ;
    }
}

//ExchangeWindowWithNextOne
const std::string ExchangeWindowWithNextOne::sname() noexcept
{
    return "exchange_window_with_next_one" ;
}

void ExchangeWindowWithNextOne::sprocess(
        const bool first_call,
        const unsigned int UNUSED(repeat_num),
        VKCLogger* const UNUSED(parent_vkclgr),
        const CharLogger* const UNUSED(parent_charlgr))
{
    if(!first_call) return ;

    using namespace ResizeWindow ;

    g_near_hwnds.clear() ;

    ForegroundInfo fginfo ;
    if(!EnumWindows(EnumWindowsProcForExchange, reinterpret_cast<LPARAM>(&fginfo))) {
        throw RUNTIME_EXCEPT("Could not enumerate all top-level windows on the screen.") ;
    }

    if(g_near_hwnds.empty()) {
        return ;
    }

    auto nearest_hwnd = g_near_hwnds.begin()->second ;
    RECT nearest_rect ;
    if(!GetWindowRect(nearest_hwnd, &nearest_rect)) {
        throw RUNTIME_EXCEPT("Could not get a rectangle of the nearest window.") ;
    }

    ResizeWindow::resize(
            nearest_hwnd,
            fginfo.rect.left,
            fginfo.rect.top,
            ScreenMetrics::width(fginfo.rect),
            ScreenMetrics::height(fginfo.rect)) ;

    ResizeWindow::resize(
            fginfo.hwnd,
            nearest_rect.left,
            nearest_rect.top,
            ScreenMetrics::width(nearest_rect),
            ScreenMetrics::height(nearest_rect)) ;
}


//OpenNewCurWinWithHorizontalSplit
const std::string OpenNewCurWinWithHorizontalSplit::sname() noexcept
{
    return "open_new_current_window_with_hsplit" ;
}

void OpenNewCurWinWithHorizontalSplit::sprocess(
        const bool first_call,
        const unsigned int UNUSED(repeat_num),
        VKCLogger* const UNUSED(parent_vkclgr),
        const CharLogger* const UNUSED(parent_charlgr))
{
    if(!first_call) return ;

    ResizeWindow::ForegroundInfo fginfo ;

    const auto w = ScreenMetrics::width(fginfo.rect) ;
    const auto h = ScreenMetrics::height(fginfo.rect) ;

    //snap a original window to top
    ResizeWindow::resize(
            fginfo.hwnd,
            fginfo.rect.left, fginfo.rect.top,
            w, h / 2) ;
    OpenNewCurrentWindow::sprocess(true, 1, nullptr, nullptr) ;

    auto new_hwnd = GetForegroundWindow() ;
    if(new_hwnd == NULL) {
        throw RUNTIME_EXCEPT("There is not the foreground window.") ;
    }
    if(new_hwnd == fginfo.hwnd) {
        Sleep(500) ;
        new_hwnd = GetForegroundWindow() ;
    }

    //snap a new window to bottom
    ResizeWindow::resize(
            new_hwnd,
            fginfo.rect.left, fginfo.rect.top + h / 2,
            w, h / 2) ;
}


//OpenNewCurWinWithVerticalSplit
const std::string OpenNewCurWinWithVerticalSplit::sname() noexcept
{
    return "open_new_current_window_with_vsplit" ;
}

void OpenNewCurWinWithVerticalSplit::sprocess(
        const bool first_call,
        const unsigned int UNUSED(repeat_num),
        VKCLogger* const UNUSED(parent_vkclgr),
        const CharLogger* const UNUSED(parent_charlgr))
{
    if(!first_call) return ;

    ResizeWindow::ForegroundInfo fginfo ;

    const auto w = ScreenMetrics::width(fginfo.rect) ;
    const auto h = ScreenMetrics::height(fginfo.rect) ;

    //snap a original window to left
    ResizeWindow::resize(
            fginfo.hwnd,
            fginfo.rect.left, fginfo.rect.top,
            w / 2, h) ;
    OpenNewCurrentWindow::sprocess(true, 1, nullptr, nullptr) ;

    auto new_hwnd = GetForegroundWindow() ;
    if(new_hwnd == NULL) {
        throw RUNTIME_EXCEPT("There is not the foreground window.") ;
    }
    if(new_hwnd == fginfo.hwnd) {
        Sleep(500) ;
        new_hwnd = GetForegroundWindow() ;
    }

    //snap a new window to right
    ResizeWindow::resize(
            new_hwnd,
            fginfo.rect.left + w / 2, fginfo.rect.top,
            w / 2, h) ;
}

namespace ResizeWindow
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

    ResizeWindow::ForegroundInfo fginfo ;

    auto str = parent_charlgr->to_str() ;
    if(str.empty()) return ;

    auto width = static_cast<LONG>(KeyLoggerUtility::extract_num(str)) ;

    ScreenMetrics::MonitorInfo minfo ;
    ScreenMetrics::get_monitor_metrics(fginfo.hwnd, minfo) ;

    auto max_width = minfo.work_rect.right - fginfo.rect.left ;
    if(max_width < width) {
        width = max_width ;
    }

    ResizeWindow::resize(
            fginfo.hwnd, fginfo.rect.left, fginfo.rect.top,
            width, ScreenMetrics::height(fginfo.rect)) ;
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

    ResizeWindow::ForegroundInfo fginfo ;

    auto width = ScreenMetrics::width(fginfo.rect) ;
    width += ResizeWindow::compute_resize_delta(
            repeat_num, parent_charlgr, "window_width_delta") ;

    ScreenMetrics::MonitorInfo minfo ;
    ScreenMetrics::get_monitor_metrics(fginfo.hwnd, minfo) ;

    auto max_width = minfo.work_rect.right - fginfo.rect.left ;
    if(max_width < width) {
        width = max_width ;
    }

    ResizeWindow::resize(
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

    ResizeWindow::ForegroundInfo fginfo ;

    const auto delta = ResizeWindow::compute_resize_delta(
            repeat_num, parent_charlgr, "window_width_delta") ;

    auto width = ScreenMetrics::width(fginfo.rect) - delta ;
    if(width <= 0) { 
        VirtualCmdLine::msgout("e: Width below zero") ;
        return ;
    }

    ResizeWindow::resize(
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

    ResizeWindow::ForegroundInfo fginfo ;

    auto str = parent_charlgr->to_str() ;
    if(str.empty()) return ;

    auto height = static_cast<LONG>(KeyLoggerUtility::extract_num(str)) ;

    ScreenMetrics::MonitorInfo minfo ;
    ScreenMetrics::get_monitor_metrics(fginfo.hwnd, minfo) ;

    auto max_height = minfo.work_rect.bottom - fginfo.rect.top ;
    if(max_height < height) {
        height = max_height ;
    }

    ResizeWindow::resize(
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

    ResizeWindow::ForegroundInfo fginfo ;

    auto height = ScreenMetrics::height(fginfo.rect) ;
    height += ResizeWindow::compute_resize_delta(
            repeat_num, parent_charlgr, "window_height_delta") ;

    ScreenMetrics::MonitorInfo minfo ;
    ScreenMetrics::get_monitor_metrics(fginfo.hwnd, minfo) ;

    auto max_height = minfo.work_rect.bottom - fginfo.rect.top ;
    if(max_height < height) {
        height = max_height ;
    }

    ResizeWindow::resize(
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

    ResizeWindow::ForegroundInfo fginfo ;

    const auto delta = ResizeWindow::compute_resize_delta(
            repeat_num, parent_charlgr, "window_height_delta") ;

    auto height = ScreenMetrics::height(fginfo.rect) - delta ;
    if(height <= 0) { 
        VirtualCmdLine::msgout("e: Height below zero") ;
        return ;
    }

    ResizeWindow::resize(
            fginfo.hwnd, fginfo.rect.left, fginfo.rect.top,
            ScreenMetrics::width(fginfo.rect), height) ;
}
