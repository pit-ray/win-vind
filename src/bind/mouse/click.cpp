#include "click.hpp"

#include <windows.h>

#include "bind/safe_repeater.hpp"
#include "core/mode.hpp"
#include "core/ntype_logger.hpp"
#include "util/mouse.hpp"

#if defined(DEBUG)
#include <iostream>
#endif

namespace vind
{
    //ClickLeft
    ClickLeft::ClickLeft()
    : BindedFuncCreator("click_left")
    {}
    void ClickLeft::sprocess(unsigned int repeat_num) {
        using namespace mode ;
        if(get_global_mode() == Mode::GUI_VISUAL) {
            set_global_mode(Mode::GUI_NORMAL) ;
        }

        repeater::safe_for(repeat_num, [] {
            util::click(KEYCODE_MOUSE_LEFT) ;
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
        if(get_global_mode() == Mode::GUI_VISUAL) {
            set_global_mode(Mode::GUI_NORMAL) ;
        }

        repeater::safe_for(repeat_num, [] {
            util::click(KEYCODE_MOUSE_RIGHT) ;
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
