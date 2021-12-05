#ifndef _EDI_MOVE_CARET_HPP
#define _EDI_MOVE_CARET_HPP

#include "movebase.hpp"


namespace vind
{
    namespace bind
    {
        class MoveCaretLeft : public MoveBaseCreator<MoveCaretLeft> {
        private:
            struct Impl ;
            std::unique_ptr<Impl> pimpl ;

        public:
            void sprocess(unsigned int repeat_num=1) const ;
            void sprocess(core::NTypeLogger& parent_lgr) const ;
            void sprocess(const core::CharLogger& parent_lgr) const ;

            explicit MoveCaretLeft() ;
            virtual ~MoveCaretLeft() noexcept ;

            MoveCaretLeft(MoveCaretLeft&&) ;
            MoveCaretLeft& operator=(MoveCaretLeft&&) ;
            MoveCaretLeft(const MoveCaretLeft&)            = delete ;
            MoveCaretLeft& operator=(const MoveCaretLeft&) = delete ;
        } ;

        class MoveCaretRight : public MoveBaseCreator<MoveCaretRight> {
        private:
            struct Impl ;
            std::unique_ptr<Impl> pimpl ;

        public:
            void sprocess(unsigned int repeat_num=1) const ;
            void sprocess(core::NTypeLogger& parent_lgr) const ;
            void sprocess(const core::CharLogger& parent_lgr) const ;

            explicit MoveCaretRight() ;
            virtual ~MoveCaretRight() noexcept ;

            MoveCaretRight(MoveCaretRight&&) ;
            MoveCaretRight& operator=(MoveCaretRight&&) ;
            MoveCaretRight(const MoveCaretRight&)            = delete ;
            MoveCaretRight& operator=(const MoveCaretRight&) = delete ;
        } ;

        class MoveCaretUp : public MoveBaseCreator<MoveCaretUp> {
        private:
            struct Impl ;
            std::unique_ptr<Impl> pimpl ;

        public:
            void sprocess(unsigned int repeat_num=1) const ;
            void sprocess(core::NTypeLogger& parent_lgr) const ;
            void sprocess(const core::CharLogger& parent_lgr) const ;

            explicit MoveCaretUp() ;
            virtual ~MoveCaretUp() noexcept ;

            MoveCaretUp(MoveCaretUp&&) ;
            MoveCaretUp& operator=(MoveCaretUp&&) ;
            MoveCaretUp(const MoveCaretUp&)            = delete ;
            MoveCaretUp& operator=(const MoveCaretUp&) = delete ;
        } ;

        class MoveCaretDown : public MoveBaseCreator<MoveCaretDown> {
        private:
            struct Impl ;
            std::unique_ptr<Impl> pimpl ;

        public:
            void sprocess(unsigned int repeat_num=1) const ;
            void sprocess(core::NTypeLogger& parent_lgr) const ;
            void sprocess(const core::CharLogger& parent_lgr) const ;

            explicit MoveCaretDown() ;
            virtual ~MoveCaretDown() noexcept ;

            MoveCaretDown(MoveCaretDown&&) ;
            MoveCaretDown& operator=(MoveCaretDown&&) ;
            MoveCaretDown(const MoveCaretDown&)            = delete ;
            MoveCaretDown& operator=(const MoveCaretDown&) = delete ;
        } ;


        class MoveCaretWordForward : public MoveBaseCreator<MoveCaretWordForward> {
        private:
            struct Impl ;
            std::unique_ptr<Impl> pimpl ;

        public:
            void sprocess(unsigned int repeat_num=1) const ;
            void sprocess(core::NTypeLogger& parent_lgr) const ;
            void sprocess(const core::CharLogger& parent_lgr) const ;

            explicit MoveCaretWordForward() ;
            virtual ~MoveCaretWordForward() noexcept ;

            MoveCaretWordForward(MoveCaretWordForward&&) ;
            MoveCaretWordForward& operator=(MoveCaretWordForward&&) ;
            MoveCaretWordForward(const MoveCaretWordForward&)            = delete ;
            MoveCaretWordForward& operator=(const MoveCaretWordForward&) = delete ;
        } ;

        class MoveCaretWordBackward : public MoveBaseCreator<MoveCaretWordBackward> {
        private:
            struct Impl ;
            std::unique_ptr<Impl> pimpl ;

        public:
            void sprocess(unsigned int repeat_num=1) const ;
            void sprocess(core::NTypeLogger& parent_lgr) const ;
            void sprocess(const core::CharLogger& parent_lgr) const ;

            explicit MoveCaretWordBackward() ;
            virtual ~MoveCaretWordBackward() noexcept ;

            MoveCaretWordBackward(MoveCaretWordBackward&&) ;
            MoveCaretWordBackward& operator=(MoveCaretWordBackward&&) ;
            MoveCaretWordBackward(const MoveCaretWordBackward&)            = delete ;
            MoveCaretWordBackward& operator=(const MoveCaretWordBackward&) = delete ;
        } ;


        class MoveCaretNonBlankWordForward : public MoveBaseCreator<MoveCaretNonBlankWordForward> {
        private:
            struct Impl ;
            std::unique_ptr<Impl> pimpl ;

        public:
            void sprocess(unsigned int repeat_num=1) const ;
            void sprocess(core::NTypeLogger& parent_lgr) const ;
            void sprocess(const core::CharLogger& parent_lgr) const ;

            explicit MoveCaretNonBlankWordForward() ;
            virtual ~MoveCaretNonBlankWordForward() noexcept ;

            MoveCaretNonBlankWordForward(MoveCaretNonBlankWordForward&&) ;
            MoveCaretNonBlankWordForward& operator=(MoveCaretNonBlankWordForward&&) ;
            MoveCaretNonBlankWordForward(const MoveCaretNonBlankWordForward&)            = delete ;
            MoveCaretDown& operator=(const MoveCaretDown&) = delete ;
        } ;

        class MoveCaretNonBlankWordBackward : public MoveBaseCreator<MoveCaretNonBlankWordBackward> {
        private:
            struct Impl ;
            std::unique_ptr<Impl> pimpl ;

        public:
            void sprocess(unsigned int repeat_num=1) const ;
            void sprocess(core::NTypeLogger& parent_lgr) const ;
            void sprocess(const core::CharLogger& parent_lgr) const ;

            explicit MoveCaretNonBlankWordBackward() ;
            virtual ~MoveCaretNonBlankWordBackward() noexcept ;

            MoveCaretNonBlankWordBackward(MoveCaretNonBlankWordBackward&&) ;
            MoveCaretNonBlankWordBackward& operator=(MoveCaretNonBlankWordBackward&&) ;
            MoveCaretNonBlankWordBackward(const MoveCaretNonBlankWordBackward&)            = delete ;
            MoveCaretNonBlankWordBackward& operator=(const MoveCaretNonBlankWordBackward&) = delete ;
        } ;
    }
}

#endif
