#ifndef _EDI_CHANGE_MOVE_HPP
#define _EDI_CHANGE_MOVE_HPP

#include "bind/binded_func_creator.hpp"

namespace vind
{
    struct ToInsertBOL : public BindedFuncCreator<ToInsertBOL> {
        explicit ToInsertBOL() ;
        static void sprocess(bool vclmodeout=true) ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;

    struct Change2EdiBkInsert : public BindedFuncCreator<Change2EdiBkInsert> {
        explicit Change2EdiBkInsert() ;
        static void sprocess(bool vclmodeout=true) ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;

    struct ToInsertEOL : public BindedFuncCreator<ToInsertEOL> {
        explicit ToInsertEOL() ;
        static void sprocess(bool vclmodeout=true) ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;

    struct ToInsertNLBelow : public BindedFuncCreator<ToInsertNLBelow> {
        explicit ToInsertNLBelow() ;
        static void sprocess(bool vclmodeout=true) ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;

    struct ToInsertNLAbove : public BindedFuncCreator<ToInsertNLAbove> {
        explicit ToInsertNLAbove() ;
        static void sprocess(bool vclmodeout=true) ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;
}

#endif
