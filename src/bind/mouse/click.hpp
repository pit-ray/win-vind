#ifndef _CLICK_HPP
#define _CLICK_HPP
#include "bind/bindedfunc.hpp"

namespace vind
{
    namespace bind
    {
        struct ClickLeft : public BindedFuncVoid<ClickLeft> {
            explicit ClickLeft() ;
            static void sprocess(unsigned int count=1) ;
            static void sprocess(core::NTypeLogger& parent_lgr) ;
            static void sprocess(const core::CharLogger& parent_lgr) ;
        } ;


        struct ClickRight : public BindedFuncVoid<ClickRight> {
            explicit ClickRight() ;
            static void sprocess(unsigned int count=1) ;
            static void sprocess(core::NTypeLogger& parent_lgr) ;
            static void sprocess(const core::CharLogger& parent_lgr) ;
        } ;
    }
}

#endif
