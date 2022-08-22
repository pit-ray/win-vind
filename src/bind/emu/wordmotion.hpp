#ifndef _WORDMOTION_HPP
#define _WORDMOTION_HPP

#include "movebase.hpp"

namespace vind
{
    namespace bind
    {
        struct MoveFwdWord : public MoveBaseCreator<MoveFwdWord> {
            explicit MoveFwdWord() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;

        struct MoveFwdBigWord : public MoveBaseCreator<MoveFwdBigWord> {
            explicit MoveFwdBigWord() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;


        struct MoveBckWord : public MoveBaseCreator<MoveBckWord> {
            explicit MoveBckWord() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;

        struct MoveBckBigWord : public MoveBaseCreator<MoveBckBigWord> {
            explicit MoveBckBigWord() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;


        struct MoveEndWord : public MoveBaseCreator<MoveEndWord> {
            explicit MoveEndWord() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;

        struct MoveEndBigWord : public MoveBaseCreator<MoveEndBigWord> {
            explicit MoveEndBigWord() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;

        struct MoveBckEndWord : public MoveBaseCreator<MoveBckEndWord> {
            explicit MoveBckEndWord() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;

        struct MoveBckEndBigWord : public MoveBaseCreator<MoveBckEndBigWord> {
            explicit MoveBckEndBigWord() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;


        struct MoveFwdWordSimple : public MoveBaseCreator<MoveFwdWordSimple> {
            explicit MoveFwdWordSimple() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;

        struct MoveBckWordSimple : public MoveBaseCreator<MoveBckWordSimple> {
            explicit MoveBckWordSimple() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;
    }
}

#endif
