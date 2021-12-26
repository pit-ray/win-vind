#ifndef _EDI_CHANGE_MOVE_HPP
#define _EDI_CHANGE_MOVE_HPP

#include "changebase.hpp"


namespace vind
{
    namespace bind
    {
        struct ToInsertBOL : public ChangeBaseCreator<ToInsertBOL> {
            explicit ToInsertBOL() ;
            static void sprocess(bool vclmodeout=true) ;
            static void sprocess(core::NTypeLogger& parent_lgr) ;
            static void sprocess(const core::CharLogger& parent_lgr) ;
        } ;

        struct Change2EdiBkInsert : public ChangeBaseCreator<Change2EdiBkInsert> {
            explicit Change2EdiBkInsert() ;
            static void sprocess(bool vclmodeout=true) ;
            static void sprocess(core::NTypeLogger& parent_lgr) ;
            static void sprocess(const core::CharLogger& parent_lgr) ;
        } ;

        struct ToInsertEOL : public ChangeBaseCreator<ToInsertEOL> {
            explicit ToInsertEOL() ;
            static void sprocess(bool vclmodeout=true) ;
            static void sprocess(core::NTypeLogger& parent_lgr) ;
            static void sprocess(const core::CharLogger& parent_lgr) ;
        } ;

        struct ToInsertNLBelow : public ChangeBaseCreator<ToInsertNLBelow> {
            explicit ToInsertNLBelow() ;
            static void sprocess(bool vclmodeout=true) ;
            static void sprocess(core::NTypeLogger& parent_lgr) ;
            static void sprocess(const core::CharLogger& parent_lgr) ;
        } ;

        struct ToInsertNLAbove : public ChangeBaseCreator<ToInsertNLAbove> {
            explicit ToInsertNLAbove() ;
            static void sprocess(bool vclmodeout=true) ;
            static void sprocess(core::NTypeLogger& parent_lgr) ;
            static void sprocess(const core::CharLogger& parent_lgr) ;
        } ;
    }
}

#endif
