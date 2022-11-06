#include "guictrl.hpp"

#include <windows.h>

#include "util/def.hpp"


namespace vind
{
    namespace bind
    {
        ShowConfigGUI::ShowConfigGUI()
        : BindedFuncVoid("show_config_gui")
        {}
        void ShowConfigGUI::sprocess(
                std::uint16_t UNUSED(count),
                const std::string& UNUSED(args)) {
            /*
             * NOT IMPLEMENTED
             */
        }

        ExitConfigGUI::ExitConfigGUI()
        : BindedFuncFlex("exit_config_gui")
        {}
        SystemCall ExitConfigGUI::sprocess(
                std::uint16_t UNUSED(count),
                const std::string& UNUSED(args)) {
            return SystemCall::TERMINATE ;
        }
    }
}
