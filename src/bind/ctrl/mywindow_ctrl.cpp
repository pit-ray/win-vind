#include "mywindow_ctrl.hpp"

#include <windows.h>

#include "bind/mouse/jump_actwin.hpp"
#include "core/ntype_logger.hpp"
#include "util/def.hpp"


namespace vind
{
    namespace bind
    {
        //ShowConfigGUI
        ShowConfigGUI::ShowConfigGUI()
        : BindedFuncVoid("show_config_gui")
        {}
        void ShowConfigGUI::sprocess() {
            /*
             * NOT IMPLEMENTED
             */
        }
        void ShowConfigGUI::sprocess(core::NTypeLogger& parent_lgr) {
            if(parent_lgr.is_long_pressing()) {
                sprocess() ;
            }
        }
        void ShowConfigGUI::sprocess(const core::CharLogger& UNUSED(parent_lgr)) {
            sprocess() ;
        }


        //ExitConfigGUI
        ExitConfigGUI::ExitConfigGUI()
        : BindedFuncFlex("exit_config_gui")
        {}
        core::SystemCall ExitConfigGUI::sprocess() {
            return core::SystemCall::TERMINATE ;
        }
        core::SystemCall ExitConfigGUI::sprocess(core::NTypeLogger& parent_lgr) {
            if(!parent_lgr.is_long_pressing()) {
                return sprocess() ;
            }
            return core::SystemCall::NOTHING ;
        }
        core::SystemCall ExitConfigGUI::sprocess(const core::CharLogger& UNUSED(parent_lgr)) {
            return sprocess() ;
        }
    }
}
