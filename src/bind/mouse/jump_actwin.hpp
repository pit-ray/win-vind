#ifndef _JUMP_ACTWIN_HPP
#define _JUMP_ACTWIN_HPP

#include "bind/bindedfunc.hpp"

namespace vind
{
    namespace bind
    {
        struct JumpToActiveWindow : public BindedFuncVoid<JumpToActiveWindow> {
            explicit JumpToActiveWindow() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;
    }
}

#endif
