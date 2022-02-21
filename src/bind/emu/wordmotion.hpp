#ifndef _WORDMOTION_HPP
#define _WORDMOTION_HPP

#include "movebase.hpp"

namespace vind
{
    namespace bind
    {
        class MoveFwdWord : MoveBaseCreator<MoveFwdWord> {
        private:
            struct Impl ;
            std::unique_ptr<Impl> pimpl ;

        public:
            void sprocess(unsigned int repeat_num=1) const ;
            void sprocess(core::NTypeLogger& parent_lgr) const ;
            void sprocess(const core::CharLogger& parent_lgr) const ;

            explicit MoveFwdWord() ;
            virtual ~MoveFwdWord() noexcept ;

            MoveFwdWord(MoveFwdWord&&) ;
            MoveFwdWord& operator=(MoveFwdWord&&) ;

            MoveFwdWord(const MoveFwdWord&) = delete ;
            MoveFwdWord& operator=(const MoveFwdWord&) = delete ;
        } ;

        class MoveFwdBigWord : MoveBaseCreator<MoveFwdBigWord> {
        private:
            struct Impl ;
            std::unique_ptr<Impl> pimpl ;

        public:
            void sprocess(unsigned int repeat_num=1) const ;
            void sprocess(core::NTypeLogger& parent_lgr) const ;
            void sprocess(const core::CharLogger& parent_lgr) const ;

            explicit MoveFwdBigWord() ;
            virtual ~MoveFwdBigWord() noexcept ;

            MoveFwdBigWord(MoveFwdBigWord&&) ;
            MoveFwdBigWord& operator=(MoveFwdBigWord&&) ;

            MoveFwdBigWord(const MoveFwdBigWord&) = delete ;
            MoveFwdBigWord& operator=(const MoveFwdBigWord&) = delete ;
        } ;


        class MoveBckWord : MoveBaseCreator<MoveBckWord> {
        private:
            struct Impl ;
            std::unique_ptr<Impl> pimpl ;

        public:
            void sprocess(unsigned int repeat_num=1) const ;
            void sprocess(core::NTypeLogger& parent_lgr) const ;
            void sprocess(const core::CharLogger& parent_lgr) const ;

            explicit MoveBckWord() ;
            virtual ~MoveBckWord() noexcept ;

            MoveBckWord(MoveBckWord&&) ;
            MoveBckWord& operator=(MoveBckWord&&) ;

            MoveBckWord(const MoveBckWord&) = delete ;
            MoveBckWord& operator=(const MoveBckWord&) = delete ;
        } ;

        class MoveBckBigWord : MoveBaseCreator<MoveBckBigWord> {
        private:
            struct Impl ;
            std::unique_ptr<Impl> pimpl ;

        public:
            void sprocess(unsigned int repeat_num=1) const ;
            void sprocess(core::NTypeLogger& parent_lgr) const ;
            void sprocess(const core::CharLogger& parent_lgr) const ;

            explicit MoveBckBigWord() ;
            virtual ~MoveBckBigWord() noexcept ;

            MoveBckBigWord(MoveBckBigWord&&) ;
            MoveBckBigWord& operator=(MoveBckBigWord&&) ;

            MoveBckBigWord(const MoveBckBigWord&) = delete ;
            MoveBckBigWord& operator=(const MoveBckBigWord&) = delete ;
        } ;


        class MoveEndWord : MoveBaseCreator<MoveEndWord> {
        private:
            struct Impl ;
            std::unique_ptr<Impl> pimpl ;

        public:
            void sprocess(unsigned int repeat_num=1) const ;
            void sprocess(core::NTypeLogger& parent_lgr) const ;
            void sprocess(const core::CharLogger& parent_lgr) const ;

            explicit MoveEndWord() ;
            virtual ~MoveEndWord() noexcept ;

            MoveEndWord(MoveEndWord&&) ;
            MoveEndWord& operator=(MoveEndWord&&) ;

            MoveEndWord(const MoveEndWord&) = delete ;
            MoveEndWord& operator=(const MoveEndWord&) = delete ;
        } ;

        class MoveEndBigWord : MoveBaseCreator<MoveEndBigWord> {
        private:
            struct Impl ;
            std::unique_ptr<Impl> pimpl ;

        public:
            void sprocess(unsigned int repeat_num=1) const ;
            void sprocess(core::NTypeLogger& parent_lgr) const ;
            void sprocess(const core::CharLogger& parent_lgr) const ;

            explicit MoveEndBigWord() ;
            virtual ~MoveEndBigWord() noexcept ;

            MoveEndBigWord(MoveEndBigWord&&) ;
            MoveEndBigWord& operator=(MoveEndBigWord&&) ;

            MoveEndBigWord(const MoveEndBigWord&) = delete ;
            MoveEndBigWord& operator=(const MoveEndBigWord&) = delete ;
        } ;


        class MoveBckEndWord : MoveBaseCreator<MoveBckEndWord> {
        private:
            struct Impl ;
            std::unique_ptr<Impl> pimpl ;

        public:
            void sprocess(unsigned int repeat_num=1) const ;
            void sprocess(core::NTypeLogger& parent_lgr) const ;
            void sprocess(const core::CharLogger& parent_lgr) const ;

            explicit MoveBckEndWord() ;
            virtual ~MoveBckEndWord() noexcept ;

            MoveBckEndWord(MoveBckEndWord&&) ;
            MoveBckEndWord& operator=(MoveBckEndWord&&) ;

            MoveBckEndWord(const MoveBckEndWord&) = delete ;
            MoveBckEndWord& operator=(const MoveBckEndWord&) = delete ;
        } ;

        class MoveBckEndBigWord : MoveBaseCreator<MoveBckEndBigWord> {
        private:
            struct Impl ;
            std::unique_ptr<Impl> pimpl ;

        public:
            void sprocess(unsigned int repeat_num=1) const ;
            void sprocess(core::NTypeLogger& parent_lgr) const ;
            void sprocess(const core::CharLogger& parent_lgr) const ;

            explicit MoveBckEndBigWord() ;
            virtual ~MoveBckEndBigWord() noexcept ;

            MoveBckEndBigWord(MoveBckEndBigWord&&) ;
            MoveBckEndBigWord& operator=(MoveBckEndBigWord&&) ;

            MoveBckEndBigWord(const MoveBckEndBigWord&) = delete ;
            MoveBckEndBigWord& operator=(const MoveBckEndBigWord&) = delete ;
        } ;
    }
}

#endif
