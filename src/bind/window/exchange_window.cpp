#include "exchange_window.hpp"

#include <map>

#include "window_utility.hpp"

#include "core/char_logger.hpp"
#include "core/ntype_logger.hpp"
#include "util/def.hpp"
#include "util/rect.hpp"
#include "util/screen_metrics.hpp"

namespace
{
    using namespace vind ;

    std::map<LONG, HWND> g_near_hwnds ;

    BOOL CALLBACK EnumWindowsProcForExchange(HWND hwnd, LPARAM lparam) {
        const auto fginfo = reinterpret_cast<bind::ForegroundInfo*>(lparam) ;

        if(fginfo->hwnd == hwnd) {
            return TRUE ;
        }

        if(!bind::is_visible_hwnd(hwnd)) {
            return TRUE ; //continue
        }

        RECT rect ;
        if(!GetWindowRect(hwnd, &rect)) {
            return TRUE ;
        }

        if(!bind::is_window_mode(hwnd, rect)) {
            return TRUE ; //continue
        }

        //Is in range of work area
        util::MonitorInfo minfo ;
        util::get_monitor_metrics(hwnd, minfo) ;
        if(util::is_out_of_range(rect, minfo.work_rect.data())) {
            return TRUE ;
        }

        g_near_hwnds[util::l2_distance_nosq(rect, fginfo->rect) / 100] = hwnd ;
        return TRUE ;
    }
}

namespace vind
{
    namespace bind
    {
        //ExchangeWindowWithNearest
        ExchangeWindowWithNearest::ExchangeWindowWithNearest()
        : BindedFuncCreator("exchange_window_with_nearest")
        {}
        void ExchangeWindowWithNearest::sprocess() {
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

            resize_window(nearest_hwnd, fginfo.rect) ;
            resize_window(fginfo.hwnd, nearest_rect) ;
        }
        void ExchangeWindowWithNearest::sprocess(core::NTypeLogger& parent_lgr) {
            if(!parent_lgr.is_long_pressing()) {
                sprocess() ;
            }
        }
        void ExchangeWindowWithNearest::sprocess(const core::CharLogger& UNUSED(parent_lgr)) {
            sprocess() ;
        }
    }
}
