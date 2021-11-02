#ifndef _TELEPORT_VDESKTOP_HPP
#define _TELEPORT_VDESKTOP_HPP

#include "bind/binded_func_creator.hpp"

namespace vind
{
    struct TeleportOverVDesktop : public BindedFuncCreator<TeleportOverVDesktop> {
        explicit TeleportOverVDesktop() ;
        static void sprocess(unsigned int repeat_num=1) ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;
}

#endif
