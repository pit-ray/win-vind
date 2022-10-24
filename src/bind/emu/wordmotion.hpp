#ifndef _WORDMOTION_HPP
#define _WORDMOTION_HPP

#include "bind/bindedfunc.hpp"

namespace vind
{
    namespace bind
    {
        struct MoveFwdWord : public BindedFuncVoid<MoveFwdWord> {
            explicit MoveFwdWord() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;

        struct MoveFwdBigWord : public BindedFuncVoid<MoveFwdBigWord> {
            explicit MoveFwdBigWord() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;


        struct MoveBckWord : public BindedFuncVoid<MoveBckWord> {
            explicit MoveBckWord() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;

        struct MoveBckBigWord : public BindedFuncVoid<MoveBckBigWord> {
            explicit MoveBckBigWord() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;


        struct MoveEndWord : public BindedFuncVoid<MoveEndWord> {
            explicit MoveEndWord() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;

        struct MoveEndBigWord : public BindedFuncVoid<MoveEndBigWord> {
            explicit MoveEndBigWord() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;

        struct MoveBckEndWord : public BindedFuncVoid<MoveBckEndWord> {
            explicit MoveBckEndWord() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;

        struct MoveBckEndBigWord : public BindedFuncVoid<MoveBckEndBigWord> {
            explicit MoveBckEndBigWord() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;


        struct MoveFwdWordSimple : public BindedFuncVoid<MoveFwdWordSimple> {
            explicit MoveFwdWordSimple() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;

        struct MoveBckWordSimple : public BindedFuncVoid<MoveBckWordSimple> {
            explicit MoveBckWordSimple() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;
    }
}

#endif
