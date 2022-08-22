#include "select.hpp"

#include "bind/saferepeat.hpp"
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

        void SelectAll::sprocess(
                std::uint16_t count, const std::string& args) {
            util::click(KEYCODE_MOUSE_LEFT) ;
            core::InputGate::get_instance().pushup(
                    KEYCODE_LCTRL, KEYCODE_A) ;
        }

        //ForwardUINavigation
        ForwardUINavigation::ForwardUINavigation()
        : BindedFuncVoid("forward_ui_navigation")
        {}
        void ForwardUINavigation::sprocess(
                std::uint16_t count, const std::string& args) {
            safe_for(count, [] {
                core::InputGate::get_instance().pushup(KEYCODE_TAB) ;
            }) ;
        }

        //BackwardUINavigation
        BackwardUINavigation::BackwardUINavigation()
        : BindedFuncVoid("backward_ui_navigation")
        {}
        void BackwardUINavigation::sprocess(
                std::uint16_t count, const std::string& args) {
            safe_for(count, [] {
                core::InputGate::get_instance().pushup(
                        KEYCODE_LSHIFT, KEYCODE_TAB) ;
            }) ;
        }

        //DecideFocusedUIObject
        DecideFocusedUIObject::DecideFocusedUIObject()
        : BindedFuncVoid("decide_focused_ui_object")
        {}
        void DecideFocusedUIObject::sprocess(
                std::uint16_t count, const std::string& args) {
            core::InputGate::get_instance().pushup(KEYCODE_SPACE) ;
        }
    }
}
