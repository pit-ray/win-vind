#ifndef _SELECT_WINDOW_HPP
#define _SELECT_WINDOW_HPP

#include "bind/binded_func_creator.hpp"

namespace vind
{
    struct SelectLeftWindow : public BindedFuncCreator<SelectLeftWindow> {
        explicit SelectLeftWindow() ;
        static void sprocess() ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;

    struct SelectRightWindow : public BindedFuncCreator<SelectRightWindow> {
        explicit SelectRightWindow() ;
        static void sprocess() ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;

    struct SelectUpperWindow : public BindedFuncCreator<SelectUpperWindow> {
        explicit SelectUpperWindow() ;
        static void sprocess() ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;

    struct SelectLowerWindow : public BindedFuncCreator<SelectLowerWindow> {
        explicit SelectLowerWindow() ;
        static void sprocess() ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;
}

#endif
