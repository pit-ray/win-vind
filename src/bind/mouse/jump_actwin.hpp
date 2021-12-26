#ifndef _JUMP_ACTWIN_HPP
#define _JUMP_ACTWIN_HPP

#include "bind/bindedfunc.hpp"

namespace vind
{
    namespace bind
    {
        struct JumpToActiveWindow : public BindedFuncVoid<JumpToActiveWindow> {
            explicit JumpToActiveWindow() ;
            static void sprocess() ;
            static void sprocess(core::NTypeLogger& parent_lgr) ;
            static void sprocess(const core::CharLogger& parent_lgr) ;
        } ;
    }
}

#endif
