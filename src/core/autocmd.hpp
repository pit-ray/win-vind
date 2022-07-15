#ifndef _AUTOCMD_HPP
#define _AUTOCMD_HPP

#include <string>


namespace vind
{
    namespace core
    {
        enum class AutoCmdEvent {
            WIN_ENTER,
            WIN_LEAVE,
            GUI_NORMAL_ENTER,
            GUI_NORMAL_LEAVE,
            GUI_VISUAL_ENTER,
            GUI_VISUAL_LEAVE,
            EDI_NORMAL_ENTER,
            EDI_NORMAL_LEAVE,
            EDI_VISUAL_ENTER,
            EDI_VISUAL_LEAVE,
            INSERT_ENTER,
            INSERT_LEAVE,
            RESIDENT_ENTER,
            RESIDENT_LEAVE,
            CMDLINE_ENTER,
            CMDLINE_LEAVE
        } ;


        AutoCmdEvent get_autocmd_event(const std::string& event_name) ;
    }
}

#endif
