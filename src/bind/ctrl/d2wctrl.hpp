#ifndef _DEDICATE_TO_WINDOW_CTRL_HPP
#define _DEDICATE_TO_WINDOW_CTRL_HPP

#include "bind/bindedfunc.hpp"

namespace vind
{
    namespace bind
    {
        struct EnableTargetingOfDedicate2Window : public BindedFuncVoid<EnableTargetingOfDedicate2Window> {
            explicit EnableTargetingOfDedicate2Window() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;

        struct DisableTargetingOfDedicate2Window : public BindedFuncVoid<DisableTargetingOfDedicate2Window> {
            explicit DisableTargetingOfDedicate2Window() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;
    }
}

#endif
