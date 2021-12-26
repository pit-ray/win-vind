#include "tab.hpp"

#include <windows.h>

#include "bind/mode/change_mode.hpp"
#include "bind/saferepeat.hpp"
#include "core/charlogger.hpp"
#include "core/errlogger.hpp"
#include "core/inputgate.hpp"
#include "core/ntypelogger.hpp"
#include "util/def.hpp"


namespace vind
{
    namespace bind
    {
        //Switch2LeftTab
        Switch2LeftTab::Switch2LeftTab()
        : BindedFuncVoid("switch_to_left_tab")
        {}
        void Switch2LeftTab::sprocess(unsigned int repeat_num) {
            safe_for(repeat_num, [] {
                core::InputGate::get_instance().pushup(
                        KEYCODE_LCTRL, KEYCODE_LSHIFT, KEYCODE_TAB) ;
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
        : BindedFuncVoid("switch_to_right_tab")
        {}
        void Switch2RightTab::sprocess(unsigned repeat_num) {
            safe_for(repeat_num, [] {
                core::InputGate::get_instance().pushup(
                        KEYCODE_LCTRL, KEYCODE_TAB) ;
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
        : BindedFuncVoid("open_new_tab")
        {}
        void OpenNewTab::sprocess() {
            core::InputGate::get_instance().pushup(
                    KEYCODE_LCTRL, KEYCODE_T) ;
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
        : BindedFuncVoid("close_current_tab")
        {}
        void CloseCurrentTab::sprocess() {
            core::InputGate::get_instance().pushup(
                    KEYCODE_LCTRL, KEYCODE_F4) ;
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
}
