#include "click.hpp"

#include <windows.h>

#include "bind/saferepeat.hpp"
#include "core/mode.hpp"
#include "util/debug.hpp"
#include "util/mouse.hpp"


namespace vind
{
    namespace bind
    {
        ClickLeft::ClickLeft()
        : BindedFuncVoid("click_left")
        {}
        void ClickLeft::sprocess(
                std::uint16_t count,
                const std::string& UNUSED(args)) {
            using core::Mode ;
            if(core::get_global_mode() == Mode::GUI_VISUAL) {
                core::set_global_mode(Mode::GUI_NORMAL) ;
            }

            safe_for(count, [] {
                util::click(KEYCODE_MOUSE_LEFT) ;
            }) ;
        }

        ClickRight::ClickRight()
        : BindedFuncVoid("click_right")
        {}
        void ClickRight::sprocess(
                std::uint16_t count,
                const std::string& UNUSED(args)) {
            using core::Mode ;
            if(core::get_global_mode() == Mode::GUI_VISUAL) {
                core::set_global_mode(Mode::GUI_NORMAL) ;
            }

            safe_for(count, [] {
                util::click(KEYCODE_MOUSE_RIGHT) ;
            }) ;
        }

        ClickMid::ClickMid()
        : BindedFuncVoid("click_mid")
        {}
        void ClickMid::sprocess(
                std::uint16_t count,
                const std::string& UNUSED(args)) {
            using core::Mode ;
            if(core::get_global_mode() == Mode::GUI_VISUAL) {
                core::set_global_mode(Mode::GUI_NORMAL) ;
            }

            safe_for(count, [] {
                util::click(KEYCODE_MOUSE_MID) ;
            }) ;
        }
    }
}
