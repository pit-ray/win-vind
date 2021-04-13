#include "bind/mouse/click.hpp"

#include <windows.h>
#include <iostream>

#include "bind/base/ntype_logger.hpp"
#include "io/mouse.hpp"
#include "bind/base/mode.hpp"

namespace vind
{
    //ClickLeft
    const std::string ClickLeft::sname() noexcept {
        return "click_left" ;
    }
    void ClickLeft::sprocess(unsigned int repeat_num) {
        using namespace mode ;
        if(get_global_mode() == Mode::Visual) {
            change_mode(Mode::Normal) ;
        }

        for(decltype(repeat_num) i = 0 ; i < repeat_num ; i ++) {
            mouse::click(KEYCODE_MOUSE_LEFT) ;
        }
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
    const std::string ClickRight::sname() noexcept {
        return "click_right" ;
    }
    void ClickRight::sprocess(unsigned int repeat_num) {
        using namespace mode ;
        if(get_global_mode() == Mode::Visual) {
            change_mode(Mode::Normal) ;
        }

        for(decltype(repeat_num) i = 0 ; i < repeat_num ; i ++) {
            mouse::click(KEYCODE_MOUSE_RIGHT) ;
        }
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
