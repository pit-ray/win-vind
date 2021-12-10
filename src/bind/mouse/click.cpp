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
    namespace bind
    {
        //ClickLeft
        ClickLeft::ClickLeft()
        : BindedFuncVoid("click_left")
        {}
        void ClickLeft::sprocess(unsigned int repeat_num) {
            using core::Mode ;
            if(core::get_global_mode() == Mode::GUI_VISUAL) {
                core::set_global_mode(Mode::GUI_NORMAL) ;
            }

            safe_for(repeat_num, [] {
                util::click(KEYCODE_MOUSE_LEFT) ;
            }) ;
        }
        void ClickLeft::sprocess(core::NTypeLogger& parent_lgr) {
            if(!parent_lgr.is_long_pressing()) {
                sprocess(parent_lgr.get_head_num()) ;
            }
        }
        void ClickLeft::sprocess(const core::CharLogger& UNUSED(parent_lgr)) {
            sprocess(1) ;
        }


        //ClickRight
        ClickRight::ClickRight()
        : BindedFuncVoid("click_right")
        {}
        void ClickRight::sprocess(unsigned int repeat_num) {
            using core::Mode ;
            if(core::get_global_mode() == Mode::GUI_VISUAL) {
                core::set_global_mode(Mode::GUI_NORMAL) ;
            }

            safe_for(repeat_num, [] {
                util::click(KEYCODE_MOUSE_RIGHT) ;
            }) ;
        }
        void ClickRight::sprocess(core::NTypeLogger& parent_lgr) {
            if(!parent_lgr.is_long_pressing()) {
                sprocess(parent_lgr.get_head_num()) ;
            }
        }

        void ClickRight::sprocess(const core::CharLogger& UNUSED(parent_lgr)) {
            sprocess(1) ;
        }
    }
}
