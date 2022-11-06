#ifndef _EDI_DELETE_HPP
#define _EDI_DELETE_HPP

#include "bind/bindedfunc.hpp"
#include "changebase.hpp"


namespace vind
{
    namespace bind
    {
        //Delete
        struct DeleteHighlightText : public BindedFuncVoid<DeleteHighlightText> {
            explicit DeleteHighlightText() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;

        struct DeleteLine : public ChangeBaseCreator<DeleteLine> {
            explicit DeleteLine() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;

        struct DeleteLineUntilEOL : public ChangeBaseCreator<DeleteLineUntilEOL> {
            explicit DeleteLineUntilEOL() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;

        struct DeleteAfter : public ChangeBaseCreator<DeleteAfter> {
            explicit DeleteAfter() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;

        struct DeleteBefore : public ChangeBaseCreator<DeleteBefore> {
            explicit DeleteBefore() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;
    }
}

#endif
