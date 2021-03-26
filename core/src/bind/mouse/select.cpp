#include "select.hpp"

#include "io/keybrd.hpp"
#include "io/mouse.hpp"
#include "util/def.hpp"

namespace vind
{
    //SelectAll
    const std::string SelectAll::sname() noexcept {
        return "select_all" ;
    }

    void SelectAll::sprocess(
            const bool first_call,
            const unsigned int UNUSED(repeat_num),
            KeycodeLogger* const UNUSED(parent_vkclgr),
            const CharLogger* const UNUSED(parent_charlgr)) {
        if(first_call) {
            mouse::click(KEYCODE_MOUSE_LEFT) ;
            keybrd::pushup(KEYCODE_LCTRL, KEYCODE_A) ;
        }
    }


    //ForwardUINavigation
    const std::string ForwardUINavigation::sname() noexcept {
        return "forward_ui_navigation" ;
    }

    void ForwardUINavigation::sprocess(
            const bool first_call,
            const unsigned int repeat_num,
            KeycodeLogger* const UNUSED(parent_vkclgr),
            const CharLogger* const UNUSED(parent_charlgr)) {
        if(first_call) {
            for(unsigned int i = 0 ; i < repeat_num ; i ++) {
                keybrd::pushup(KEYCODE_TAB) ;
            }
        }
    }

    //BackwardUINavigation
    const std::string BackwardUINavigation::sname() noexcept {
        return "backward_ui_navigation" ;
    }

    void BackwardUINavigation::sprocess(
            const bool first_call,
            const unsigned int repeat_num,
            KeycodeLogger* const UNUSED(parent_vkclgr),
            const CharLogger* const UNUSED(parent_charlgr)) {
        if(first_call) {
            for(unsigned int i = 0 ; i < repeat_num ; i ++) {
                keybrd::pushup(KEYCODE_LSHIFT, KEYCODE_TAB) ;
            }
        }
    }

    //DecideFocusedUIObject
    const std::string DecideFocusedUIObject::sname() noexcept {
        return "decide_focused_ui_object" ;
    }

    void DecideFocusedUIObject::sprocess(
            const bool first_call,
            const unsigned int UNUSED(repeat_num),
            KeycodeLogger* const UNUSED(parent_vkclgr),
            const CharLogger* const UNUSED(parent_charlgr)) {
        if(first_call) {
            keybrd::pushup(KEYCODE_SPACE) ;
        }
    }
}
