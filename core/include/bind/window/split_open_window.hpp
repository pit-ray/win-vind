#ifndef _SPLIT_OPEN_WINDOW_HPP
#define _SPLIT_OPEN_WINDOW_HPP

#include "bind/binded_func_creator.hpp"

namespace vind
{
    struct OpenNewWindowWithHorizontalSplit : public BindedFuncCreator<OpenNewWindowWithHorizontalSplit> {
        explicit OpenNewWindowWithHorizontalSplit() ;
        static void sprocess() ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;

    struct OpenNewWindowWithVerticalSplit : public BindedFuncCreator<OpenNewWindowWithVerticalSplit> {
        explicit OpenNewWindowWithVerticalSplit() ;
        static void sprocess() ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;
}

#endif
