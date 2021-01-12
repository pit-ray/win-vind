#ifndef _WIN_NOTIFY_ICON_HPP
#define _WIN_NOTIFY_ICON_HPP

#include "msg_logger.hpp"

#include <cstring>
#include <string>
#include <windows.h>

#define APPWM_ICONNOTIFY (WM_APP + 1)

class NotifyIcon
{
private:
    NOTIFYICONDATAA nid ;

public:
    explicit NotifyIcon(HWND& hwnd, const std::string tooltip_text, const std::string icon_path) : nid() {

        ZeroMemory(&nid, sizeof(nid)) ;

        nid.cbSize = sizeof(nid) ;
        nid.hWnd = hwnd ;
        nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP ;
        nid.uCallbackMessage = APPWM_ICONNOTIFY ;
        nid.hIcon = static_cast<HICON>(LoadImageA(0, icon_path.c_str(), IMAGE_ICON, 0, 0, LR_LOADFROMFILE)) ;
        strcpy(nid.szTip, tooltip_text.c_str()) ;
        nid.dwState = NIS_SHAREDICON ;
        nid.dwStateMask = NIS_SHAREDICON ;

        add_icon() ;
        ShowWindow(hwnd, SW_HIDE) ;
    }

    ~NotifyIcon() {
        delete_icon() ;
    }

    NotifyIcon(NotifyIcon&&) = delete ;
    NotifyIcon& operator=(NotifyIcon&&) = delete ;

    NotifyIcon(const NotifyIcon&) = delete ;
    NotifyIcon& operator=(const NotifyIcon&) = delete ;

    bool add_icon() noexcept {
        if(!Shell_NotifyIconA(NIM_ADD, &nid)) {
            WIN_ERROR_PRINT("cannnot setup notification area.") ;
            return false ;
        }
        return true ;
    }

    bool delete_icon() noexcept {
        if(!Shell_NotifyIconA(NIM_DELETE, &nid)) {
            WIN_ERROR_PRINT("cannnot refresh notification area.") ;
            return false ;
        }
        return true ;
    }
} ;


#endif
