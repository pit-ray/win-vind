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

    void click(const unsigned char btcode) {
        INPUT in ;
        in.type = INPUT_MOUSE ;
        in.mi.dx = 0 ;
        in.mi.dy = 0 ;
        in.mi.mouseData = 0 ;
        in.mi.dwFlags = (btcode == VKC_MOUSE_LEFT) ? MOUSEEVENTF_LEFTDOWN : MOUSEEVENTF_RIGHTDOWN ;
        in.mi.time = 0 ;
        in.mi.dwExtraInfo = GetMessageExtraInfo() ;

        if(!SendInput(1, &in, sizeof(INPUT))) {
            throw RUNTIME_EXCEPT("cannot send mouse-down event") ;
            return ;
        }
        //btstate[btcode] = true ;

        in.mi.dwFlags = (btcode == VKC_MOUSE_LEFT) ? MOUSEEVENTF_LEFTUP : MOUSEEVENTF_RIGHTUP ;
        if(!SendInput(1, &in, sizeof(INPUT))) {
            throw RUNTIME_EXCEPT("cannot send mouse-up event") ;
            return ;
        }
    }

    inline static void _change_btstate(const DWORD event) {
        INPUT in ;
        in.type = INPUT_MOUSE ;
        in.mi.dx = 0 ;
        in.mi.dy = 0 ;
        in.mi.mouseData = 0 ;
        in.mi.dwFlags = event ;
        in.mi.time = 0 ;
        in.mi.dwExtraInfo = GetMessageExtraInfo() ;

        if(!SendInput(1, &in, sizeof(INPUT))) {
            throw RUNTIME_EXCEPT("cannot change the mouse button state") ;
            return ;
        }
    }

    void press(const unsigned char btcode) {
        _change_btstate((btcode == VKC_MOUSE_LEFT) ? MOUSEEVENTF_LEFTDOWN : MOUSEEVENTF_RIGHTDOWN) ;
    }

    void release(const unsigned char btcode) {
        _change_btstate((btcode == VKC_MOUSE_LEFT) ? MOUSEEVENTF_LEFTUP : MOUSEEVENTF_RIGHTUP) ;
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
