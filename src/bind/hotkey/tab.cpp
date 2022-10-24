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
        void Switch2LeftTab::sprocess(
                std::uint16_t count,
                const std::string& UNUSED(args)) {
            safe_for(count, [] {
                core::InputGate::get_instance().pushup(
                        KEYCODE_LCTRL, KEYCODE_LSHIFT, KEYCODE_TAB) ;
            }) ;
        }

        //Switch2RightTab
        Switch2RightTab::Switch2RightTab()
        : BindedFuncVoid("switch_to_right_tab")
        {}
        void Switch2RightTab::sprocess(
                std::uint16_t count,
                const std::string& UNUSED(args)) {
            safe_for(count, [] {
                core::InputGate::get_instance().pushup(
                        KEYCODE_LCTRL, KEYCODE_TAB) ;
            }) ;
        }

        //OpenNewTab
        OpenNewTab::OpenNewTab()
        : BindedFuncVoid("open_new_tab")
        {}
        void OpenNewTab::sprocess(
                std::uint16_t UNUSED(count),
                const std::string& UNUSED(args)) {
            core::InputGate::get_instance().pushup(
                    KEYCODE_LCTRL, KEYCODE_T) ;
        }

        //CloseCurrentTab
        CloseCurrentTab::CloseCurrentTab()
        : BindedFuncVoid("close_current_tab")
        {}
        void CloseCurrentTab::sprocess(
                std::uint16_t UNUSED(count),
                const std::string& UNUSED(args)) {
            core::InputGate::get_instance().pushup(
                    KEYCODE_LCTRL, KEYCODE_F4) ;
        }
    }
}
