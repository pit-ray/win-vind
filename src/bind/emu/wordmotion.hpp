#ifndef _WORDMOTION_HPP
#define _WORDMOTION_HPP

#include "movebase.hpp"

namespace vind
{
    namespace bind
    {
        struct MoveFwdWord : public MoveBaseCreator<MoveFwdWord> {
            explicit MoveFwdWord() ;
            void sprocess(unsigned int repeat_num=1) const ;
            void sprocess(core::NTypeLogger& parent_lgr) const ;
            void sprocess(const core::CharLogger& parent_lgr) const ;
        } ;

        struct MoveFwdBigWord : public MoveBaseCreator<MoveFwdBigWord> {
            explicit MoveFwdBigWord() ;
            void sprocess(unsigned int repeat_num=1) const ;
            void sprocess(core::NTypeLogger& parent_lgr) const ;
            void sprocess(const core::CharLogger& parent_lgr) const ;
        } ;


        struct MoveBckWord : public MoveBaseCreator<MoveBckWord> {
            explicit MoveBckWord() ;
            void sprocess(unsigned int repeat_num=1) const ;
            void sprocess(core::NTypeLogger& parent_lgr) const ;
            void sprocess(const core::CharLogger& parent_lgr) const ;
        } ;

        struct MoveBckBigWord : public MoveBaseCreator<MoveBckBigWord> {
            explicit MoveBckBigWord() ;
            void sprocess(unsigned int repeat_num=1) const ;
            void sprocess(core::NTypeLogger& parent_lgr) const ;
            void sprocess(const core::CharLogger& parent_lgr) const ;
        } ;


        struct MoveEndWord : public MoveBaseCreator<MoveEndWord> {
            explicit MoveEndWord() ;
            void sprocess(unsigned int repeat_num=1) const ;
            void sprocess(core::NTypeLogger& parent_lgr) const ;
            void sprocess(const core::CharLogger& parent_lgr) const ;
        } ;

        struct MoveEndBigWord : public MoveBaseCreator<MoveEndBigWord> {
            explicit MoveEndBigWord() ;
            void sprocess(unsigned int repeat_num=1) const ;
            void sprocess(core::NTypeLogger& parent_lgr) const ;
            void sprocess(const core::CharLogger& parent_lgr) const ;
        } ;


        struct MoveBckEndWord : public MoveBaseCreator<MoveBckEndWord> {
            explicit MoveBckEndWord() ;
            void sprocess(unsigned int repeat_num=1) const ;
            void sprocess(core::NTypeLogger& parent_lgr) const ;
            void sprocess(const core::CharLogger& parent_lgr) const ;
        } ;

        struct MoveBckEndBigWord : public MoveBaseCreator<MoveBckEndBigWord> {
            explicit MoveBckEndBigWord() ;
            void sprocess(unsigned int repeat_num=1) const ;
            void sprocess(core::NTypeLogger& parent_lgr) const ;
            void sprocess(const core::CharLogger& parent_lgr) const ;
        } ;
    }
}

#endif
