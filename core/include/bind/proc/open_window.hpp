#ifndef _OPEN_WINDOW_HPP
#define _OPEN_WINDOW_HPP

#include "bind/binded_func_creator.hpp"

namespace vind
{
    struct OpenNewWindow : public BindedFuncCreator<OpenNewWindow> {
        explicit OpenNewWindow() ;
        static void sprocess() ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;
}

#endif
