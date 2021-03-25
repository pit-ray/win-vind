#include "exchange_window.hpp"

#include <map>

#include "io/screen_metrics.hpp"
#include "utility.hpp"
#include "window_utility.hpp"

namespace
{
    using namespace vind ;

    std::map<LONG, HWND> g_near_hwnds ;

    BOOL CALLBACK EnumWindowsProcForExchange(HWND hwnd, LPARAM lparam) {
        const auto fginfo = reinterpret_cast<WindowUtility::ForegroundInfo*>(lparam) ;

        if(fginfo->hwnd == hwnd) {
            return TRUE ;
        }

        if(!WindowUtility::is_visible_hwnd(hwnd)) {
            return TRUE ; //continue
        }

        RECT rect ;
        if(!GetWindowRect(hwnd, &rect)) {
            return TRUE ;
        }

        if(!WindowUtility::is_window_mode(hwnd, rect)) {
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

namespace vind
{
    //ExchangeWindowWithNextOne
    const std::string ExchangeWindowWithNextOne::sname() noexcept {
        return "exchange_window_with_next_one" ;
    }

    void ExchangeWindowWithNextOne::sprocess(
            const bool first_call,
            const unsigned int UNUSED(repeat_num),
            VKCLogger* const UNUSED(parent_vkclgr),
            const CharLogger* const UNUSED(parent_charlgr)) {
        if(!first_call) return ;

        g_near_hwnds.clear() ;

        WindowUtility::ForegroundInfo fginfo ;
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

        WindowUtility::resize(
                nearest_hwnd,
                fginfo.rect.left,
                fginfo.rect.top,
                ScreenMetrics::width(fginfo.rect),
                ScreenMetrics::height(fginfo.rect)) ;

        WindowUtility::resize(
                fginfo.hwnd,
                nearest_rect.left,
                nearest_rect.top,
                ScreenMetrics::width(nearest_rect),
                ScreenMetrics::height(nearest_rect)) ;
    }
}
