#ifndef _MOUSE_HPP
#define _MOUSE_HPP

#include <windows.h>

#include "err_logger.hpp"
#include "key/keycode_def.hpp"
#include "util/def.hpp"

#ifndef MOUSEEVENTF_HWHEEL
#define MOUSEEVENTF_HWHEEL 0x01000
#endif

namespace vind
{
    namespace mouse {
        void click(KeyCode btcode) ;
        void press(KeyCode btcode) ;
        void release(KeyCode btcode) ;

        bool is_releasing_occured(KeyCode btcode) ; //(since the last call)

        template <typename T>
        inline void _scroll_core(DWORD event, T&& scr_delta) {
            static INPUT in = {INPUT_MOUSE, {.mi = {0, 0, 0, 0, 0, 0}}};
            in.mi.mouseData = static_cast<int>(scr_delta) ;
            in.mi.dwFlags = event ;
            in.mi.dwExtraInfo = GetMessageExtraInfo() ;
            if(!SendInput(1, &in, sizeof(INPUT))) {
                throw RUNTIME_EXCEPT("SendInput failed.") ;
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

        void move_cursor(int dx, int dy) ;
    }
}

#endif
