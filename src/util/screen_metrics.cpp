#include "screen_metrics.hpp"

#include <iomanip>

#include <sstream>
#include <string>

#include "debug.hpp"
#include "def.hpp"
#include "rect.hpp"

#define MONITOR_SEARCH_MARGIN (256)


namespace vind
{
    namespace util
    {
        Box2D get_combined_metrics() {
            WINDOWINFO winfo ;
            winfo.cbSize = sizeof(WINDOWINFO) ;
            if(!GetWindowInfo(GetDesktopWindow(), &winfo)) {
                throw RUNTIME_EXCEPT("Could not get window infomation.") ;
            }
            return Box2D(winfo.rcWindow) ;
        }

        Box2D get_primary_metrics() {
            MONITORINFO minfo ;
            minfo.cbSize = sizeof(MONITORINFO) ;
            auto hmonitor = MonitorFromWindow(GetDesktopWindow(), MONITOR_DEFAULTTOPRIMARY) ;
            if(!GetMonitorInfo(hmonitor, &minfo)) {
                throw RUNTIME_EXCEPT("Could not get primary monitor infomation.") ;
            }
            return Box2D(minfo.rcMonitor) ;
        }

        void get_monitor_metrics(HMONITOR hmonitor, MonitorInfo& minfo) {
            if(hmonitor == NULL) return ;
            minfo.hmonitor = hmonitor ;

            MONITORINFO native_minfo ;
            native_minfo.cbSize = sizeof(MONITORINFO) ;
            if(!GetMonitorInfo(minfo.hmonitor, &native_minfo)) {
                throw RUNTIME_EXCEPT("Could not get monitor infomation.") ;
            }

            minfo.rect = native_minfo.rcMonitor ;
            minfo.work_rect = native_minfo.rcWork ;
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

        // TODO: we should be implement vertical monitor detection.
        std::vector<MonitorInfo> get_all_monitor_metrics() {
            std::vector<MonitorInfo> minfos ;
            auto entire_box = get_combined_metrics() ;

            MonitorInfo buf ;
            long x = entire_box.left() + MONITOR_SEARCH_MARGIN ;
            long y_mid = 1 ;
            while(x < entire_box.right()) {
                get_monitor_metrics(Point2D{x, y_mid}, buf) ;
                minfos.push_back(buf) ;

                x += buf.rect.width() ;
                y_mid = buf.rect.center_y() ;
            }

            return minfos ;
        }

        namespace debug {
            std::string info(const Box2D& rect) {
                std::stringstream ss ;
                ss << "("    << std::setw(4) << rect.left() ;
                ss << ", "   << std::setw(4) << rect.top() << ")\t" ;
                ss << "("    << std::setw(4) << rect.right() ;
                ss << ", "   << std::setw(4) << rect.bottom() << ")\t" ;
                ss << "w: "  << std::setw(4) << rect.width() ;
                ss << ", h:" << std::setw(4) << rect.height() ;
                return ss.str() ;
            }
        }
    }
}
