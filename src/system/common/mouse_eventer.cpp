#include "mouse_eventer.hpp"
#include <iostream>
using namespace std ;

namespace MouseEventer
{
    bool is_click(const Button bt) noexcept {
        INPUT in ;
        in.type = INPUT_MOUSE ;
        in.mi.dx = 0 ;
        in.mi.dy = 0 ;
        in.mi.mouseData = 0 ;
        in.mi.dwFlags = (bt == Button::LEFT) ? MOUSEEVENTF_LEFTDOWN : MOUSEEVENTF_RIGHTDOWN ;
        in.mi.time = 0 ;
        in.mi.dwExtraInfo = GetMessageExtraInfo() ;

        if(!SendInput(1, &in, sizeof(INPUT))) {
            ERROR_STREAM << "windows.h: " << GetLastError() << " (win_mouse_eventer.cpp)" << endl ;
            return false ;
        }

        in.mi.dwFlags = (bt == Button::LEFT) ? MOUSEEVENTF_LEFTUP : MOUSEEVENTF_RIGHTUP ;
        if(!SendInput(1, &in, sizeof(INPUT))) {
            ERROR_STREAM << "windows.h: " << GetLastError() << " (win_mouse_eventer.cpp)" << endl ;
            return false ;
        }

        return true ;
    }

    inline static bool _is_down_or_up(const DWORD event) noexcept {
        INPUT in ;
        in.type = INPUT_MOUSE ;
        in.mi.dx = 0 ;
        in.mi.dy = 0 ;
        in.mi.mouseData = 0 ;
        in.mi.dwFlags = event ;
        in.mi.time = 0 ;
        in.mi.dwExtraInfo = GetMessageExtraInfo() ;

        if(!SendInput(1, &in, sizeof(INPUT))) {
            ERROR_STREAM << "windows.h: " << GetLastError() << " (win_mouse_eventer.cpp)" << endl ;
            return false ;
        }

        return true ;
    }

    bool is_down(const Button bt) noexcept {
        return _is_down_or_up((bt == Button::LEFT) ? MOUSEEVENTF_LEFTDOWN : MOUSEEVENTF_RIGHTDOWN) ;
    }

    bool is_up(const Button bt) noexcept {
        return _is_down_or_up((bt == Button::LEFT) ? MOUSEEVENTF_LEFTUP : MOUSEEVENTF_RIGHTUP) ;
    }
}