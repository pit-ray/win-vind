#ifndef _MIN_MAX_WINDOW_HPP
#define _MIN_MAX_WINDOW_HPP

#include "bind/bindedfunc.hpp"

namespace vind
{
    namespace bind
    {
        struct MinimizeCurrentWindow : public BindedFuncVoid<MinimizeCurrentWindow> {
            explicit MinimizeCurrentWindow() ;
            static void sprocess(unsigned int count=1) ;
            static void sprocess(core::NTypeLogger& parent_lgr) ;
            static void sprocess(const core::CharLogger& parent_lgr) ;
        } ;

        struct MaximizeCurrentWindow : public BindedFuncVoid<MaximizeCurrentWindow> {
            explicit MaximizeCurrentWindow() ;
            static void sprocess(unsigned int count=1) ;
            static void sprocess(core::NTypeLogger& parent_lgr) ;
            static void sprocess(const core::CharLogger& parent_lgr) ;
        } ;
    }
}

#endif
