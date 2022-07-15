#include "autocmd.hpp"
#include <unordered_map>
#include <vector>

namespace
{
    class AutoCmd {

    } ;

    struct AutoPat {
        std::vector<AutoCmd> cmds_ ;
    } ;
}

namespace vind
{
    namespace core
    {
        AutoCmdEvent get_autocmd_event(const std::string& event_name) {
            static std::unordered_map<std::string, AutoCmdEvent> names {
                {"WinEnter",        AutoCmdEvent::WIN_ENTER},
                {"WinLeave",        AutoCmdEvent::WIN_LEAVE},
                {"GUINormalEnter",  AutoCmdEvent::GUI_NORMAL_ENTER},
                {"GUINormalLeave",  AutoCmdEvent::GUI_NORMAL_LEAVE},
                {"GUIVisualEnter",  AutoCmdEvent::GUI_VISUAL_ENTER},
                {"GUIVisualLeave",  AutoCmdEvent::GUI_VISUAL_LEAVE},
                {"EdiNormalEnter",  AutoCmdEvent::EDI_NORMAL_ENTER},
                {"EdiNormalLeave",  AutoCmdEvent::EDI_NORMAL_LEAVE},
                {"EdiVisualEnter",  AutoCmdEvent::EDI_VISUAL_ENTER},
                {"EdiVisualLeave",  AutoCmdEvent::EDI_VISUAL_LEAVE},
                {"InsertEnter",     AutoCmdEvent::INSERT_ENTER},
                {"InsertLeave",     AutoCmdEvent::INSERT_LEAVE},
                {"ResidentEnter",   AutoCmdEvent::RESIDENT_ENTER},
                {"ResidentLeave",   AutoCmdEvent::RESIDENT_LEAVE},
                {"CmdLineEnter",    AutoCmdEvent::CMDLINE_ENTER},
                {"CmdLineLeave",    AutoCmdEvent::CMDLINE_LEAVE}
            } ;
            return names.at(event_name) ;
        }

    }
}
