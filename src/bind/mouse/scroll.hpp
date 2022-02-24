#ifndef _SCROLL_HPP
#define _SCROLL_HPP
#include "bind/bindedfunc.hpp"

namespace vind
{
    namespace bind
    {
        class ScrollUp : public BindedFuncVoid<ScrollUp> {
        private:
            struct Impl ;
            std::unique_ptr<Impl> pimpl ;

        public:
            void sprocess(unsigned int count=1) const ;
            void sprocess(core::NTypeLogger& parent_lgr) const ;
            void sprocess(const core::CharLogger& parent_lgr) const ;

            explicit ScrollUp() ;
            virtual ~ScrollUp() noexcept ;

            ScrollUp(ScrollUp&&) ;
            ScrollUp& operator=(ScrollUp&&) ;
            ScrollUp(const ScrollUp&)               = delete ;
            ScrollUp& operator=(const ScrollUp&)    = delete ;
        } ;


        class ScrollDown : public BindedFuncVoid<ScrollDown> {
        private:
            struct Impl ;
            std::unique_ptr<Impl> pimpl ;

        public:
            void sprocess(unsigned int count=1) const ;
            void sprocess(core::NTypeLogger& parent_lgr) const ;
            void sprocess(const core::CharLogger& parent_lgr) const ;

            explicit ScrollDown() ;
            virtual ~ScrollDown() noexcept ;

            ScrollDown(ScrollDown&&) ;
            ScrollDown& operator=(ScrollDown&&) ;
            ScrollDown(const ScrollDown&)               = delete ;
            ScrollDown& operator=(const ScrollDown&)    = delete ;
        } ;


        class ScrollUpHalfPage : public BindedFuncVoid<ScrollUpHalfPage> {
        private:
            struct Impl ;
            std::unique_ptr<Impl> pimpl ;

        public:
            void sprocess(unsigned int count=1) const ;
            void sprocess(core::NTypeLogger& parent_lgr) const ;
            void sprocess(const core::CharLogger& parent_lgr) const ;

            explicit ScrollUpHalfPage() ;
            virtual ~ScrollUpHalfPage() noexcept ;

            ScrollUpHalfPage(ScrollUpHalfPage&&) ;
            ScrollUpHalfPage& operator=(ScrollUpHalfPage&&) ;
            ScrollUpHalfPage(const ScrollUpHalfPage&)             = delete ;
            ScrollUpHalfPage& operator=(const ScrollUpHalfPage&)  = delete ;
        } ;


        class ScrollDownHalfPage : public BindedFuncVoid<ScrollDownHalfPage> {
        private:
            struct Impl ;
            std::unique_ptr<Impl> pimpl ;

        public:
            void sprocess(unsigned int count=1) const ;
            void sprocess(core::NTypeLogger& parent_lgr) const ;
            void sprocess(const core::CharLogger& parent_lgr) const ;

            explicit ScrollDownHalfPage() ;
            virtual ~ScrollDownHalfPage() noexcept ;

            ScrollDownHalfPage(ScrollDownHalfPage&&) ;
            ScrollDownHalfPage& operator=(ScrollDownHalfPage&&) ;
            ScrollDownHalfPage(const ScrollDownHalfPage&)             = delete ;
            ScrollDownHalfPage& operator=(const ScrollDownHalfPage&)  = delete ;
        } ;


        class ScrollUpOnePage : public BindedFuncVoid<ScrollUpOnePage> {
        private:
            struct Impl ;
            std::unique_ptr<Impl> pimpl ;

        public:
            void sprocess(unsigned int count=1) const ;
            void sprocess(core::NTypeLogger& parent_lgr) const ;
            void sprocess(const core::CharLogger& parent_lgr) const ;

            explicit ScrollUpOnePage() ;
            virtual ~ScrollUpOnePage() noexcept ;

            ScrollUpOnePage(ScrollUpOnePage&&) ;
            ScrollUpOnePage& operator=(ScrollUpOnePage&&) ;
            ScrollUpOnePage(const ScrollUpOnePage&)               = delete ;
            ScrollUpOnePage& operator=(const ScrollUpOnePage&)    = delete ;
        } ;


        class ScrollDownOnePage : public BindedFuncVoid<ScrollDownOnePage> {
        private:
            struct Impl ;
            std::unique_ptr<Impl> pimpl ;


        public:
            void sprocess(unsigned int count=1) const ;
            void sprocess(core::NTypeLogger& parent_lgr) const ;
            void sprocess(const core::CharLogger& parent_lgr) const ;

            explicit ScrollDownOnePage() ;
            virtual ~ScrollDownOnePage() noexcept ;

            ScrollDownOnePage(ScrollDownOnePage&&) ;
            ScrollDownOnePage& operator=(ScrollDownOnePage&&) ;
            ScrollDownOnePage(const ScrollDownOnePage&)               = delete ;
            ScrollDownOnePage& operator=(const ScrollDownOnePage&)    = delete ;
        } ;


        class ScrollLeft : public BindedFuncVoid<ScrollLeft> {
        private:
            struct Impl ;
            std::unique_ptr<Impl> pimpl ;

        public:
            void sprocess(unsigned int count=1) const ;
            void sprocess(core::NTypeLogger& parent_lgr) const ;
            void sprocess(const core::CharLogger& parent_lgr) const ;

            explicit ScrollLeft() ;
            virtual ~ScrollLeft() noexcept ;

            ScrollLeft(ScrollLeft&&) ;
            ScrollLeft& operator=(ScrollLeft&&) ;
            ScrollLeft(const ScrollLeft&)               = delete ;
            ScrollLeft& operator=(const ScrollLeft&)    = delete ;
        } ;


        class ScrollRight : public BindedFuncVoid<ScrollRight> {
        private:
            struct Impl ;
            std::unique_ptr<Impl> pimpl ;

        public:
            void sprocess(unsigned int count=1) const ;
            void sprocess(core::NTypeLogger& parent_lgr) const ;
            void sprocess(const core::CharLogger& parent_lgr) const ;

            explicit ScrollRight() ;
            virtual ~ScrollRight() noexcept ;

            ScrollRight(ScrollRight&&) ;
            ScrollRight& operator=(ScrollRight&&) ;
            ScrollRight(const ScrollRight&)             = delete ;
            ScrollRight& operator=(const ScrollRight&)  = delete ;
        } ;


        class ScrollLeftHalfPage : public BindedFuncVoid<ScrollLeftHalfPage> {
        private:
            struct Impl ;
            std::unique_ptr<Impl> pimpl ;

        public:
            void sprocess(unsigned int count=1) const ;
            void sprocess(core::NTypeLogger& parent_lgr) const ;
            void sprocess(const core::CharLogger& parent_lgr) const ;

            explicit ScrollLeftHalfPage() ;
            virtual ~ScrollLeftHalfPage() noexcept ;

            ScrollLeftHalfPage(ScrollLeftHalfPage&&) ;
            ScrollLeftHalfPage& operator=(ScrollLeftHalfPage&&) ;
            ScrollLeftHalfPage(const ScrollLeftHalfPage&)             = delete ;
            ScrollLeftHalfPage& operator=(const ScrollLeftHalfPage&)  = delete ;
        } ;


        class ScrollRightHalfPage : public BindedFuncVoid<ScrollRightHalfPage> {
        private:
            struct Impl ;
            std::unique_ptr<Impl> pimpl ;

        public:
            void sprocess(unsigned int count=1) const ;
            void sprocess(core::NTypeLogger& parent_lgr) const ;
            void sprocess(const core::CharLogger& parent_lgr) const ;

            explicit ScrollRightHalfPage() ;
            virtual ~ScrollRightHalfPage() noexcept ;

            ScrollRightHalfPage(ScrollRightHalfPage&&) ;
            ScrollRightHalfPage& operator=(ScrollRightHalfPage&&) ;
            ScrollRightHalfPage(const ScrollRightHalfPage&)               = delete ;
            ScrollRightHalfPage& operator=(const ScrollRightHalfPage&)    = delete ;
        } ;
    }
}
#endif
