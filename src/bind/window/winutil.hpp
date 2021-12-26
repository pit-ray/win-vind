#ifndef _WINDOW_UTILITY_HPP
#define _WINDOW_UTILITY_HPP

#include <windows.h>

#include <memory>
#include <unordered_map>

#include "util/box2d.hpp"
#include "util/def.hpp"
#include "util/rect.hpp"

namespace vind
{
    namespace bind
    {
        bool is_visible_hwnd(HWND hwnd) ;
        bool is_window_mode(HWND hwnd, const RECT& rect) ;

        void resize_window(
                HWND hwnd,
                LONG left, LONG top,
                LONG width, LONG height) ;
        inline void resize_window(HWND hwnd, const RECT& rect) {
            resize_window(hwnd, rect.left, rect.top, util::width(rect), util::height(rect)) ;
        }
        inline void resize_window(HWND hwnd, const util::Box2D& rect) {
            resize_window(hwnd, rect.left(), rect.top(), rect.width(), rect.height()) ;
        }

        void batch_resize(const std::unordered_map<HWND, util::Box2D>& rects) ;
        void batch_resize(const std::unordered_map<HWND, RECT>& rects) ;

        struct ForegroundInfo {
            HWND hwnd ;
            HMONITOR hmonitor ;
            util::Box2D rect ;

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
