#ifndef _SPLIT_OPEN_WINDOW_HPP
#define _SPLIT_OPEN_WINDOW_HPP

#include "bind/bindedfunc.hpp"

namespace vind
{
    namespace bind
    {
        struct OpenNewWindowWithHorizontalSplit : public BindedFuncVoid<OpenNewWindowWithHorizontalSplit> {
            explicit OpenNewWindowWithHorizontalSplit() ;
            static void sprocess() ;
            static void sprocess(core::NTypeLogger& parent_lgr) ;
            static void sprocess(const core::CharLogger& parent_lgr) ;
        } ;

        struct OpenNewWindowWithVerticalSplit : public BindedFuncVoid<OpenNewWindowWithVerticalSplit> {
            explicit OpenNewWindowWithVerticalSplit() ;
            static void sprocess() ;
            static void sprocess(core::NTypeLogger& parent_lgr) ;
            static void sprocess(const core::CharLogger& parent_lgr) ;
        } ;
    }
}

#endif
