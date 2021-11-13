#ifndef _CLICK_HPP
#define _CLICK_HPP
#include "bind/binded_func_creator.hpp"

namespace vind
{
    struct ClickLeft : public BindedFuncCreator<ClickLeft> {
        explicit ClickLeft() ;
        static void sprocess(unsigned int repeat_num=1) ;
        static void sprocess(core::NTypeLogger& parent_lgr) ;
        static void sprocess(const core::CharLogger& parent_lgr) ;
    } ;


    struct ClickRight : public BindedFuncCreator<ClickRight> {
        explicit ClickRight() ;
        static void sprocess(unsigned int repeat_num=1) ;
        static void sprocess(core::NTypeLogger& parent_lgr) ;
        static void sprocess(const core::CharLogger& parent_lgr) ;
    } ;
}

#endif
