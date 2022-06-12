#ifndef _WORDMOTION_HPP
#define _WORDMOTION_HPP

#include "movebase.hpp"

namespace vind
{
    namespace bind
    {
        struct MoveFwdWord : public MoveBaseCreator<MoveFwdWord> {
            explicit MoveFwdWord() ;
            void sprocess(unsigned int count=1) ;
            void sprocess(core::NTypeLogger& parent_lgr) ;
            void sprocess(const core::CharLogger& parent_lgr) ;
        } ;

        struct MoveFwdBigWord : public MoveBaseCreator<MoveFwdBigWord> {
            explicit MoveFwdBigWord() ;
            void sprocess(unsigned int count=1) ;
            void sprocess(core::NTypeLogger& parent_lgr) ;
            void sprocess(const core::CharLogger& parent_lgr) ;
        } ;


        struct MoveBckWord : public MoveBaseCreator<MoveBckWord> {
            explicit MoveBckWord() ;
            void sprocess(unsigned int count=1) ;
            void sprocess(core::NTypeLogger& parent_lgr) ;
            void sprocess(const core::CharLogger& parent_lgr) ;
        } ;

        struct MoveBckBigWord : public MoveBaseCreator<MoveBckBigWord> {
            explicit MoveBckBigWord() ;
            void sprocess(unsigned int count=1) ;
            void sprocess(core::NTypeLogger& parent_lgr) ;
            void sprocess(const core::CharLogger& parent_lgr) ;
        } ;


        struct MoveEndWord : public MoveBaseCreator<MoveEndWord> {
            explicit MoveEndWord() ;
            void sprocess(unsigned int count=1) ;
            void sprocess(core::NTypeLogger& parent_lgr) ;
            void sprocess(const core::CharLogger& parent_lgr) ;
        } ;

        struct MoveEndBigWord : public MoveBaseCreator<MoveEndBigWord> {
            explicit MoveEndBigWord() ;
            void sprocess(unsigned int count=1) ;
            void sprocess(core::NTypeLogger& parent_lgr) ;
            void sprocess(const core::CharLogger& parent_lgr) ;
        } ;


        struct MoveBckEndWord : public MoveBaseCreator<MoveBckEndWord> {
            explicit MoveBckEndWord() ;
            void sprocess(unsigned int count=1) ;
            void sprocess(core::NTypeLogger& parent_lgr) ;
            void sprocess(const core::CharLogger& parent_lgr) ;
        } ;

        struct MoveBckEndBigWord : public MoveBaseCreator<MoveBckEndBigWord> {
            explicit MoveBckEndBigWord() ;
            void sprocess(unsigned int count=1) ;
            void sprocess(core::NTypeLogger& parent_lgr) ;
            void sprocess(const core::CharLogger& parent_lgr) ;
        } ;

        class MoveFwdWordSimple : public MoveBaseCreator<MoveFwdWordSimple> {
        private:
            struct Impl ;
            std::unique_ptr<Impl> pimpl ;

        public:
            void sprocess(unsigned int count=1) ;
            void sprocess(core::NTypeLogger& parent_lgr) ;
            void sprocess(const core::CharLogger& parent_lgr) ;

            explicit MoveFwdWordSimple() ;
            virtual ~MoveFwdWordSimple() noexcept ;

            MoveFwdWordSimple(MoveFwdWordSimple&&) ;
            MoveFwdWordSimple& operator=(MoveFwdWordSimple&&) ;
            MoveFwdWordSimple(const MoveFwdWordSimple&)            = delete ;
            MoveFwdWordSimple& operator=(const MoveFwdWordSimple&) = delete ;
        } ;

        class MoveBckWordSimple : public MoveBaseCreator<MoveBckWordSimple> {
        private:
            struct Impl ;
            std::unique_ptr<Impl> pimpl ;

        public:
            void sprocess(unsigned int count=1) ;
            void sprocess(core::NTypeLogger& parent_lgr) ;
            void sprocess(const core::CharLogger& parent_lgr) ;

            explicit MoveBckWordSimple() ;
            virtual ~MoveBckWordSimple() noexcept ;

            MoveBckWordSimple(MoveBckWordSimple&&) ;
            MoveBckWordSimple& operator=(MoveBckWordSimple&&) ;
            MoveBckWordSimple(const MoveBckWordSimple&)            = delete ;
            MoveBckWordSimple& operator=(const MoveBckWordSimple&) = delete ;
        } ;
    }
}

#endif
