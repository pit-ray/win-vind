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

    bool click(const unsigned char btcode) noexcept {
        INPUT in ;
        in.type = INPUT_MOUSE ;
        in.mi.dx = 0 ;
        in.mi.dy = 0 ;
        in.mi.mouseData = 0 ;
        in.mi.dwFlags = (btcode == VKC_MOUSE_LEFT) ? MOUSEEVENTF_LEFTDOWN : MOUSEEVENTF_RIGHTDOWN ;
        in.mi.time = 0 ;
        in.mi.dwExtraInfo = GetMessageExtraInfo() ;

        if(!SendInput(1, &in, sizeof(INPUT))) {
            WIN_ERROR_STREAM << "(MouseEventer::click)\n" ;
            return false ;
        }
        btstate[btcode] = true ;

        in.mi.dwFlags = (btcode == VKC_MOUSE_LEFT) ? MOUSEEVENTF_LEFTUP : MOUSEEVENTF_RIGHTUP ;
        if(!SendInput(1, &in, sizeof(INPUT))) {
            WIN_ERROR_STREAM << "(MouseEventer::click)\n" ;
            return false ;
        }

        return true ;
    }

    inline static bool change_btstate(const DWORD event) noexcept {
        INPUT in ;
        in.type = INPUT_MOUSE ;
        in.mi.dx = 0 ;
        in.mi.dy = 0 ;
        in.mi.mouseData = 0 ;
        in.mi.dwFlags = event ;
        in.mi.time = 0 ;
        in.mi.dwExtraInfo = GetMessageExtraInfo() ;

        if(!SendInput(1, &in, sizeof(INPUT))) {
            WIN_ERROR_STREAM << " (MouseEventer::change_btstate)" << endl ;
            return false ;
        }

        return true ;
    }

    bool press(const unsigned char btcode) noexcept {
        return change_btstate((btcode == VKC_MOUSE_LEFT) ? MOUSEEVENTF_LEFTDOWN : MOUSEEVENTF_RIGHTDOWN) ;
    }

    bool release(const unsigned char btcode) noexcept {
        return change_btstate((btcode == VKC_MOUSE_LEFT) ? MOUSEEVENTF_LEFTUP : MOUSEEVENTF_RIGHTUP) ;
    }

    bool is_releasing_occured(const unsigned char btcode) noexcept {
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