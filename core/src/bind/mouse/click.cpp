#include "bind/mouse/click.hpp"

#include <iostream>
#include <windows.h>

#include "bind/safe_repeater.hpp"
#include "io/mouse.hpp"
#include "key/ntype_logger.hpp"
#include "mode.hpp"

namespace vind
{
    //ClickLeft
    ClickLeft::ClickLeft()
    : BindedFuncCreator("click_left")
    {}
    void ClickLeft::sprocess(unsigned int repeat_num) {
        using namespace mode ;
        if(get_global_mode() == Mode::Visual) {
            change_mode(Mode::Normal) ;
        }

        repeater::safe_for(repeat_num, [] {
            mouse::click(KEYCODE_MOUSE_LEFT) ;
        }) ;
    }
    void ClickLeft::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(parent_lgr.get_head_num()) ;
        }
    }
    void ClickLeft::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess(1) ;
    }


    //ClickRight
    ClickRight::ClickRight()
    : BindedFuncCreator("click_right")
    {}
    void ClickRight::sprocess(unsigned int repeat_num) {
        using namespace mode ;
        if(get_global_mode() == Mode::Visual) {
            change_mode(Mode::Normal) ;
        }

        repeater::safe_for(repeat_num, [] {
            mouse::click(KEYCODE_MOUSE_RIGHT) ;
        }) ;
    }
    void ClickRight::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(parent_lgr.get_head_num()) ;
        }
    }

    void ClickRight::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess(1) ;
    }
}
