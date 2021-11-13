#include "tab_ctrl.hpp"

#include <windows.h>

#include "bind/mode/change_mode.hpp"
#include "bind/safe_repeater.hpp"
#include "core/char_logger.hpp"
#include "core/err_logger.hpp"
#include "core/ntype_logger.hpp"
#include "util/def.hpp"
#include "util/keybrd.hpp"


namespace vind
{
    //Switch2LeftTab
    Switch2LeftTab::Switch2LeftTab()
    : BindedFuncCreator("switch_to_left_tab")
    {}
    void Switch2LeftTab::sprocess(unsigned int repeat_num) {
        repeater::safe_for(repeat_num, [] {
            util::pushup(KEYCODE_LCTRL, KEYCODE_LSHIFT, KEYCODE_TAB) ;
        }) ;
    }
    void Switch2LeftTab::sprocess(core::NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess() ;
        }
    }
    void Switch2LeftTab::sprocess(const core::CharLogger& UNUSED(parent_lgr)) {
        sprocess() ;
    }


    //Switch2RightTab
    Switch2RightTab::Switch2RightTab()
    : BindedFuncCreator("switch_to_right_tab")
    {}
    void Switch2RightTab::sprocess(unsigned repeat_num) {
        repeater::safe_for(repeat_num, [] {
            util::pushup(KEYCODE_LCTRL, KEYCODE_TAB) ;
        }) ;
    }
    void Switch2RightTab::sprocess(core::NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess() ;
        }
    }
    void Switch2RightTab::sprocess(const core::CharLogger& UNUSED(parent_lgr)) {
        sprocess() ;
    }

    //OpenNewTab
    OpenNewTab::OpenNewTab()
    : BindedFuncCreator("open_new_tab")
    {}
    void OpenNewTab::sprocess() {
        util::pushup(KEYCODE_LCTRL, KEYCODE_T) ;
    }
    void OpenNewTab::sprocess(core::NTypeLogger& parent_lgr) {
        if(parent_lgr.is_long_pressing()) {
            sprocess() ;
        }
    }
    void OpenNewTab::sprocess(const core::CharLogger& UNUSED(parent_lgr)) {
        sprocess() ;
    }


    //CloseCurrentTab
    CloseCurrentTab::CloseCurrentTab()
    : BindedFuncCreator("close_current_tab")
    {}
    void CloseCurrentTab::sprocess() {
        util::pushup(KEYCODE_LCTRL, KEYCODE_F4) ;
    }
    void CloseCurrentTab::sprocess(core::NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess() ;
        }
    }
    void CloseCurrentTab::sprocess(const core::CharLogger& UNUSED(parent_lgr)) {
        sprocess() ;
    }
}
