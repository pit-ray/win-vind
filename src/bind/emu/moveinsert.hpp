#ifndef _EDI_CHANGE_MOVE_HPP
#define _EDI_CHANGE_MOVE_HPP

#include "changebase.hpp"


namespace vind
{
    namespace bind
    {
        struct ToInsertBOL : public ChangeBaseCreator<ToInsertBOL> {
            explicit ToInsertBOL() ;
            static void sprocess(
                std::uint16_t count,
                const std::string& args,
                bool vclmodeout=true) ;
        } ;

        struct Change2EdiBkInsert : public ChangeBaseCreator<Change2EdiBkInsert> {
            explicit Change2EdiBkInsert() ;
            static void sprocess(
                std::uint16_t count,
                const std::string& args,
                bool vclmodeout=true) ;
        } ;

        struct ToInsertEOL : public ChangeBaseCreator<ToInsertEOL> {
            explicit ToInsertEOL() ;
            static void sprocess(
                std::uint16_t count,
                const std::string& args,
                bool vclmodeout=true) ;
        } ;

        struct ToInsertNLBelow : public ChangeBaseCreator<ToInsertNLBelow> {
            explicit ToInsertNLBelow() ;
            static void sprocess(
                std::uint16_t count,
                const std::string& args,
                bool vclmodeout=true) ;
        } ;

        struct ToInsertNLAbove : public ChangeBaseCreator<ToInsertNLAbove> {
            explicit ToInsertNLAbove() ;
            static void sprocess(
                std::uint16_t count,
                const std::string& args,
                bool vclmodeout=true) ;
        } ;
    }
}

#endif
