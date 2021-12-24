#include "mouse.hpp"

#include <memory>
#include <unordered_map>

#include "core/keycode_def.hpp"
#include "def.hpp"

#if defined(DEBUG)
#include <iostream>
#endif


namespace
{
    std::unordered_map<unsigned char, bool> btstate = {
        {KEYCODE_MOUSE_LEFT,  false},
        {KEYCODE_MOUSE_MID,   false},
        {KEYCODE_MOUSE_RIGHT, false}
    } ;

    inline DWORD btcode_to_upevent(unsigned btcode) noexcept {
        switch(btcode) {
            case KEYCODE_MOUSE_LEFT: return MOUSEEVENTF_LEFTUP ;
            case KEYCODE_MOUSE_RIGHT: return MOUSEEVENTF_RIGHTUP ;
            case KEYCODE_MOUSE_MID: return MOUSEEVENTF_MIDDLEUP ;
            default: return 0 ;
        }
    }
    inline DWORD btcode_to_downevent(unsigned btcode) noexcept {
        switch(btcode) {
            case KEYCODE_MOUSE_LEFT: return MOUSEEVENTF_LEFTDOWN ;
            case KEYCODE_MOUSE_RIGHT: return MOUSEEVENTF_RIGHTDOWN ;
            case KEYCODE_MOUSE_MID: return MOUSEEVENTF_MIDDLEDOWN ;
            default: return 0 ;
        }
    }
}
namespace vind
{
    namespace util {
        void click(unsigned char btcode) {
#if defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif
            static INPUT in = {INPUT_MOUSE} ;
#if defined(__gnuc__)
#pragma gcc diagnostic warning "-wmissing-field-initializers"
#endif
            in.mi.dwFlags = btcode_to_downevent(btcode) ;
            in.mi.dwExtraInfo = GetMessageExtraInfo() ;

            if(!SendInput(1, &in, sizeof(INPUT))) {
                throw RUNTIME_EXCEPT("cannot send mouse-down event") ;
            }
            //btstate[btcode] = true ;

            in.mi.dwFlags = btcode_to_upevent(btcode) ;
            if(!SendInput(1, &in, sizeof(INPUT))) {
                throw RUNTIME_EXCEPT("cannot send mouse-up event") ;
            }
        }

        void press_mousestate(unsigned char btcode) {
#if defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif
            static INPUT in = {INPUT_MOUSE} ;
#if defined(__gnuc__)
#pragma gcc diagnostic warning "-wmissing-field-initializers"
#endif
            in.mi.dwFlags = btcode_to_downevent(btcode) ;
            in.mi.dwExtraInfo = GetMessageExtraInfo() ;

            if(!SendInput(1, &in, sizeof(INPUT))) {
                throw RUNTIME_EXCEPT("cannot change the mouse button state to down.") ;
            }
        }

        void release_mousestate(unsigned char btcode) {
#if defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif
            static INPUT in = {INPUT_MOUSE} ;
#if defined(__gnuc__)
#pragma gcc diagnostic warning "-wmissing-field-initializers"
#endif
            in.mi.dwFlags = btcode_to_upevent(btcode) ;
            in.mi.dwExtraInfo = GetMessageExtraInfo() ;

            if(!SendInput(1, &in, sizeof(INPUT))) {
                throw RUNTIME_EXCEPT("cannot change the mouse button state to up.") ;
            }
        }

        bool is_releasing_occured(unsigned char btcode) {
            if(GetAsyncKeyState(btcode) & 0x8000) {
                if(!btstate[btcode]) btstate[btcode] = true ;
            }
            else {
                if(btstate[btcode]) {
                    btstate[btcode] = false ;
                    return true ;
                }
            }
            return false ;
        }

        void move_cursor(int dx, int dy) {
            static INPUT in = [] {
                INPUT tmp = {INPUT_MOUSE} ;
                tmp.mi.dwFlags = MOUSEEVENTF_MOVE ;
                return tmp ;
            } () ;

            in.mi.dx = dx ;
            in.mi.dy = dy ;

            if(!SendInput(1, &in, sizeof(INPUT))) {
                throw RUNTIME_EXCEPT("cannot send the moving event of the mouse") ;
            }
        }
    }
}
