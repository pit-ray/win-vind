#include "select.hpp"

#include "bind/safe_repeater.hpp"
#include "core/charlogger.hpp"
#include "core/inputgate.hpp"
#include "core/ntypelogger.hpp"
#include "util/def.hpp"
#include "util/mouse.hpp"

namespace vind
{
    namespace bind
    {
        //SelectAll
        SelectAll::SelectAll()
        : BindedFuncVoid("select_all")
        {}

        void SelectAll::sprocess() {
            util::click(KEYCODE_MOUSE_LEFT) ;
            core::InputGate::get_instance().pushup(
                    KEYCODE_LCTRL, KEYCODE_A) ;
        }
        void SelectAll::sprocess(core::NTypeLogger& parent_lgr) {
            if(!parent_lgr.is_long_pressing()) {
                sprocess() ;
            }
        }
        void SelectAll::sprocess(const core::CharLogger& UNUSED(parent_lgr)) {
            sprocess() ;
        }


        //ForwardUINavigation
        ForwardUINavigation::ForwardUINavigation()
        : BindedFuncVoid("forward_ui_navigation")
        {}
        void ForwardUINavigation::sprocess(unsigned int repeat_num) {
            safe_for(repeat_num, [] {
                core::InputGate::get_instance().pushup(KEYCODE_TAB) ;
            }) ;
        }
        void ForwardUINavigation::sprocess(core::NTypeLogger& parent_lgr) {
            if(!parent_lgr.is_long_pressing()) {
                sprocess(parent_lgr.get_head_num()) ;
            }
        }
        void ForwardUINavigation::sprocess(const core::CharLogger& UNUSED(parent_lgr)) {
            sprocess() ;
        }


        //BackwardUINavigation
        BackwardUINavigation::BackwardUINavigation()
        : BindedFuncVoid("backward_ui_navigation")
        {}
        void BackwardUINavigation::sprocess(unsigned int repeat_num) {
            safe_for(repeat_num, [] {
                core::InputGate::get_instance().pushup(
                        KEYCODE_LSHIFT, KEYCODE_TAB) ;
            }) ;
        }
        void BackwardUINavigation::sprocess(core::NTypeLogger& parent_lgr) {
            if(!parent_lgr.is_long_pressing()) {
                sprocess(parent_lgr.get_head_num()) ;
            }
        }
        void BackwardUINavigation::sprocess(const core::CharLogger& UNUSED(parent_lgr)) {
            sprocess() ;
        }


        //DecideFocusedUIObject
        DecideFocusedUIObject::DecideFocusedUIObject()
        : BindedFuncVoid("decide_focused_ui_object")
        {}
        void DecideFocusedUIObject::sprocess() {
            core::InputGate::get_instance().pushup(KEYCODE_SPACE) ;
        }
        void DecideFocusedUIObject::sprocess(core::NTypeLogger& parent_lgr) {
            if(!parent_lgr.is_long_pressing()) {
                sprocess() ;
            }
        }
        void DecideFocusedUIObject::sprocess(const core::CharLogger& UNUSED(parent_lgr)) {
            sprocess() ;
        }
    }
}
