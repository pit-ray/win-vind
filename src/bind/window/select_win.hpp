#ifndef _SELECT_WINDOW_HPP
#define _SELECT_WINDOW_HPP

#include "bind/bindedfunc.hpp"

namespace vind
{
    namespace bind
    {
        struct SelectLeftWindow : public BindedFuncVoid<SelectLeftWindow> {
            explicit SelectLeftWindow() ;
            static void sprocess() ;
            static void sprocess(core::NTypeLogger& parent_lgr) ;
            static void sprocess(const core::CharLogger& parent_lgr) ;
        } ;

        struct SelectRightWindow : public BindedFuncVoid<SelectRightWindow> {
            explicit SelectRightWindow() ;
            static void sprocess() ;
            static void sprocess(core::NTypeLogger& parent_lgr) ;
            static void sprocess(const core::CharLogger& parent_lgr) ;
        } ;

        struct SelectUpperWindow : public BindedFuncVoid<SelectUpperWindow> {
            explicit SelectUpperWindow() ;
            static void sprocess() ;
            static void sprocess(core::NTypeLogger& parent_lgr) ;
            static void sprocess(const core::CharLogger& parent_lgr) ;
        } ;

        struct SelectLowerWindow : public BindedFuncVoid<SelectLowerWindow> {
            explicit SelectLowerWindow() ;
            static void sprocess() ;
            static void sprocess(core::NTypeLogger& parent_lgr) ;
            static void sprocess(const core::CharLogger& parent_lgr) ;
        } ;
    }
}

#endif
