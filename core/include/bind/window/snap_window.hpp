#ifndef _SNAP_WINDOW_HPP
#define _SNAP_WINDOW_HPP

#include "bind/base/binded_func_creator.hpp"

namespace vind
{
    struct SnapCurrentWindow2Left : public BindedFuncCreator<SnapCurrentWindow2Left> {
        explicit SnapCurrentWindow2Left() ;
        static void sprocess() ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;

    struct SnapCurrentWindow2Right : public BindedFuncCreator<SnapCurrentWindow2Right> {
        explicit SnapCurrentWindow2Right() ;
        static void sprocess() ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;


    struct SnapCurrentWindow2Top : public BindedFuncCreator<SnapCurrentWindow2Top> {
        explicit SnapCurrentWindow2Top() ;
        static void sprocess() ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;

    struct SnapCurrentWindow2Bottom : public BindedFuncCreator<SnapCurrentWindow2Bottom> {
        explicit SnapCurrentWindow2Bottom() ;
        static void sprocess() ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;
}

#endif
