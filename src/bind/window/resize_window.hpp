#ifndef _RESIZE_WINDOW_HPP
#define _RESIZE_WINDOW_HPP

#include "bind/binded_func_creator.hpp"

namespace vind
{
    namespace bind
    {
        struct ResizeWindowWidth: public BindedFuncCreator<ResizeWindowWidth> {
            explicit ResizeWindowWidth() ;
            static void sprocess(long width=0) ;
            static void sprocess(core::NTypeLogger& parent_lgr) ;
            static void sprocess(const core::CharLogger& parent_lgr) ;
        } ;

        struct IncreaseWindowWidth : public BindedFuncCreator<IncreaseWindowWidth> {
            explicit IncreaseWindowWidth() ;
            static void sprocess(long delta=0) ;
            static void sprocess(core::NTypeLogger& parent_lgr) ;
            static void sprocess(const core::CharLogger& parent_lgr) ;
        } ;

        struct DecreaseWindowWidth: public BindedFuncCreator<DecreaseWindowWidth> {
            explicit DecreaseWindowWidth() ;
            static void sprocess(long delta=0) ;
            static void sprocess(core::NTypeLogger& parent_lgr) ;
            static void sprocess(const core::CharLogger& parent_lgr) ;
        } ;

        struct ResizeWindowHeight : public BindedFuncCreator<ResizeWindowHeight> {
            explicit ResizeWindowHeight() ;
            static void sprocess(long height=0) ;
            static void sprocess(core::NTypeLogger& parent_lgr) ;
            static void sprocess(const core::CharLogger& parent_lgr) ;
        } ;

        struct IncreaseWindowHeight : public BindedFuncCreator<IncreaseWindowHeight> {
            explicit IncreaseWindowHeight() ;
            static void sprocess(long delta=0) ;
            static void sprocess(core::NTypeLogger& parent_lgr) ;
            static void sprocess(const core::CharLogger& parent_lgr) ;
        } ;

        struct DecreaseWindowHeight : public BindedFuncCreator<DecreaseWindowHeight> {
            explicit DecreaseWindowHeight() ;
            static void sprocess(long delta=0) ;
            static void sprocess(core::NTypeLogger& parent_lgr) ;
            static void sprocess(const core::CharLogger& parent_lgr) ;
        } ;
    }
}

#endif
