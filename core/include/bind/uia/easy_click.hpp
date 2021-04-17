#ifndef _EASY_CLICK_HPP
#define _EASY_CLICK_HPP

#include "bind/base/binded_func_with_creator.hpp"

namespace vind
{
    struct EasyClickLeft : public BindedFuncCreator<EasyClickLeft> {
        explicit EasyClickLeft() ;
        static void sprocess() ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;

    struct EasyClickRight : public BindedFuncCreator<EasyClickRight> {
        explicit EasyClickRight() ;
        static void sprocess() ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;

    struct EasyClickMid : public BindedFuncCreator<EasyClickMid> {
        explicit EasyClickMid() ;
        static void sprocess() ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;

    struct EasyClickHover : public BindedFuncCreator<EasyClickHover> {
        explicit EasyClickHover() ;
        static void sprocess() ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;
}

#endif
