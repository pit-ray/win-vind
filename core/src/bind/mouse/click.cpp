#include "click.hpp"

#include <windows.h>
#include <iostream>

#include "io/mouse.hpp"
#include "mode.hpp"

namespace vind
{
    //ClickLeft
    const std::string ClickLeft::sname() noexcept {
        return "click_left" ;
    }

    void ClickLeft::sprocess(
            bool first_call,
            unsigned int repeat_num,
            KeycodeLogger* const UNUSED(parent_keycodelgr),
            const CharLogger* const UNUSED(parent_charlgr)) {
        if(!first_call) return ;
        using namespace mode ;
        if(get_global_mode() == Mode::Visual) {
            change_mode(Mode::Normal) ;
        }

        for(unsigned int i = 0 ; i < repeat_num ; i ++)
            mouse::click(KEYCODE_MOUSE_LEFT) ;
    }


    //ClickRight
    const std::string ClickRight::sname() noexcept {
        return "click_right" ;
    }

    void ClickRight::sprocess(
            bool first_call,
            unsigned int repeat_num,
            KeycodeLogger* const UNUSED(parent_keycodelgr),
            const CharLogger* const UNUSED(parent_charlgr)) {
        if(!first_call) return ;
        using namespace mode ;
        if(get_global_mode() == Mode::Visual) {
            change_mode(Mode::Normal) ;
        }

        for(unsigned int i = 0 ; i < repeat_num ; i ++)
            mouse::click(KEYCODE_MOUSE_RIGHT) ;
    }
}
