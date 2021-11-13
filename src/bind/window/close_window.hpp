#ifndef _CLOSE_WINDOW_HPP
#define _CLOSE_WINDOW_HPP

#include "bind/binded_func_creator.hpp"

namespace vind
{
    struct CloseCurrentWindow : public BindedFuncCreator<CloseCurrentWindow> {
        explicit CloseCurrentWindow() ;
        static void sprocess() ;
        static void sprocess(core::NTypeLogger& parent_lgr) ;
        static void sprocess(const core::CharLogger& parent_lgr) ;
    } ;
}

#endif
