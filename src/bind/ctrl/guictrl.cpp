#include "guictrl.hpp"

#include <windows.h>

#include "bind/mouse/jump_actwin.hpp"
#include "core/ntypelogger.hpp"
#include "util/def.hpp"


namespace vind
{
    namespace bind
    {
        //ShowConfigGUI
        ShowConfigGUI::ShowConfigGUI()
        : BindedFuncVoid("show_config_gui")
        {}
        void ShowConfigGUI::sprocess(
                std::uint16_t UNUSED(count), const std::string& UNUSED(args)) {
            /*
             * NOT IMPLEMENTED
             */
        }

        //ExitConfigGUI
        ExitConfigGUI::ExitConfigGUI()
        : BindedFuncFlex("exit_config_gui")
        {}
        SystemCall ExitConfigGUI::sprocess(
                std::uint16_t UNUSED(count), const std::string& UNUSED(args)) {
            return SystemCall::TERMINATE ;
        }
    }
}
