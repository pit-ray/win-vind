#ifndef _MOVE_CURSOR_HPP
#define _MOVE_CURSOR_HPP

#include "bind/bindedfunc.hpp"

namespace vind
{
    namespace bind
    {
        class MoveCursorLeft : public BindedFuncVoid<MoveCursorLeft> {
        private:
            struct Impl ;
            std::unique_ptr<Impl> pimpl ;

        public:
            explicit MoveCursorLeft() ;
            virtual ~MoveCursorLeft() noexcept ;

            void sprocess(
                std::uint16_t count, const std::string& args) ;

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
            explicit MoveCursorRight() ;
            virtual ~MoveCursorRight() noexcept ;

            void sprocess(
                std::uint16_t count, const std::string& args) ;

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
            explicit MoveCursorUp() ;
            virtual ~MoveCursorUp() noexcept ;

            void sprocess(
                std::uint16_t count, const std::string& args) ;

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
            explicit MoveCursorDown() ;
            virtual ~MoveCursorDown() noexcept ;

            void sprocess(
                std::uint16_t count, const std::string& args) ;

            MoveCursorDown(MoveCursorDown&&) ;
            MoveCursorDown& operator=(MoveCursorDown&&) ;
            MoveCursorDown(const MoveCursorDown&)            = delete ;
            MoveCursorDown& operator=(const MoveCursorDown&) = delete ;

            void reconstruct() override ;
        } ;
    }
}
#endif
