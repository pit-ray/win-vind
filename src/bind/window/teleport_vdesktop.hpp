#ifndef _TELEPORT_VDESKTOP_HPP
#define _TELEPORT_VDESKTOP_HPP

#include "bind/bindedfunc.hpp"

namespace vind
{
    namespace bind
    {
        struct TeleportOverVDesktop : public BindedFuncVoid<TeleportOverVDesktop> {
            explicit TeleportOverVDesktop() ;
            static void sprocess(unsigned int repeat_num=1) ;
            static void sprocess(core::NTypeLogger& parent_lgr) ;
            static void sprocess(const core::CharLogger& parent_lgr) ;
        } ;
    }
}

#endif
