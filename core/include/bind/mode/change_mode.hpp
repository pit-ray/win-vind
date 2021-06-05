#ifndef _CHANGE_MODE_HPP
#define _CHANGE_MODE_HPP

#include "bind/binded_func_creator.hpp"

namespace vind
{
    struct Change2Normal : public BindedFuncCreator<Change2Normal> {
        explicit Change2Normal() ;
        static void sprocess(bool vclmodeout=true) ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;

    struct Change2Resident : public BindedFuncCreator<Change2Resident> {
        explicit Change2Resident() ;
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

    struct Change2EdiNormal : public BindedFuncCreator<Change2EdiNormal> {
        explicit Change2EdiNormal() ;
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

    struct Change2EdiVisual : public BindedFuncCreator<Change2EdiVisual> {
        explicit Change2EdiVisual() ;
        static void sprocess(bool vclmodeout=true) ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;

    struct Change2EdiLineVisual : public BindedFuncCreator<Change2EdiLineVisual> {
        explicit Change2EdiLineVisual() ;
        static void sprocess(bool vclmodeout=true) ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;
}

#endif
