#include "bind/easyclick/ui_scanner.hpp"

#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include "io/screen_metrics.hpp"
#include "util/container.hpp"
#include "util/def.hpp"
#include "util/rect.hpp"
#include "util/winwrap.hpp"

#if defined(DEBUG)
#include <iostream>
#endif


namespace
{
    using namespace vind ;

    struct ProcessScanInfo {
        DWORD pid ;
        std::vector<Box2D>& points ;
    } ;

    BOOL CALLBACK ScanCenterPoint(HWND hwnd, LPARAM lparam) {
        auto obj_list = reinterpret_cast<std::vector<Box2D>*>(lparam) ;

        if(!IsWindowVisible(hwnd)) {
            return TRUE ;
        }
        if(!IsWindowEnabled(hwnd)) {
            return TRUE ;
        }

        //also register the position of the other thread window's title bar.
        RECT rect ;
        if(!GetWindowRect(hwnd, &rect)) {
            return TRUE ;
        }

        if(util::width(rect) == 0 || util::height(rect) == 0) {
            return TRUE ;
        }
        if(rect.left < 0 || rect.top < 0 || rect.right < 0 || rect.bottom < 0) {
            return TRUE ;
        }

        obj_list->emplace_back(rect) ;
        return TRUE ;
    }

    BOOL CALLBACK EnumerateAllThread(HWND hwnd, LPARAM lparam) {
        auto psinfo = reinterpret_cast<ProcessScanInfo*>(lparam) ;

        DWORD procid ;
        auto thid = GetWindowThreadProcessId(hwnd, &procid) ;

        if(procid == psinfo->pid) {
            //enumerate all threads owned by the parent process.
            EnumThreadWindows(
                    thid, ScanCenterPoint,
                    reinterpret_cast<LPARAM>(&(psinfo->points))) ;
        }

        return TRUE ;
    }
}


namespace vind
{
    UIScanner::UIScanner()
    : UIWalker(UIA_IsKeyboardFocusablePropertyId)
    {}

    bool UIScanner::is_target(uiauto::SmartElement& elem) {
        BOOL flag ;
        if(FAILED(elem->get_CachedIsKeyboardFocusable(&flag))) {
            throw std::runtime_error("Could not get the keyboard focusable flag.") ;
        }
        return flag == TRUE ;
    }

    void UIScanner::scan(std::vector<Box2D>& obj_list, HWND hwnd) {
        //scan all GUI objects in current window and children
        UIWalker::scan(obj_list, hwnd) ;

        //enumerate all window owned by the foreground window process.
        DWORD procid ;
        if(GetWindowThreadProcessId(hwnd, &procid)) {
            ProcessScanInfo psinfo{procid, obj_list} ;
            if(!EnumWindows(EnumerateAllThread, reinterpret_cast<LPARAM>(&psinfo))) {
                throw RUNTIME_EXCEPT("Failed to scan for threads in the same process.") ;
            }
        }

        util::remove_deplication(obj_list) ;
    }
}
