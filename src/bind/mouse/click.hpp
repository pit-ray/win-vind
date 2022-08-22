#ifndef _CLICK_HPP
#define _CLICK_HPP
#include "bind/bindedfunc.hpp"

namespace vind
{
    namespace bind
    {
        struct ClickLeft : public BindedFuncVoid<ClickLeft> {
            explicit ClickLeft() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;


        struct ClickRight : public BindedFuncVoid<ClickRight> {
            explicit ClickRight() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;
    }
}

#endif
