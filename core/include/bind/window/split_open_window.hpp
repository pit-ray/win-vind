#ifndef _SPLIT_OPEN_WINDOW_HPP
#define _SPLIT_OPEN_WINDOW_HPP

#include "bind/base/binded_func_creator.hpp"

namespace vind
{
    struct OpenNewCurWinWithHorizontalSplit : public BindedFuncCreator<OpenNewCurWinWithHorizontalSplit> {
        explicit OpenNewCurWinWithHorizontalSplit() ;
        static void sprocess() ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;

    struct OpenNewCurWinWithVerticalSplit : public BindedFuncCreator<OpenNewCurWinWithVerticalSplit> {
        explicit OpenNewCurWinWithVerticalSplit() ;
        static void sprocess() ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;
}

#endif
