#ifndef _WINDOW_UTILITY_HPP
#define _WINDOW_UTILITY_HPP

#include <windows.h>

#include <memory>
#include <unordered_map>

#include "util/box_2d.hpp"
#include "util/def.hpp"

namespace vind
{
    namespace windowutil {
        bool is_visible_hwnd(HWND hwnd) ;
        bool is_window_mode(HWND hwnd, const RECT& rect) ;

        void resize(
                HWND hwnd,
                LONG left, LONG top,
                LONG width, LONG height) ;
        void resize(HWND hwnd, const RECT& rect) ;
        void resize(HWND hwnd, const Box2D& rect) ;

        void batch_resize(const std::unordered_map<HWND, Box2D>& rects) ;
        void batch_resize(const std::unordered_map<HWND, RECT>& rects) ;

        struct ForegroundInfo {
            HWND hwnd ;
            HMONITOR hmonitor ;
            Box2D rect ;

            explicit ForegroundInfo()
            : hwnd(NULL),
              hmonitor(NULL),
              rect()
            {
                hwnd = GetForegroundWindow() ;
                if(hwnd == NULL) {
                    throw RUNTIME_EXCEPT("Could not get a position of a mouse cursor.") ;
                }

                hmonitor = MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST) ;

                if(!GetWindowRect(hwnd, &(rect.data()))) {
                    throw RUNTIME_EXCEPT("Could not get a rectangle of a foreground window.") ;
                }
            }

            virtual ~ForegroundInfo() noexcept               = default ;
            ForegroundInfo(const ForegroundInfo&)            = default ;
            ForegroundInfo& operator=(const ForegroundInfo&) = default ;
            ForegroundInfo(ForegroundInfo&&)                 = default ;
            ForegroundInfo& operator=(ForegroundInfo&&)      = default ;
        } ;
    }
}

#endif
