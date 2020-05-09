#ifndef _MOUSE_EVENTER_HPP
#define _MOUSE_EVENTER_HPP

#include <windows.h>

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

    bool is_vscroll(const int scr_delta=WHEEL_DELTA) noexcept ;
    bool is_hscroll(const int scr_delta=WHEEL_DELTA) noexcept ;
}

#endif