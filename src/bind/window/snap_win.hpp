#ifndef _SNAP_WINDOW_HPP
#define _SNAP_WINDOW_HPP

#include "bind/bindedfunc.hpp"

namespace vind
{
    namespace bind
    {
        struct SnapCurrentWindow2Left : public BindedFuncVoid<SnapCurrentWindow2Left> {
            explicit SnapCurrentWindow2Left() ;
            static void sprocess() ;
            static void sprocess(core::NTypeLogger& parent_lgr) ;
            static void sprocess(const core::CharLogger& parent_lgr) ;
        } ;

        struct SnapCurrentWindow2Right : public BindedFuncVoid<SnapCurrentWindow2Right> {
            explicit SnapCurrentWindow2Right() ;
            static void sprocess() ;
            static void sprocess(core::NTypeLogger& parent_lgr) ;
            static void sprocess(const core::CharLogger& parent_lgr) ;
        } ;


        struct SnapCurrentWindow2Top : public BindedFuncVoid<SnapCurrentWindow2Top> {
            explicit SnapCurrentWindow2Top() ;
            static void sprocess() ;
            static void sprocess(core::NTypeLogger& parent_lgr) ;
            static void sprocess(const core::CharLogger& parent_lgr) ;
        } ;

        struct SnapCurrentWindow2Bottom : public BindedFuncVoid<SnapCurrentWindow2Bottom> {
            explicit SnapCurrentWindow2Bottom() ;
            static void sprocess() ;
            static void sprocess(core::NTypeLogger& parent_lgr) ;
            static void sprocess(const core::CharLogger& parent_lgr) ;
        } ;
    }
}

#endif
