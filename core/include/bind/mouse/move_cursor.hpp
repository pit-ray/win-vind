#ifndef _MOVE_CURSOR_HPP
#define _MOVE_CURSOR_HPP

#include "bind/binded_func_creator.hpp"

namespace vind
{
    class MoveCursorLeft : public BindedFuncCreator<MoveCursorLeft> {
    private:
        struct Impl ;
        std::unique_ptr<Impl> pimpl ;

    public:
        void sprocess(int delta=1) const ;
        void sprocess(NTypeLogger& parent_lgr) const ;
        void sprocess(const CharLogger& parent_lgr) const ;

        explicit MoveCursorLeft() ;
        virtual ~MoveCursorLeft() noexcept ;

        MoveCursorLeft(MoveCursorLeft&&) ;
        MoveCursorLeft& operator=(MoveCursorLeft&&) ;
        MoveCursorLeft(const MoveCursorLeft&)            = delete ;
        MoveCursorLeft& operator=(const MoveCursorLeft&) = delete ;

        void reconstruct() override ;
    } ;


    class MoveCursorRight : public BindedFuncCreator<MoveCursorRight> {
    private:
        struct Impl ;
        std::unique_ptr<Impl> pimpl ;

    public:
        void sprocess(int delta=1) const ;
        void sprocess(NTypeLogger& parent_lgr) const ;
        void sprocess(const CharLogger& parent_lgr) const ;

        explicit MoveCursorRight() ;
        virtual ~MoveCursorRight() noexcept ;

        MoveCursorRight(MoveCursorRight&&) ;
        MoveCursorRight& operator=(MoveCursorRight&&) ;
        MoveCursorRight(const MoveCursorRight&)            = delete ;
        MoveCursorRight& operator=(const MoveCursorRight&) = delete ;

        void reconstruct() override ;
    } ;


    class MoveCursorUp : public BindedFuncCreator<MoveCursorUp> {
    private:
        struct Impl ;
        std::unique_ptr<Impl> pimpl ;

    public:
        void sprocess(int delta=1) const ;
        void sprocess(NTypeLogger& parent_lgr) const ;
        void sprocess(const CharLogger& parent_lgr) const ;

        explicit MoveCursorUp() ;
        virtual ~MoveCursorUp() noexcept ;

        MoveCursorUp(MoveCursorUp&&) ;
        MoveCursorUp& operator=(MoveCursorUp&&) ;
        MoveCursorUp(const MoveCursorUp&)            = delete ;
        MoveCursorUp& operator=(const MoveCursorUp&) = delete ;

        void reconstruct() override ;
    } ;


    class MoveCursorDown : public BindedFuncCreator<MoveCursorDown> {
    private:
        struct Impl ;
        std::unique_ptr<Impl> pimpl ;

    public:
        void sprocess(int delta=1) const ;
        void sprocess(NTypeLogger& parent_lgr) const ;
        void sprocess(const CharLogger& parent_lgr) const ;

        explicit MoveCursorDown() ;
        virtual ~MoveCursorDown() noexcept ;

        MoveCursorDown(MoveCursorDown&&) ;
        MoveCursorDown& operator=(MoveCursorDown&&) ;
        MoveCursorDown(const MoveCursorDown&)            = delete ;
        MoveCursorDown& operator=(const MoveCursorDown&) = delete ;

        void reconstruct() override ;
    } ;
}
#endif
