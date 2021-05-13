#ifndef _SAFE_REPEATER_HPP
#define _SAFE_REPEATER_HPP

#include <windows.h>

#include "key/key_absorber.hpp"

namespace vind
{
    namespace repeater {
        template <typename T, typename FuncType, typename ...Args>
        inline void safe_for(T repeat_num, FuncType&& func, Args... args) {
            for(T i = 0 ; i < repeat_num ; i ++) {
                if(keyabsorber::is_pressed(KEYCODE_ESC)) {
                    break ;
                }
                func(std::forward<Args>(args)...) ;
                Sleep(1) ;
            }
        }
    }
}

#endif
