#include "bind/window/exchange_window.hpp"

#include <map>

#include "bind/window/window_utility.hpp"
#include "io/screen_metrics.hpp"
#include "key/char_logger.hpp"
#include "key/ntype_logger.hpp"
#include "util/def.hpp"

namespace
{
    using namespace vind ;

    std::map<LONG, HWND> g_near_hwnds ;

    BOOL CALLBACK EnumWindowsProcForExchange(HWND hwnd, LPARAM lparam) {
        const auto fginfo = reinterpret_cast<windowutil::ForegroundInfo*>(lparam) ;

        if(fginfo->hwnd == hwnd) {
            return TRUE ;
        }

        if(!windowutil::is_visible_hwnd(hwnd)) {
            return TRUE ; //continue
        }

        RECT rect ;
        if(!GetWindowRect(hwnd, &rect)) {
            return TRUE ;
        }

        if(!windowutil::is_window_mode(hwnd, rect)) {
            return TRUE ; //continue
        }

        //Is in range of work area
        screenmetrics::MonitorInfo minfo ;
        screenmetrics::get_monitor_metrics(hwnd, minfo) ;
        if(screenmetrics::is_out_of_range(rect, minfo.work_rect)) {
            return TRUE ;
        }

        g_near_hwnds[screenmetrics::l2_distance_nosq(rect, fginfo->rect) / 100] = hwnd ;
        return TRUE ;
    }
}

namespace vind
{
    //ExchangeWindowWithNextOne
    ExchangeWindowWithNextOne::ExchangeWindowWithNextOne()
    : BindedFuncCreator("exchange_window_with_next_one")
    {}
    void ExchangeWindowWithNextOne::sprocess() {
        g_near_hwnds.clear() ;

        windowutil::ForegroundInfo fginfo ;
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

        windowutil::resize(
                nearest_hwnd,
                fginfo.rect.left,
                fginfo.rect.top,
                screenmetrics::width(fginfo.rect),
                screenmetrics::height(fginfo.rect)) ;

        windowutil::resize(
                fginfo.hwnd,
                nearest_rect.left,
                nearest_rect.top,
                screenmetrics::width(nearest_rect),
                screenmetrics::height(nearest_rect)) ;
    }
    void ExchangeWindowWithNextOne::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess() ;
        }
    }
    void ExchangeWindowWithNextOne::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess() ;
    }
}
