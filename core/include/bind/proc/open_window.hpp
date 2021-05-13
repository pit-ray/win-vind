#ifndef _OPEN_WINDOW_HPP
#define _OPEN_WINDOW_HPP

#include "bind/binded_func_creator.hpp"

namespace vind
{
    struct OpenNewCurrentWindow : public BindedFuncCreator<OpenNewCurrentWindow> {
        explicit OpenNewCurrentWindow() ;
        static void sprocess() ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;
}

#endif
