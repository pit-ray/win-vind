#include "bind/hotkey/tab_ctrl.hpp"

#include <windows.h>

#include "bind/mode/change_mode.hpp"
#include "bind/safe_repeater.hpp"
#include "err_logger.hpp"
#include "io/keybrd.hpp"
#include "key/char_logger.hpp"
#include "util/def.hpp"

#include "key/ntype_logger.hpp"


namespace vind
{
    //Switch2LeftTab
    Switch2LeftTab::Switch2LeftTab()
    : BindedFuncCreator("switch_to_left_tab")
    {}
    void Switch2LeftTab::sprocess(unsigned int repeat_num) {
        repeater::safe_for(repeat_num, [] {
            keybrd::pushup(KEYCODE_LCTRL, KEYCODE_LSHIFT, KEYCODE_TAB) ;
        }) ;
    }
    void Switch2LeftTab::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess() ;
        }
    }
    void Switch2LeftTab::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess() ;
    }


    //Switch2RightTab
    Switch2RightTab::Switch2RightTab()
    : BindedFuncCreator("switch_to_right_tab")
    {}
    void Switch2RightTab::sprocess(unsigned repeat_num) {
        repeater::safe_for(repeat_num, [] {
            keybrd::pushup(KEYCODE_LCTRL, KEYCODE_TAB) ;
        }) ;
    }
    void Switch2RightTab::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess() ;
        }
    }
    void Switch2RightTab::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess() ;
    }

    //OpenNewTab
    OpenNewTab::OpenNewTab()
    : BindedFuncCreator("open_new_tab")
    {}
    void OpenNewTab::sprocess() {
        keybrd::pushup(KEYCODE_LCTRL, KEYCODE_T) ;
    }
    void OpenNewTab::sprocess(NTypeLogger& parent_lgr) {
        if(parent_lgr.is_long_pressing()) {
            sprocess() ;
        }
    }
    void OpenNewTab::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess() ;
    }


    //CloseCurrentTab
    CloseCurrentTab::CloseCurrentTab()
    : BindedFuncCreator("close_current_tab")
    {}
    void CloseCurrentTab::sprocess() {
        keybrd::pushup(KEYCODE_LCTRL, KEYCODE_F4) ;

        auto hwnd = GetForegroundWindow() ;
        if(hwnd == NULL) {
            throw RUNTIME_EXCEPT("The foreground window is not existed.") ;
        }

        Sleep(500) ; //wait by openning the dialog for saving
        if(hwnd != GetForegroundWindow()) { //opened popup
            Change2Normal::sprocess(true) ;
        }
    }
    void CloseCurrentTab::sprocess(NTypeLogger& parent_lgr) {
        if(!parent_lgr.is_long_pressing()) {
            sprocess() ;
        }
    }
    void CloseCurrentTab::sprocess(const CharLogger& UNUSED(parent_lgr)) {
        sprocess() ;
    }
}
