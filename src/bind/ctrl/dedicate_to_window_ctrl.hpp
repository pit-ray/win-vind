#ifndef _DEDICATE_TO_WINDOW_CTRL_HPP
#define _DEDICATE_TO_WINDOW_CTRL_HPP

#include "bind/binded_func_creator.hpp"

namespace vind
{
    struct EnableTargetingOfDedicate2Window : public BindedFuncCreator<EnableTargetingOfDedicate2Window> {
        explicit EnableTargetingOfDedicate2Window() ;
        static void sprocess() ;
        static void sprocess(core::NTypeLogger& parent_lgr) ;
        static void sprocess(const core::CharLogger& parent_lgr) ;
    } ;

    struct DisableTargetingOfDedicate2Window : public BindedFuncCreator<DisableTargetingOfDedicate2Window> {
        explicit DisableTargetingOfDedicate2Window() ;
        static void sprocess() ;
        static void sprocess(core::NTypeLogger& parent_lgr) ;
        static void sprocess(const core::CharLogger& parent_lgr) ;
    } ;
}

#endif
