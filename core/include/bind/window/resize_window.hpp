#ifndef _RESIZE_WINDOW_HPP
#define _RESIZE_WINDOW_HPP

#include "bind/base/binded_func_creator.hpp"

namespace vind
{
    struct ResizeWindowWidth: public BindedFuncCreator<ResizeWindowWidth> {
        explicit ResizeWindowWidth() ;
        static void sprocess(long width=0) ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;

    struct IncreaseWindowWidth : public BindedFuncCreator<IncreaseWindowWidth> {
        explicit IncreaseWindowWidth() ;
        static void sprocess(long delta=0) ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;

    struct DecreaseWindowWidth: public BindedFuncCreator<DecreaseWindowWidth> {
        explicit DecreaseWindowWidth() ;
        static void sprocess(long delta=0) ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;

    struct ResizeWindowHeight : public BindedFuncCreator<ResizeWindowHeight> {
        explicit ResizeWindowHeight() ;
        static void sprocess(long height=0) ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;

    struct IncreaseWindowHeight : public BindedFuncCreator<IncreaseWindowHeight> {
        explicit IncreaseWindowHeight() ;
        static void sprocess(long delta=0) ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;

    struct DecreaseWindowHeight : public BindedFuncCreator<DecreaseWindowHeight> {
        explicit DecreaseWindowHeight() ;
        static void sprocess(long delta=0) ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;
}

#endif
