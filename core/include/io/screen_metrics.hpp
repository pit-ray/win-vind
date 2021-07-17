#ifndef _SCREEN_METRICS_HPP
#define _SCREEN_METRICS_HPP

#include <windows.h>

#include <string>

#include "util/box_2d.hpp"


namespace vind
{
    namespace screenmetrics {
        Box2D get_conbined_metrics() ;
        Box2D get_primary_metrics() ;

        struct MonitorInfo {
            Box2D rect{} ;
            Box2D work_rect{} ;
            HMONITOR hmonitor = NULL ;
        } ;

        void get_monitor_metrics(HMONITOR hmonitor, MonitorInfo& minfo) ;

        void get_monitor_metrics(HWND hwnd, MonitorInfo& minfo) ;

        void get_monitor_metrics(const POINT& pos, MonitorInfo& minfo) ;

        void get_monitor_metrics(POINT&& pos, MonitorInfo& minfo) ;

        namespace debug {
            std::string info(const Box2D& rect) ;
        }
    }
}

#endif
