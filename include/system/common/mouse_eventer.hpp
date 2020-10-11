#ifndef _MOUSE_EVENTER_HPP
#define _MOUSE_EVENTER_HPP

#include <windows.h>

#include "msg_logger.hpp"
#include "utility.hpp"
#include "virtual_key_fwd.hpp"

#define MOUSEEVENTF_HWHEEL 0x01000

namespace MouseEventer
{
    void click(const unsigned char btcode) ;
    void press(const unsigned char btcode) ;
    void release(const unsigned char btcode) ;

    bool is_releasing_occured(const unsigned char btcode) ; //(since the last call)

    template <typename T>
    inline void _scroll_core(const DWORD event, T&& scr_delta) {
        INPUT in ;
        in.type = INPUT_MOUSE ;
        in.mi.dx = 0 ; //amount of horizontal motion
        in.mi.dy = 0 ; //amount of vertical motion
        in.mi.mouseData = static_cast<int>(scr_delta) ;
        in.mi.dwFlags = event ;
        in.mi.time = 0 ;
        in.mi.dwExtraInfo = GetMessageExtraInfo() ;
        if(!SendInput(1, &in, sizeof(INPUT))) {
            throw RUNTIME_EXCEPT("SendInput is failed.") ;
        }
    }

    template <typename T>
    inline void vscroll(T&& scr_delta) {
        return _scroll_core(MOUSEEVENTF_WHEEL, std::forward<T>(scr_delta)) ;
    }

    template <typename T>
    inline void hscroll(T&& scr_delta) {
        return _scroll_core(MOUSEEVENTF_HWHEEL, std::forward<T>(scr_delta)) ;
    }
}

#endif
