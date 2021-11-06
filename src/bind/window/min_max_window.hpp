#ifndef _MIN_MAX_WINDOW_HPP
#define _MIN_MAX_WINDOW_HPP

#include "bind/binded_func_creator.hpp"

namespace vind
{
    struct MinimizeCurrentWindow : public BindedFuncCreator<MinimizeCurrentWindow> {
        explicit MinimizeCurrentWindow() ;
        static void sprocess(unsigned int repeat_num=1) ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;

    struct MaximizeCurrentWindow : public BindedFuncCreator<MaximizeCurrentWindow> {
        explicit MaximizeCurrentWindow() ;
        static void sprocess(unsigned int repeat_num=1) ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;
}

#endif
