#ifndef _MOVE_CURSOR_HPP
#define _MOVE_CURSOR_HPP

#include "bind/binded_func.hpp"

namespace vind
{
    namespace bind
    {
        class MoveCursorLeft : public BindedFuncVoid<MoveCursorLeft> {
        private:
            struct Impl ;
            std::unique_ptr<Impl> pimpl ;

        public:
            void sprocess(int delta=1) const ;
            void sprocess(core::NTypeLogger& parent_lgr) const ;
            void sprocess(const core::CharLogger& parent_lgr) const ;

            explicit MoveCursorLeft() ;
            virtual ~MoveCursorLeft() noexcept ;

            MoveCursorLeft(MoveCursorLeft&&) ;
            MoveCursorLeft& operator=(MoveCursorLeft&&) ;
            MoveCursorLeft(const MoveCursorLeft&)            = delete ;
            MoveCursorLeft& operator=(const MoveCursorLeft&) = delete ;

            void reconstruct() override ;
        } ;


        class MoveCursorRight : public BindedFuncVoid<MoveCursorRight> {
        private:
            struct Impl ;
            std::unique_ptr<Impl> pimpl ;

        public:
            void sprocess(int delta=1) const ;
            void sprocess(core::NTypeLogger& parent_lgr) const ;
            void sprocess(const core::CharLogger& parent_lgr) const ;

            explicit MoveCursorRight() ;
            virtual ~MoveCursorRight() noexcept ;

            MoveCursorRight(MoveCursorRight&&) ;
            MoveCursorRight& operator=(MoveCursorRight&&) ;
            MoveCursorRight(const MoveCursorRight&)            = delete ;
            MoveCursorRight& operator=(const MoveCursorRight&) = delete ;

            void reconstruct() override ;
        } ;


        class MoveCursorUp : public BindedFuncVoid<MoveCursorUp> {
        private:
            struct Impl ;
            std::unique_ptr<Impl> pimpl ;

        public:
            void sprocess(int delta=1) const ;
            void sprocess(core::NTypeLogger& parent_lgr) const ;
            void sprocess(const core::CharLogger& parent_lgr) const ;

            explicit MoveCursorUp() ;
            virtual ~MoveCursorUp() noexcept ;

            MoveCursorUp(MoveCursorUp&&) ;
            MoveCursorUp& operator=(MoveCursorUp&&) ;
            MoveCursorUp(const MoveCursorUp&)            = delete ;
            MoveCursorUp& operator=(const MoveCursorUp&) = delete ;

            void reconstruct() override ;
        } ;


        class MoveCursorDown : public BindedFuncVoid<MoveCursorDown> {
        private:
            struct Impl ;
            std::unique_ptr<Impl> pimpl ;

        public:
            void sprocess(int delta=1) const ;
            void sprocess(core::NTypeLogger& parent_lgr) const ;
            void sprocess(const core::CharLogger& parent_lgr) const ;

            explicit MoveCursorDown() ;
            virtual ~MoveCursorDown() noexcept ;

            MoveCursorDown(MoveCursorDown&&) ;
            MoveCursorDown& operator=(MoveCursorDown&&) ;
            MoveCursorDown(const MoveCursorDown&)            = delete ;
            MoveCursorDown& operator=(const MoveCursorDown&) = delete ;

            void reconstruct() override ;
        } ;
    }
}
#endif
