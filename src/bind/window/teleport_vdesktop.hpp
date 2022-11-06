#ifndef _TELEPORT_VDESKTOP_HPP
#define _TELEPORT_VDESKTOP_HPP

#include "bind/bindedfunc.hpp"

namespace vind
{
    namespace bind
    {
        struct TeleportOverVDesktop : public BindedFuncVoid<TeleportOverVDesktop> {
            explicit TeleportOverVDesktop() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;
    }
}

#endif
