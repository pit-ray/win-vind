#ifndef _JUMP_CURSOR_HPP
#define _JUMP_CURSOR_HPP

#include "bind/base/binded_func_creator.hpp"

namespace vind
{
    struct Jump2Left : public BindedFuncCreator<Jump2Left> {
        explicit Jump2Left() ;
        static void sprocess() ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;

    struct Jump2Right : public BindedFuncCreator<Jump2Right> {
        explicit Jump2Right() ;
        static void sprocess() ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;

    struct Jump2Top : public BindedFuncCreator<Jump2Top> {
        explicit Jump2Top() ;
        static void sprocess() ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;

    struct Jump2Bottom : public BindedFuncCreator<Jump2Bottom> {
        explicit Jump2Bottom() ;
        static void sprocess() ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;

    struct Jump2XCenter : public BindedFuncCreator<Jump2XCenter> {
        explicit Jump2XCenter() ;
        static void sprocess() ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;

    struct Jump2YCenter : public BindedFuncCreator<Jump2YCenter> {
        explicit Jump2YCenter() ;
        static void sprocess() ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;
}
#endif
