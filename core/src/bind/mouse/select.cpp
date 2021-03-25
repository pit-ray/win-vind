#include "select.hpp"

#include "io/keybrd_eventer.hpp"
#include "io/mouse_eventer.hpp"
#include "utility.hpp"

namespace vind
{
    //SelectAll
    const std::string SelectAll::sname() noexcept {
        return "select_all" ;
    }

    void SelectAll::sprocess(
            const bool first_call,
            const unsigned int UNUSED(repeat_num),
            VKCLogger* const UNUSED(parent_vkclgr),
            const CharLogger* const UNUSED(parent_charlgr)) {
        if(first_call) {
            MouseEventer::click(VKC_MOUSE_LEFT) ;
            KeybrdEventer::pushup(VKC_LCTRL, VKC_A) ;
        }
    }


    //ForwardUINavigation
    const std::string ForwardUINavigation::sname() noexcept {
        return "forward_ui_navigation" ;
    }

    void ForwardUINavigation::sprocess(
            const bool first_call,
            const unsigned int repeat_num,
            VKCLogger* const UNUSED(parent_vkclgr),
            const CharLogger* const UNUSED(parent_charlgr)) {
        if(first_call) {
            for(unsigned int i = 0 ; i < repeat_num ; i ++) {
                KeybrdEventer::pushup(VKC_TAB) ;
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
            VKCLogger* const UNUSED(parent_vkclgr),
            const CharLogger* const UNUSED(parent_charlgr)) {
        if(first_call) {
            for(unsigned int i = 0 ; i < repeat_num ; i ++) {
                KeybrdEventer::pushup(VKC_LSHIFT, VKC_TAB) ;
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
            VKCLogger* const UNUSED(parent_vkclgr),
            const CharLogger* const UNUSED(parent_charlgr)) {
        if(first_call) {
            KeybrdEventer::pushup(VKC_SPACE) ;
        }
    }
}
