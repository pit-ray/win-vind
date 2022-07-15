#ifndef _AUTOCMD_HPP
#define _AUTOCMD_HPP

namespace vind
{
    namespace core
    {
        enum class AutocmdEvent {
            WinEnter,
            WinLeave,

            // GUI
            GNormalEnter,
            GNormalLeave,
            GVisualEnter,
            GVisualLeave,

            // Editor
            ENormalEnter,
            ENormalLeave,
            EVisualEnter,
            EVisualLeave,

            InsertEnter,
            InsertLeave,

            ResidentEnter,
            ResidentLeave,

            CmdLineEnter,
            CmdLineLeave
        } ;

    }
}

#endif
