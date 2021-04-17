#ifndef _CLOSE_WINDOW_HPP
#define _CLOSE_WINDOW_HPP

#include "bind/base/binded_func_creator.hpp"

namespace vind
{
    struct CloseCurrentWindow : public BindedFuncCreator<CloseCurrentWindow> {
        explicit CloseCurrentWindow() ;
        static void sprocess() ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;
}

#endif
