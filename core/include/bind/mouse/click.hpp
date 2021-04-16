#ifndef _CLICK_HPP
#define _CLICK_HPP
#include "bind/base/binded_func_with_creator.hpp"

namespace vind
{
    struct ClickLeft : public BindedFuncCreator<ClickLeft> {
        explicit ClickLeft() ;
        static void sprocess(unsigned int repeat_num=1) ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;


    struct ClickRight : public BindedFuncCreator<ClickRight> {
        explicit ClickRight() ;
        static void sprocess(unsigned int repeat_num=1) ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;
}

#endif
