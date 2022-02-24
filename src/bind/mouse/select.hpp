#ifndef _SELECT_HPP
#define _SELECT_HPP
#include "bind/bindedfunc.hpp"

namespace vind
{
    namespace bind
    {
        struct SelectAll : public BindedFuncVoid<SelectAll> {
            explicit SelectAll() ;
            static void sprocess() ;
            static void sprocess(core::NTypeLogger& parent_lgr) ;
            static void sprocess(const core::CharLogger& parent_lgr) ;
        } ;

        struct ForwardUINavigation : public BindedFuncVoid<ForwardUINavigation> {
            explicit ForwardUINavigation() ;
            static void sprocess(unsigned int count=1) ;
            static void sprocess(core::NTypeLogger& parent_lgr) ;
            static void sprocess(const core::CharLogger& parent_lgr) ;
        } ;

        struct BackwardUINavigation : public BindedFuncVoid<BackwardUINavigation> {
            explicit BackwardUINavigation() ;
            static void sprocess(unsigned int count=1) ;
            static void sprocess(core::NTypeLogger& parent_lgr) ;
            static void sprocess(const core::CharLogger& parent_lgr) ;
        } ;

        struct DecideFocusedUIObject : public BindedFuncVoid<DecideFocusedUIObject> {
            explicit DecideFocusedUIObject() ;
            static void sprocess() ;
            static void sprocess(core::NTypeLogger& parent_lgr) ;
            static void sprocess(const core::CharLogger& parent_lgr) ;
        } ;
    }
}

#endif
