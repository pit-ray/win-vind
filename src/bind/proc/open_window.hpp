#ifndef _OPEN_WINDOW_HPP
#define _OPEN_WINDOW_HPP

#include "bind/binded_func_creator.hpp"

namespace vind
{
    namespace bind
    {
        struct OpenNewWindow : public BindedFuncCreator<OpenNewWindow> {
            explicit OpenNewWindow() ;
            static void sprocess() ;
            static void sprocess(core::NTypeLogger& parent_lgr) ;
            static void sprocess(const core::CharLogger& parent_lgr) ;
        } ;
    }
}

#endif
