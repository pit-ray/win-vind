#include "io/screen_metrics.hpp"

#include <iomanip>

#include <sstream>
#include <string>

#include "util/def.hpp"

namespace vind
{
    namespace screenmetrics
    {
        void get_conbined_metrics(RECT* const rect) {
            WINDOWINFO winfo ;
            winfo.cbSize = sizeof(WINDOWINFO) ;
            if(!GetWindowInfo(GetDesktopWindow(), &winfo)) {
                throw RUNTIME_EXCEPT("Could not get window infomation.") ;
            }
            copy(*rect, winfo.rcWindow) ;
        }

        void get_primary_metrics(RECT* const rect) {
            MONITORINFO minfo ;
            minfo.cbSize = sizeof(MONITORINFO) ;
            const auto hmonitor = MonitorFromWindow(GetDesktopWindow(), MONITOR_DEFAULTTOPRIMARY) ;
            if(!GetMonitorInfo(hmonitor, &minfo)) {
                throw RUNTIME_EXCEPT("Could not get primary monitor infomation.") ;
            }
            copy(*rect, minfo.rcMonitor) ;
        }

        void get_monitor_metrics(HMONITOR hmonitor, MonitorInfo& minfo) {
            if(hmonitor == NULL) return ;
            minfo.hmonitor = hmonitor ;

            MONITORINFO native_minfo ;
            native_minfo.cbSize = sizeof(MONITORINFO) ;
            if(!GetMonitorInfo(minfo.hmonitor, &native_minfo)) {
                throw RUNTIME_EXCEPT("Could not get monitor infomation.") ;
            }

            copy(minfo.rect, native_minfo.rcMonitor) ;
            copy(minfo.work_rect, native_minfo.rcWork) ;
        }

        void get_monitor_metrics(HWND hwnd, MonitorInfo& minfo) {
            if(hwnd == NULL) return ;
            get_monitor_metrics(MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST), minfo) ;
        }

        void get_monitor_metrics(const POINT& pos, MonitorInfo& minfo) {
            get_monitor_metrics(MonitorFromPoint(pos, MONITOR_DEFAULTTONEAREST), minfo) ;
        }

        void get_monitor_metrics(POINT&& pos, MonitorInfo& minfo) {
            get_monitor_metrics(MonitorFromPoint(std::move(pos), MONITOR_DEFAULTTONEAREST), minfo) ;
        }

        namespace debug {
            std::string info(const RECT& rect) {
                std::stringstream ss ;
                ss << "("    << std::setw(4) << rect.left ;
                ss << ", "   << std::setw(4) << rect.top << ")\t" ;
                ss << "("    << std::setw(4) << rect.right ;
                ss << ", "   << std::setw(4) << rect.bottom << ")\t" ;
                ss << "w: "  << std::setw(4) << width(rect) ;
                ss << ", h:" << std::setw(4) << height(rect) ;
                return ss.str() ;
            }
        }
    }
}
