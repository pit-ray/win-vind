#ifndef _SELECT_HPP
#define _SELECT_HPP
#include "bind/binded_func_creator.hpp"

namespace vind
{
    struct SelectAll : public BindedFuncCreator<SelectAll> {
        explicit SelectAll() ;
        static void sprocess() ;
        static void sprocess(core::NTypeLogger& parent_lgr) ;
        static void sprocess(const core::CharLogger& parent_lgr) ;
    } ;

    struct ForwardUINavigation : public BindedFuncCreator<ForwardUINavigation> {
        explicit ForwardUINavigation() ;
        static void sprocess(unsigned int repeat_num=1) ;
        static void sprocess(core::NTypeLogger& parent_lgr) ;
        static void sprocess(const core::CharLogger& parent_lgr) ;
    } ;

    struct BackwardUINavigation : public BindedFuncCreator<BackwardUINavigation> {
        explicit BackwardUINavigation() ;
        static void sprocess(unsigned int repeat_num=1) ;
        static void sprocess(core::NTypeLogger& parent_lgr) ;
        static void sprocess(const core::CharLogger& parent_lgr) ;
    } ;

    struct DecideFocusedUIObject : public BindedFuncCreator<DecideFocusedUIObject> {
        explicit DecideFocusedUIObject() ;
        static void sprocess() ;
        static void sprocess(core::NTypeLogger& parent_lgr) ;
        static void sprocess(const core::CharLogger& parent_lgr) ;
    } ;
}

#endif
