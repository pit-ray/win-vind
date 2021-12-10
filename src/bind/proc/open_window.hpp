#ifndef _OPEN_WINDOW_HPP
#define _OPEN_WINDOW_HPP

#include "bind/binded_func.hpp"

namespace vind
{
    namespace bind
    {
        struct OpenNewWindow : public BindedFuncVoid<OpenNewWindow> {
            explicit OpenNewWindow() ;
            static void sprocess() ;
            static void sprocess(core::NTypeLogger& parent_lgr) ;
            static void sprocess(const core::CharLogger& parent_lgr) ;
        } ;
    }
}

#endif
