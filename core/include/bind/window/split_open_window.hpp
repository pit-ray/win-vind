#ifndef _SPLIT_OPEN_WINDOW_HPP
#define _SPLIT_OPEN_WINDOW_HPP

#include "bind/base/binded_func_with_creator.hpp"

namespace vind
{
    struct OpenNewCurWinWithHorizontalSplit : public BindedFuncCreator<OpenNewCurWinWithHorizontalSplit> {
        static void sprocess() ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
        static const std::string sname() noexcept ;
    } ;

    struct OpenNewCurWinWithVerticalSplit : public BindedFuncCreator<OpenNewCurWinWithVerticalSplit> {
        static void sprocess() ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
        static const std::string sname() noexcept ;
    } ;
}

#endif
