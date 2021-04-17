#ifndef _CHANGE_MODE_HPP
#define _CHANGE_MODE_HPP

#include "bind/base/binded_func_creator.hpp"

namespace vind
{
    struct Change2Normal : public BindedFuncCreator<Change2Normal> {
        explicit Change2Normal() ;
        static void sprocess(bool vclmodeout=true) ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;


    struct Change2Insert : public BindedFuncCreator<Change2Insert> {
        explicit Change2Insert() ;
        static void sprocess(bool vclmodeout=true) ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;


    struct Change2Visual : public BindedFuncCreator<Change2Visual> {
        explicit Change2Visual() ;
        static void sprocess(bool vclmodeout=true) ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;

    struct MyConfigWindowNormal : public BindedFuncCreator<MyConfigWindowNormal> {
        explicit MyConfigWindowNormal() ;
        static void sprocess() ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;

    struct MyConfigWindowInsert : public BindedFuncCreator<MyConfigWindowInsert> {
        explicit MyConfigWindowInsert() ;
        static void sprocess() ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;
}

#endif
