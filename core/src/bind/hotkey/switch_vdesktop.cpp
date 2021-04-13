#include "bind/hotkey/switch_vdesktop.hpp"
#include "io/keybrd.hpp"
#include "util/def.hpp"

#include "bind/base/ntype_logger.hpp"

namespace vind
{
    //SwitchVDesktop2Left
    const std::string SwitchVDesktop2Left::sname() noexcept {
        return "switch_to_left_vdesktop" ;
    }
    void SwitchVDesktop2Left::sprocess(unsigned int repeat_num) {
        for(unsigned int i = 0 ; i < repeat_num ; i ++) {
            keybrd::pushup(KEYCODE_LCTRL, KEYCODE_LWIN, KEYCODE_LEFT) ;
        }
    }
    void SwitchVDesktop2Left::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(parent_lgr.get_head_num()) ;
        }
    }
    void SwitchVDesktop2Left::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess(1) ;
    }


    //SwitchVDesktop2Right
    const std::string SwitchVDesktop2Right::sname() noexcept {
        return "switch_to_right_vdesktop" ;
    }
    void SwitchVDesktop2Right::sprocess(unsigned int repeat_num) {
        for(unsigned int i = 0 ; i < repeat_num ; i ++) {
            keybrd::pushup(KEYCODE_LCTRL, KEYCODE_LWIN, KEYCODE_RIGHT) ;
        }
    }
    void SwitchVDesktop2Right::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess(parent_lgr.get_head_num()) ;
        }
    }
    void SwitchVDesktop2Right::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess(1) ;
    }


    //CreateNewVDesktop
    const std::string CreateNewVDesktop::sname() noexcept {
        return "create_new_vdesktop" ;
    }
    void CreateNewVDesktop::sprocess() {
        keybrd::pushup(KEYCODE_LCTRL, KEYCODE_LWIN, KEYCODE_D) ;
    }
    void CreateNewVDesktop::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess() ;
        }
    }
    void CreateNewVDesktop::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess() ;
    }


    //CloseCurrentVDesktop
    const std::string CloseCurrentVDesktop::sname() noexcept {
        return "close_current_vdesktop" ;
    }
    void CloseCurrentVDesktop::sprocess() {
        keybrd::pushup(KEYCODE_LCTRL, KEYCODE_LWIN, KEYCODE_F4) ;
    }
    void CloseCurrentVDesktop::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess() ;
        }
    }
    void CloseCurrentVDesktop::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess() ;
    }


    //TaskView
    const std::string TaskView::sname() noexcept {
        return "task_view" ;
    }
    void TaskView::sprocess() {
        keybrd::pushup(KEYCODE_LWIN, KEYCODE_TAB) ;
    }
    void TaskView::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess() ;
        }
    }
    void TaskView::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess() ;
    }
}
