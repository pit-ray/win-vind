#ifndef _SAFE_REPEATER_HPP
#define _SAFE_REPEATER_HPP

#include <windows.h>

#include "core/key_absorber.hpp"

namespace vind
{
    namespace bind
    {
        template <typename T, typename FuncType, typename ...Args>
        inline void safe_for(T repeat_num, FuncType&& func, Args... args) {
            for(T i = 0 ; i < repeat_num ; i ++) {
                if(core::is_pressed(KEYCODE_ESC)) {
                    break ;
                }
                func(std::forward<Args>(args)...) ;
                Sleep(1) ;
            }
        }
    }
}

#endif
