#include "mouse_eventer.hpp"
#include <iostream>
#include <memory>
#include <unordered_map>
using namespace std ;

namespace MouseEventer
{
    static std::unordered_map<unsigned char, bool> btstate = {
        {VKC_MOUSE_LEFT,  false},
        {VKC_MOUSE_MID,   false},
        {VKC_MOUSE_RIGHT, false}
    } ;

    inline static DWORD btcode_to_upevent(const unsigned btcode) noexcept {
        switch(btcode) {
            case VKC_MOUSE_LEFT: return MOUSEEVENTF_LEFTUP ;
            case VKC_MOUSE_RIGHT: return MOUSEEVENTF_RIGHTUP ;
            case VKC_MOUSE_MID: return MOUSEEVENTF_MIDDLEUP ;
            default: return 0 ;
        }
    }
    inline static DWORD btcode_to_downevent(const unsigned btcode) noexcept {
        switch(btcode) {
            case VKC_MOUSE_LEFT: return MOUSEEVENTF_LEFTDOWN ;
            case VKC_MOUSE_RIGHT: return MOUSEEVENTF_RIGHTDOWN ;
            case VKC_MOUSE_MID: return MOUSEEVENTF_MIDDLEDOWN ;
            default: return 0 ;
        }
    }

    void click(const unsigned char btcode) {
        static INPUT in = {INPUT_MOUSE, {.mi = {0, 0, 0, 0, 0, 0}}} ;
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

    void press(const unsigned char btcode) {
        static INPUT in = {INPUT_MOUSE, {.mi = {0, 0, 0, 0, 0, 0}}} ;
        in.mi.dwFlags = btcode_to_downevent(btcode) ;
        in.mi.dwExtraInfo = GetMessageExtraInfo() ;

        if(!SendInput(1, &in, sizeof(INPUT))) {
            throw RUNTIME_EXCEPT("cannot change the mouse button state to down.") ;
        }
    }

    void release(const unsigned char btcode) {
        static INPUT in = {INPUT_MOUSE, {.mi = {0, 0, 0, 0, 0, 0}}} ;
        in.mi.dwFlags = btcode_to_upevent(btcode) ;
        in.mi.dwExtraInfo = GetMessageExtraInfo() ;

        if(!SendInput(1, &in, sizeof(INPUT))) {
            throw RUNTIME_EXCEPT("cannot change the mouse button state to up.") ;
        }
    }

    bool is_releasing_occured(const unsigned char btcode) {
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
}
