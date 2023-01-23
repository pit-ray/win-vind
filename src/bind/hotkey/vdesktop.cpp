#include "vdesktop.hpp"

#include "bind/saferepeat.hpp"
#include "core/inputgate.hpp"
#include "util/def.hpp"

namespace vind
{
    namespace bind
    {
        //SwitchVDesktop2Left
        SwitchVDesktop2Left::SwitchVDesktop2Left()
        : BindedFuncVoid("switch_to_left_vdesktop")
        {}
        void SwitchVDesktop2Left::sprocess(
                std::uint16_t count,
                const std::string& UNUSED(args)) {
            safe_for(count, [] {
                core::InputGate::get_instance().pushup(
                        KEYCODE_LCTRL, KEYCODE_LWIN, KEYCODE_LEFT) ;
            }) ;
        }

        //SwitchVDesktop2Right
        SwitchVDesktop2Right::SwitchVDesktop2Right()
        : BindedFuncVoid("switch_to_right_vdesktop")
        {}
        void SwitchVDesktop2Right::sprocess(
                std::uint16_t count,
                const std::string& UNUSED(args)) {
            safe_for(count, [] {
                core::InputGate::get_instance().pushup(
                        KEYCODE_LCTRL, KEYCODE_LWIN, KEYCODE_RIGHT) ;
            }) ;
        }

        //CreateNewVDesktop
        CreateNewVDesktop::CreateNewVDesktop()
        : BindedFuncVoid("create_new_vdesktop")
        {}
        void CreateNewVDesktop::sprocess(
                std::uint16_t UNUSED(count),
                const std::string& UNUSED(args)) {
            core::InputGate::get_instance().pushup(
                    KEYCODE_LCTRL, KEYCODE_LWIN, KEYCODE_D) ;
        }

        //CloseCurrentVDesktop
        CloseCurrentVDesktop::CloseCurrentVDesktop()
        : BindedFuncVoid("close_current_vdesktop")
        {}
        void CloseCurrentVDesktop::sprocess(
                std::uint16_t UNUSED(count),
                const std::string& UNUSED(args)) {
            core::InputGate::get_instance().pushup(
                    KEYCODE_LCTRL, KEYCODE_LWIN, KEYCODE_F4) ;
        }

        //TaskView
        TaskView::TaskView()
        : BindedFuncVoid("taskview")
        {}
        void TaskView::sprocess(
                std::uint16_t UNUSED(count),
                const std::string& UNUSED(args)) {
            core::InputGate::get_instance().pushup(
                    KEYCODE_LWIN, KEYCODE_TAB) ;
        }
    }
}
