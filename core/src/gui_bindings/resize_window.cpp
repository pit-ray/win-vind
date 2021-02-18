#include "resize_window.hpp"

#include <windows.h>
#include <psapi.h>

#include <functional>
#include <map>
#include <unordered_map>

#include "i_params.hpp"
#include "jump_cursor.hpp"
#include "key_logger.hpp"
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
        KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr))
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
        KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr))
{
    if(!first_call) return ;
    KeybrdEventer::pushup(VKC_LWIN, VKC_DOWN) ;
}

namespace ResizeWindow {
    inline static void resize_window(HWND hwnd, LONG left, LONG top, LONG width, LONG height) {
        if(hwnd == NULL) {
            throw RUNTIME_EXCEPT("There is not the foreground window.") ;
        }

        if(!MoveWindow(hwnd, left, top, width, height, TRUE)) {
            throw RUNTIME_EXCEPT("Could not change window size") ;
        }

        RECT rect ;
        if(!GetWindowRect(hwnd, &rect)) {
            throw RUNTIME_EXCEPT("Could not get a rectangle of a window.") ;
        }

        if(ScreenMetrics::width(rect) != width || ScreenMetrics::height(rect) != height) {

            RECT mrect ;
            RECT mrect_work ;
            ScreenMetrics::get_monitor_metrics(hwnd, &mrect, &mrect_work) ;

            if(ScreenMetrics::is_fully_in_range(rect, mrect_work)) {
                return ;
            }

            //If a window is Chromium browser (e.g. GoogleChrome or Microsoft Edge) and when it is full screen,
            //could not resize its size, so cancel full screen.
            if(!SetForegroundWindow(hwnd)) {
                throw RUNTIME_EXCEPT("Could not set a foreground window.") ;
            }

            //minimize once
            KeybrdEventer::pushup(VKC_LWIN, VKC_DOWN) ;
            Sleep(50) ;

            if(!MoveWindow(hwnd, left, top, width, height, TRUE)) {
                throw RUNTIME_EXCEPT("Could not change window size in twice.") ;
            }
        }

        Jump2ActiveWindow::sprocess(true, 1, nullptr, nullptr) ;
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
        KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr))
{
    if(!first_call) return ;

    auto hwnd = GetForegroundWindow() ;
    if(hwnd == NULL) {
        throw RUNTIME_EXCEPT("There is not the foreground window.") ;
    }

    RECT rect ;
    RECT rect_work ;
    ScreenMetrics::get_monitor_metrics(hwnd, &rect, &rect_work) ;

    ResizeWindow::resize_window(
            hwnd,
            rect_work.left,
            rect_work.top,
            ScreenMetrics::width(rect_work) / 2,
            ScreenMetrics::height(rect_work)) ;
}


//SnapCurrentWindow2Right
const std::string SnapCurrentWindow2Right::sname() noexcept
{
    return "snap_current_window_to_right" ;
}

void SnapCurrentWindow2Right::sprocess(
        const bool first_call,
        const unsigned int UNUSED(repeat_num),
        KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr))
{
    if(!first_call) return ;

    auto hwnd = GetForegroundWindow() ;
    if(hwnd == NULL) {
        throw RUNTIME_EXCEPT("There is not the foreground window.") ;
    }

    RECT rect ;
    RECT rect_work ;
    ScreenMetrics::get_monitor_metrics(hwnd, &rect, &rect_work) ;

    const auto half_of_width = ScreenMetrics::width(rect_work) / 2 ;
    ResizeWindow::resize_window(
            hwnd,
            rect_work.left + half_of_width,
            rect_work.top,
            half_of_width,
            ScreenMetrics::height(rect_work)) ;
}


//SnapCurrentWindow2Top
const std::string SnapCurrentWindow2Top::sname() noexcept
{
    return "snap_current_window_to_top" ;
}

void SnapCurrentWindow2Top::sprocess(
        const bool first_call,
        const unsigned int UNUSED(repeat_num),
        KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr))
{
    if(!first_call) return ;

    auto hwnd = GetForegroundWindow() ;
    if(hwnd == NULL) {
        throw RUNTIME_EXCEPT("There is not the foreground window.") ;
    }

    RECT rect ;
    RECT rect_work ;
    ScreenMetrics::get_monitor_metrics(hwnd, &rect, &rect_work) ;

    ResizeWindow::resize_window(
            hwnd,
            rect_work.left,
            rect_work.top,
            ScreenMetrics::width(rect_work),
            ScreenMetrics::height(rect_work) / 2) ;
}

//SnapCurrentWindow2Bottom
const std::string SnapCurrentWindow2Bottom::sname() noexcept
{
    return "snap_current_window_to_bottom" ;
}

void SnapCurrentWindow2Bottom::sprocess(
        const bool first_call,
        const unsigned int UNUSED(repeat_num),
        KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr))
{
    if(!first_call) return ;

    auto hwnd = GetForegroundWindow() ;
    if(hwnd == NULL) {
        throw RUNTIME_EXCEPT("There is not the foreground window.") ;
    }

    RECT rect ;
    RECT rect_work ;
    ScreenMetrics::get_monitor_metrics(hwnd, &rect, &rect_work) ;

    const auto half_of_height = ScreenMetrics::height(rect_work) / 2 ;
    ResizeWindow::resize_window(
            hwnd,
            rect_work.left,
            rect_work.top + half_of_height,
            ScreenMetrics::width(rect_work),
            half_of_height) ;
}


namespace ResizeWindow
{
    static std::unordered_map<HMONITOR, RECT> g_mrects ;
    using ordered_hwnd_t = std::map<SIZE_T, HWND> ;
    static std::unordered_map<HMONITOR, ordered_hwnd_t> g_ordered_hwnd ;

    static BOOL CALLBACK EnumWindowsProcForArrangement(HWND hwnd, LPARAM UNUSED(lparam)) {
        if(!WindowCtrl::is_valid_hwnd(hwnd)) {
            return TRUE ; //continue
        }

        RECT rect ;
        if(!GetWindowRect(hwnd, &rect)) {
            return TRUE ;
        }

        if(!WindowCtrl::is_valid_rect(hwnd, rect)) {
            return TRUE ; //continue
        }

        HMONITOR hmonitor ;
        RECT monitor_rect ;
        RECT monitor_rect_work ;
        ScreenMetrics::get_monitor_metrics(hwnd, &monitor_rect, &monitor_rect_work, &hmonitor) ;

        //Is in range of work area
        if(ScreenMetrics::is_out_of_range(rect, monitor_rect_work)) {
            return TRUE ;
        }

        DWORD proc_id = 0 ;
        GetWindowThreadProcessId(hwnd, &proc_id) ;
        HANDLE hproc = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, proc_id) ;
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
        auto& gh = g_ordered_hwnd[hmonitor] ;
        gh[pmc.WorkingSetSize + gh.size()] = hwnd ;
        g_mrects[hmonitor] = std::move(monitor_rect_work) ;

        return TRUE ;
    }

    inline static void assign_local_area_in_monitors(std::unordered_map<HWND, RECT>& rects) {
        for(auto& mr : g_mrects) {
            const auto& hmonitor = mr.first ;
            const auto& mrect    = mr.second ;

            const auto& oh = g_ordered_hwnd[hmonitor] ;

            //Its priority is the highest (based on the memory use).
            auto itr = oh.crbegin() ;
            auto pre_hwnd = itr->second ;
            rects[itr->second] = mrect ;
            itr ++ ;

            while(itr != oh.crend()) {
                const auto hwnd = itr->second ;

                auto& pre_rect = rects[pre_hwnd] ;
                auto rect = pre_rect ;

                const auto pre_w = ScreenMetrics::width(pre_rect) ;
                const auto pre_h = ScreenMetrics::height(pre_rect) ;
                if(pre_w > pre_h) {
                    pre_rect.right -= pre_w / 2 ;
                    rect.left += pre_w / 2 ;
                }
                else {
                    pre_rect.bottom -= pre_h / 2 ;
                    rect.top += pre_h / 2 ;
                }
                rects[hwnd] = std::move(rect) ;

                pre_hwnd = hwnd ;
                itr ++ ;
            }
        }
    }

    inline static void batch_resize(std::unordered_map<HWND, RECT>& rects) {
        //Resize each windows
        for(const auto& hr : rects) {
            const auto hwnd = hr.first ;
            const auto rect = hr.second ;
            resize_window(hwnd, rect.left, rect.top,
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
        KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr))
{
    if(!first_call) return ;

    auto hwnd = GetForegroundWindow() ;
    if(hwnd == NULL) {
        throw RUNTIME_EXCEPT("There is not a foreground window.") ;
    }

    //Search visible windows
    using namespace ResizeWindow ;
    g_ordered_hwnd.clear() ;
    g_mrects.clear() ;

    if(!EnumWindows(EnumWindowsProcForArrangement, 0)) {
        throw RUNTIME_EXCEPT("Could not enumerate all top-level windows on the screen.") ;
    }

    if(g_ordered_hwnd.empty() || g_mrects.empty()) {
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

        if(g_ordered_hwnd.empty()) {
            return ;
        }

        try {
            //rotate-shift a value in hwnd maps
            auto& oh = g_ordered_hwnd.at(hmonitor) ;
            if(oh.empty()) {
                return ;
            }

            sort_proc(oh) ;

            std::unordered_map<HWND, RECT> rects ;
            assign_local_area_in_monitors(rects) ;
            batch_resize(rects) ;
        }
        catch(const std::out_of_range&) {
            return ;
        }
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
        KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr))
{
    if(!first_call) return ;

    auto sort = [repeat_num] (ResizeWindow::ordered_hwnd_t& oh) {
        for(unsigned int i = 0 ; i < repeat_num ; i ++) {
            auto itr      = oh.rbegin() ;
            auto head_val = itr->second ;
            auto pre_itr  = itr ;
            itr ++ ;
            while(itr != oh.rend()) {
                pre_itr->second = itr->second ;
                pre_itr = itr ;
                itr ++ ;
            }
            pre_itr->second = head_val ;
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
        KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr))
{
    if(!first_call) return ;

    auto sort = [repeat_num] (ResizeWindow::ordered_hwnd_t& oh) {
        for(unsigned int i = 0 ; i < repeat_num ; i ++) {
            auto itr      = oh.begin() ;
            auto head_val = itr->second ;
            auto pre_itr  = itr ;
            itr ++ ;
            while(itr != oh.end()) {
                pre_itr->second = itr->second ;
                pre_itr = itr ;
                itr ++ ;
            }
            pre_itr->second = head_val ;
        }
    } ;

    ResizeWindow::change_order_of_arranged_windows(sort) ;
}

namespace ResizeWindow
{
    static std::map<LONG, HWND> g_near_hwnds ;

    static BOOL CALLBACK EnumWindowsProcForExchange(HWND hwnd, LPARAM lparam) {
        auto target_hwnd = reinterpret_cast<HWND>(lparam) ;

        if(target_hwnd == hwnd) {
            return TRUE ;
        }

        if(!WindowCtrl::is_valid_hwnd(hwnd)) {
            return TRUE ; //continue
        }

        RECT rect ;
        if(!GetWindowRect(hwnd, &rect)) {
            return TRUE ;
        }

        if(!WindowCtrl::is_valid_rect(hwnd, rect)) {
            return TRUE ; //continue
        }

        HMONITOR hmonitor ;
        RECT monitor_rect, monitor_rect_work ;
        ScreenMetrics::get_monitor_metrics(hwnd, &monitor_rect, &monitor_rect_work, &hmonitor) ;

        const auto target_hmonitor = MonitorFromWindow(target_hwnd, MONITOR_DEFAULTTONEAREST) ;
        if(hmonitor != target_hmonitor) {
            return TRUE ;
        }

        //Is in range of work area
        if(ScreenMetrics::is_out_of_range(rect, monitor_rect_work)) {
            return TRUE ;
        }

        RECT target_rect ;
        if(!GetWindowRect(target_hwnd, &target_rect)) {
            return TRUE ;
        }
        g_near_hwnds[ScreenMetrics::l2_distance_nosq(rect, target_rect) / 100] = hwnd ;
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
        KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr))
{
    if(!first_call) return ;

    using namespace ResizeWindow ;

    g_near_hwnds.clear() ;

    auto hwnd = GetForegroundWindow() ;
    if(hwnd == NULL) {
        throw RUNTIME_EXCEPT("Could not get a position of a mouse cursor.") ;
    }

    if(!EnumWindows(EnumWindowsProcForExchange, reinterpret_cast<LPARAM>(hwnd))) {
        throw RUNTIME_EXCEPT("Could not enumerate all top-level windows on the screen.") ;
    }

    if(g_near_hwnds.empty()) {
        return ;
    }

    RECT rect ;
    if(!GetWindowRect(hwnd, &rect)) {
        throw RUNTIME_EXCEPT("Could not get a rectangle of the target window.") ;
    }

    auto nearest_hwnd = g_near_hwnds.begin()->second ;
    RECT nearest_rect ;
    if(!GetWindowRect(nearest_hwnd, &nearest_rect)) {
        throw RUNTIME_EXCEPT("Could not get a rectangle of the nearest window.") ;
    }

    ResizeWindow::resize_window(
            nearest_hwnd,
            rect.left,
            rect.top,
            ScreenMetrics::width(rect),
            ScreenMetrics::height(rect)) ;

    ResizeWindow::resize_window(
            hwnd,
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
        KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr))
{
    if(!first_call) return ;

    auto hwnd = GetForegroundWindow() ;
    if(hwnd == NULL) {
        throw RUNTIME_EXCEPT("There is not the foreground window.") ;
    }

    RECT rect ;
    if(!GetWindowRect(hwnd, &rect)) {
        throw RUNTIME_EXCEPT("Could not get a rectangle of a foreground window.") ;
    }

    const auto w = ScreenMetrics::width(rect) ;
    const auto h = ScreenMetrics::height(rect) ;

    //snap a original window to top
    ResizeWindow::resize_window(
            hwnd,
            rect.left, rect.top,
            w, h / 2) ;
    OpenNewCurrentWindow::sprocess(true, 1, nullptr, nullptr) ;

    auto new_hwnd = GetForegroundWindow() ;
    if(new_hwnd == NULL) {
        throw RUNTIME_EXCEPT("There is not the foreground window.") ;
    }
    if(new_hwnd == hwnd) {
        Sleep(500) ;
        new_hwnd = GetForegroundWindow() ;
    }

    //snap a new window to bottom
    ResizeWindow::resize_window(
            new_hwnd,
            rect.left, rect.top + h / 2,
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
        KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr))
{
    if(!first_call) return ;

    auto hwnd = GetForegroundWindow() ;
    if(hwnd == NULL) {
        throw RUNTIME_EXCEPT("There is not the foreground window.") ;
    }

    RECT rect ;
    if(!GetWindowRect(hwnd, &rect)) {
        throw RUNTIME_EXCEPT("Could not get a rectangle of a foreground window.") ;
    }

    const auto w = ScreenMetrics::width(rect) ;
    const auto h = ScreenMetrics::height(rect) ;

    //snap a original window to left
    ResizeWindow::resize_window(
            hwnd,
            rect.left, rect.top,
            w / 2, h) ;
    OpenNewCurrentWindow::sprocess(true, 1, nullptr, nullptr) ;

    auto new_hwnd = GetForegroundWindow() ;
    if(new_hwnd == NULL) {
        throw RUNTIME_EXCEPT("There is not the foreground window.") ;
    }
    if(new_hwnd == hwnd) {
        Sleep(500) ;
        new_hwnd = GetForegroundWindow() ;
    }

    //snap a new window to right
    ResizeWindow::resize_window(
            new_hwnd,
            rect.left + w / 2, rect.top,
            w / 2, h) ;
}

namespace ResizeWindow
{
    inline static auto compute_resize_delta(
            const unsigned int repeat_num,
            const KeyLogger* const parent_charlgr,
            const std::string& param_name) {

        if(parent_charlgr != nullptr) {
            auto str = KyLgr::log2str(*parent_charlgr) ;
            return static_cast<LONG>(KyLgr::extract_num(str)) ;
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
        KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const parent_charlgr)
{
    if(!first_call) return ;

    auto hwnd = GetForegroundWindow() ;
    if(hwnd == NULL) {
        throw RUNTIME_EXCEPT("There is not a foreground window.") ;
    }

    RECT rect ;
    if(!GetWindowRect(hwnd, &rect)) {
        throw RUNTIME_EXCEPT("Could not get a rectangle of a foreground window.") ;
    }

    auto str = KyLgr::log2str(*parent_charlgr) ;
    if(str.empty()) return ;

    auto width = static_cast<LONG>(KyLgr::extract_num(str)) ;

    RECT mrect ;
    RECT mrect_work ;
    ScreenMetrics::get_monitor_metrics(hwnd, &mrect, &mrect_work) ;

    auto max_width = mrect_work.right - rect.left ;
    if(max_width < width) {
        width = max_width ;
    }

    ResizeWindow::resize_window(
            hwnd, rect.left, rect.top,
            width, ScreenMetrics::height(rect)) ;
}

//IncreaseWindowWidth
const std::string IncreaseWindowWidth::sname() noexcept
{
    return "increase_window_width" ;
}

void IncreaseWindowWidth::sprocess(
        const bool first_call,
        const unsigned int repeat_num,
        KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const parent_charlgr)
{
    if(!first_call) return ;

    auto hwnd = GetForegroundWindow() ;
    if(hwnd == NULL) {
        throw RUNTIME_EXCEPT("There is not a foreground window.") ;
    }

    RECT rect ;
    if(!GetWindowRect(hwnd, &rect)) {
        throw RUNTIME_EXCEPT("Could not get a rectangle of a foreground window.") ;
    }

    auto width = ScreenMetrics::width(rect) ;
    width += ResizeWindow::compute_resize_delta(
            repeat_num, parent_charlgr, "window_width_delta") ;

    RECT mrect ;
    RECT mrect_work ;
    ScreenMetrics::get_monitor_metrics(hwnd, &mrect, &mrect_work) ;

    auto max_width = mrect_work.right - rect.left ;
    if(max_width < width) {
        width = max_width ;
    }

    ResizeWindow::resize_window(
            hwnd, rect.left, rect.top,
            width, ScreenMetrics::height(rect)) ;
}

//DecreaseWindowWidth
const std::string DecreaseWindowWidth::sname() noexcept
{
    return "decrease_window_width" ;
}

void DecreaseWindowWidth::sprocess(
        const bool first_call,
        const unsigned int repeat_num,
        KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const parent_charlgr)
{
    if(!first_call) return ;

    auto hwnd = GetForegroundWindow() ;
    if(hwnd == NULL) {
        throw RUNTIME_EXCEPT("There is not a foreground window.") ;
    }

    RECT rect ;
    if(!GetWindowRect(hwnd, &rect)) {
        throw RUNTIME_EXCEPT("Could not get a rectangle of a foreground window.") ;
    }

    const auto delta = ResizeWindow::compute_resize_delta(
            repeat_num, parent_charlgr, "window_width_delta") ;

    auto width = ScreenMetrics::width(rect) - delta ;
    if(width <= 0) { 
        VirtualCmdLine::msgout("e: Width below zero") ;
        return ;
    }

    ResizeWindow::resize_window(
            hwnd, rect.left, rect.top,
            width, ScreenMetrics::height(rect)) ;
}

//ResizeWindowHeight
const std::string ResizeWindowHeight::sname() noexcept
{
    return "resize_window_height" ;
}

void ResizeWindowHeight::sprocess(
        const bool first_call,
        const unsigned int UNUSED(repeat_num),
        KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const parent_charlgr)
{
    if(!first_call) return ;

    auto hwnd = GetForegroundWindow() ;
    if(hwnd == NULL) {
        throw RUNTIME_EXCEPT("There is not a foreground window.") ;
    }

    RECT rect ;
    if(!GetWindowRect(hwnd, &rect)) {
        throw RUNTIME_EXCEPT("Could not get a rectangle of a foreground window.") ;
    }

    auto str = KyLgr::log2str(*parent_charlgr) ;
    if(str.empty()) return ;

    auto height = static_cast<LONG>(KyLgr::extract_num(str)) ;

    RECT mrect ;
    RECT mrect_work ;
    ScreenMetrics::get_monitor_metrics(hwnd, &mrect, &mrect_work) ;

    auto max_height = mrect_work.bottom - rect.top ;
    if(max_height < height) {
        height = max_height ;
    }

    ResizeWindow::resize_window(
            hwnd, rect.left, rect.top,
            ScreenMetrics::width(rect), height) ;
}

//IncreaseWindowHeight
const std::string IncreaseWindowHeight::sname() noexcept
{
    return "increase_window_height" ;
}

void IncreaseWindowHeight::sprocess(
        const bool first_call,
        const unsigned int repeat_num,
        KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const parent_charlgr)
{
    if(!first_call) return ;

    auto hwnd = GetForegroundWindow() ;
    if(hwnd == NULL) {
        throw RUNTIME_EXCEPT("There is not a foreground window.") ;
    }

    RECT rect ;
    if(!GetWindowRect(hwnd, &rect)) {
        throw RUNTIME_EXCEPT("Could not get a rectangle of a foreground window.") ;
    }

    auto height = ScreenMetrics::height(rect) ;
    height += ResizeWindow::compute_resize_delta(
            repeat_num, parent_charlgr, "window_height_delta") ;

    RECT mrect ;
    RECT mrect_work ;
    ScreenMetrics::get_monitor_metrics(hwnd, &mrect, &mrect_work) ;

    auto max_height = mrect_work.bottom - rect.top ;
    if(max_height < height) {
        height = max_height ;
    }

    ResizeWindow::resize_window(
            hwnd, rect.left, rect.top,
            ScreenMetrics::width(rect), height) ;
}

//DecreaseWindowHeight
const std::string DecreaseWindowHeight::sname() noexcept
{
    return "decrease_window_height" ;
}

void DecreaseWindowHeight::sprocess(
        const bool first_call,
        const unsigned int repeat_num,
        KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const parent_charlgr)
{
    if(!first_call) return ;

    auto hwnd = GetForegroundWindow() ;
    if(hwnd == NULL) {
        throw RUNTIME_EXCEPT("There is not a foreground window.") ;
    }

    RECT rect ;
    if(!GetWindowRect(hwnd, &rect)) {
        throw RUNTIME_EXCEPT("Could not get a rectangle of a foreground window.") ;
    }

    const auto delta = ResizeWindow::compute_resize_delta(
            repeat_num, parent_charlgr, "window_height_delta") ;

    auto height = ScreenMetrics::height(rect) - delta ;
    if(height <= 0) { 
        VirtualCmdLine::msgout("e: Height below zero") ;
        return ;
    }

    ResizeWindow::resize_window(
            hwnd, rect.left, rect.top,
            ScreenMetrics::width(rect), height) ;
}
