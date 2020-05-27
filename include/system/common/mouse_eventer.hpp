#ifndef _MOUSE_EVENTER_HPP
#define _MOUSE_EVENTER_HPP

#include <windows.h>
#include "msg_logger.hpp"

#define MOUSEEVENTF_HWHEEL 0x01000

namespace MouseEventer
{
    //allow this enum to implicit cast
    enum Button : unsigned char {
        LEFT = 0,
        RIGHT = 1,
    } ;

    bool is_click(const Button bt) noexcept ;
    bool is_down(const Button bt) noexcept ;
    bool is_up(const Button bt) noexcept ;

    template <typename T>
    inline bool _is_scroll_core(const DWORD event, T&& scr_delta) noexcept {
        INPUT in ;
        in.type = INPUT_MOUSE ;
        in.mi.dx = 0 ; //amount of motion
        in.mi.dy = 0 ; //amount of motion
        in.mi.mouseData = static_cast<int>(scr_delta) ;
        in.mi.dwFlags = event ;
        in.mi.time = 0 ;
        in.mi.dwExtraInfo = GetMessageExtraInfo() ;

        if(!SendInput(1, &in, sizeof(INPUT))) {
            WIN_ERROR_STREAM << "(win_mouse_eventer.cpp)" << std::endl ;
            return false ;
        }

        return true ;
    }

    template <typename T>
    inline bool is_vscroll(T&& scr_delta) noexcept {
        return _is_scroll_core(MOUSEEVENTF_WHEEL, std::forward<T>(scr_delta)) ;
    }

    template <typename T>
    inline bool is_hscroll(T&& scr_delta) noexcept {
        return _is_scroll_core(MOUSEEVENTF_HWHEEL, std::forward<T>(scr_delta)) ;
    }
}

#endif