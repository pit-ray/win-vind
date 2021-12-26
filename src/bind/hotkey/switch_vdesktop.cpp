#include "switch_vdesktop.hpp"

#include "bind/safe_repeater.hpp"
#include "core/inputgate.hpp"
#include "core/ntypelogger.hpp"
#include "util/def.hpp"

namespace vind
{
    namespace bind
    {
        //SwitchVDesktop2Left
        SwitchVDesktop2Left::SwitchVDesktop2Left()
        : BindedFuncVoid("switch_to_left_vdesktop")
        {}
        void SwitchVDesktop2Left::sprocess(unsigned int repeat_num) {
            safe_for(repeat_num, [] {
                core::InputGate::get_instance().pushup(
                        KEYCODE_LCTRL, KEYCODE_LWIN, KEYCODE_LEFT) ;
            }) ;
        }
        void SwitchVDesktop2Left::sprocess(core::NTypeLogger& parent_lgr) {
            if(!parent_lgr.is_long_pressing()) {
                sprocess(parent_lgr.get_head_num()) ;
            }
        }
        void SwitchVDesktop2Left::sprocess(const core::CharLogger& UNUSED(parent_lgr)) {
            sprocess(1) ;
        }


        //SwitchVDesktop2Right
        SwitchVDesktop2Right::SwitchVDesktop2Right()
        : BindedFuncVoid("switch_to_right_vdesktop")
        {}
        void SwitchVDesktop2Right::sprocess(unsigned int repeat_num) {
            safe_for(repeat_num, [] {
                core::InputGate::get_instance().pushup(
                        KEYCODE_LCTRL, KEYCODE_LWIN, KEYCODE_RIGHT) ;
            }) ;
        }
        void SwitchVDesktop2Right::sprocess(core::NTypeLogger& parent_lgr) {
            if(!parent_lgr.is_long_pressing()) {
                sprocess(parent_lgr.get_head_num()) ;
            }
        }
        void SwitchVDesktop2Right::sprocess(const core::CharLogger& UNUSED(parent_lgr)) {
            sprocess(1) ;
        }


        //CreateNewVDesktop
        CreateNewVDesktop::CreateNewVDesktop()
        : BindedFuncVoid("create_new_vdesktop")
        {}
        void CreateNewVDesktop::sprocess() {
            core::InputGate::get_instance().pushup(
                    KEYCODE_LCTRL, KEYCODE_LWIN, KEYCODE_D) ;
        }
        void CreateNewVDesktop::sprocess(core::NTypeLogger& parent_lgr) {
            if(!parent_lgr.is_long_pressing()) {
                sprocess() ;
            }
        }
        void CreateNewVDesktop::sprocess(const core::CharLogger& UNUSED(parent_lgr)) {
            sprocess() ;
        }


        //CloseCurrentVDesktop
        CloseCurrentVDesktop::CloseCurrentVDesktop()
        : BindedFuncVoid("close_current_vdesktop")
        {}
        void CloseCurrentVDesktop::sprocess() {
            core::InputGate::get_instance().pushup(
                    KEYCODE_LCTRL, KEYCODE_LWIN, KEYCODE_F4) ;
        }
        void CloseCurrentVDesktop::sprocess(core::NTypeLogger& parent_lgr) {
            if(!parent_lgr.is_long_pressing()) {
                sprocess() ;
            }
        }
        void CloseCurrentVDesktop::sprocess(const core::CharLogger& UNUSED(parent_lgr)) {
            sprocess() ;
        }


        //TaskView
        TaskView::TaskView()
        : BindedFuncVoid("taskview")
        {}
        void TaskView::sprocess() {
            core::InputGate::get_instance().pushup(
                    KEYCODE_LWIN, KEYCODE_TAB) ;
        }
        void TaskView::sprocess(core::NTypeLogger& parent_lgr) {
            if(!parent_lgr.is_long_pressing()) {
                sprocess() ;
            }
        }
        void TaskView::sprocess(const core::CharLogger& UNUSED(parent_lgr)) {
            sprocess() ;
        }
    }
}
