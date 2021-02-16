#include "resize_window.hpp"

#include <map>
#include <unordered_map>
#include <windows.h>
#include <dwmapi.h>
#include <psapi.h>

#include "i_params.hpp"
#include "jump_cursor.hpp"
#include "keybrd_eventer.hpp"
#include "msg_logger.hpp"
#include "screen_metrics.hpp"
#include "utility.hpp"

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
    /*
     * 探索して位置とサイズ交換
     *
     */

    /* NOT IMPLEMENTED*/

}

namespace ResizeWindow
{
    static std::unordered_map<HMONITOR, RECT> g_mrects ;
    static std::unordered_map<HMONITOR, std::map<SIZE_T, HWND>> g_ordered_hwnd ;

    inline static auto is_valid_hwnd(HWND hwnd) {
        //is movable window ? -----------
        if(hwnd == GetDesktopWindow()) {
            return false ;
        }
        if(hwnd == GetShellWindow()) {
            return false ;
        }

        //Is visible ? ------------------
        if(!IsWindowEnabled(hwnd)) {
            return false ;
        }
        if(!IsWindowVisible(hwnd)) {
            return false ;
        }
        if(IsIconic(hwnd)) { //is minimized?
            return false ;
        }

        int n_cloaked ;
        if(DwmGetWindowAttribute(hwnd, DWMWA_CLOAKED, &n_cloaked, sizeof(int)) != S_OK) {
            return false ;
        }
        if(n_cloaked) {
            return false ;
        }
        return true ;
    }

    static BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lparam) {
        if(!is_valid_hwnd(hwnd)) {
            return TRUE ; //continue
        }

        //is full screen window ?? ------------------
        RECT rect ;
        if(!GetWindowRect(hwnd, &rect)) {
            return FALSE ; //break
        }

        const auto width  = ScreenMetrics::width(rect) ;
        if(width == 0) {
            return TRUE ; //continue
        }

        const auto height = ScreenMetrics::height(rect) ;
        if(height == 0) {
            return TRUE ; //continue
        }

        RECT client_rect ;
        if(!GetClientRect(hwnd, &client_rect)) {
            return FALSE ; //break
        }
        if(ScreenMetrics::is_equel(rect, client_rect)) {
            return TRUE ; //continue
        }

        HMONITOR hmonitor ;
        RECT monitor_rect ;
        RECT monitor_rect_work ;
        ScreenMetrics::get_monitor_metrics(hwnd, &monitor_rect, &monitor_rect_work, &hmonitor) ;

        //Is in range of work area
        if(ScreenMetrics::is_out_of_range(rect, monitor_rect_work)) {
            return TRUE ; //continue
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

//ArrangeWindow
const std::string ArrangeWindow::sname() noexcept
{
    return "arrange_window" ;
}

void ArrangeWindow::sprocess(
        const bool first_call,
        const unsigned int UNUSED(repeat_num),
        KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr))
{
    if(!first_call) return ;

    //Search visible windows
    using namespace ResizeWindow ;
    g_ordered_hwnd.clear() ;
    g_mrects.clear() ;

    if(!EnumWindows(EnumWindowsProc, 0)) {
        throw RUNTIME_EXCEPT("Could not enumerate all top-level windows on the screen.") ;
    }

    std::unordered_map<HWND, RECT> rects ;
    assign_local_area_in_monitors(rects) ;
    batch_resize(rects) ;
}


//RotateWindowInCurrentMonitor
const std::string RotateWindowInCurrentMonitor::sname() noexcept
{
    return "rotate_window_in_current_monitor" ;
}

void RotateWindowInCurrentMonitor::sprocess(
        const bool first_call,
        const unsigned int repeat_num,
        KeyLogger* UNUSED(parent_vkclgr),
        const KeyLogger* const UNUSED(parent_charlgr))
{
    if(!first_call) return ;

    using namespace ResizeWindow ;

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

    //rotate-shift a value in hwnd maps
    auto& oh = g_ordered_hwnd[hmonitor] ;

    for(unsigned int i = 0 ; i < repeat_num ; i ++) {
        auto itr = oh.rbegin() ;
        auto head_val = itr->second ;
        auto pre_itr = itr ;
        itr ++ ;
        while(itr != oh.rend()) {
            pre_itr->second = itr->second ;
            pre_itr = itr ;
            itr ++ ;
        }
        pre_itr->second = head_val ;
    }

    std::unordered_map<HWND, RECT> rects ;
    assign_local_area_in_monitors(rects) ;
    batch_resize(rects) ;
}

