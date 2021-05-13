#include "bind/hotkey/switch_vdesktop.hpp"
#include "bind/safe_repeater.hpp"
#include "io/keybrd.hpp"
#include "util/def.hpp"

#include "key/ntype_logger.hpp"

namespace vind
{
    //SwitchVDesktop2Left
    SwitchVDesktop2Left::SwitchVDesktop2Left()
    : BindedFuncCreator("switch_to_left_vdesktop")
    {}
    void SwitchVDesktop2Left::sprocess(unsigned int repeat_num) {
        repeater::safe_for(repeat_num, [] {
            keybrd::pushup(KEYCODE_LCTRL, KEYCODE_LWIN, KEYCODE_LEFT) ;
        }) ;
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
    SwitchVDesktop2Right::SwitchVDesktop2Right()
    : BindedFuncCreator("switch_to_right_vdesktop")
    {}
    void SwitchVDesktop2Right::sprocess(unsigned int repeat_num) {
        repeater::safe_for(repeat_num, [] {
            keybrd::pushup(KEYCODE_LCTRL, KEYCODE_LWIN, KEYCODE_RIGHT) ;
        }) ;
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
    CreateNewVDesktop::CreateNewVDesktop()
    : BindedFuncCreator("create_new_vdesktop")
    {}
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
    CloseCurrentVDesktop::CloseCurrentVDesktop()
    : BindedFuncCreator("close_current_vdesktop")
    {}
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
    TaskView::TaskView()
    : BindedFuncCreator("task_view")
    {}
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
