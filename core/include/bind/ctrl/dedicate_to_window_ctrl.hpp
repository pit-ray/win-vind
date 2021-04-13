#ifndef _DEDICATE_TO_WINDOW_CTRL_HPP
#define _DEDICATE_TO_WINDOW_CTRL_HPP

#include "bind/base/binded_func_with_creator.hpp"

namespace vind
{
    struct EnableTargetingOfDedicate2Window : public BindedFuncCreator<EnableTargetingOfDedicate2Window> {
        static void sprocess() ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
        static const std::string sname() noexcept ;
    } ;

    struct DisableTargetingOfDedicate2Window : public BindedFuncCreator<DisableTargetingOfDedicate2Window> {
        static void sprocess() ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
        static const std::string sname() noexcept ;
    } ;
}

#endif
