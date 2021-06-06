#ifndef _JUMP_ACTWIN_HPP
#define _JUMP_ACTWIN_HPP

#include "bind/binded_func_creator.hpp"

namespace vind
{
    struct JumpToActiveWindow : public BindedFuncCreator<JumpToActiveWindow> {
        explicit JumpToActiveWindow() ;
        static void sprocess() ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;
}

#endif
