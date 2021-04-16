#ifndef _MOVE_CURSOR_HPP
#define _MOVE_CURSOR_HPP

#include "bind/base/binded_func_with_creator.hpp"

namespace vind
{
    class MoveLeft : public BindedFuncCreator<MoveLeft> {
    private:
        struct Impl ;
        std::unique_ptr<Impl> pimpl ;

    public:
        void sprocess(int delta=1) const ;
        void sprocess(NTypeLogger& parent_lgr) const ;
        void sprocess(const CharLogger& parent_lgr) const ;

        explicit MoveLeft() ;
        virtual ~MoveLeft() noexcept ;

        MoveLeft(MoveLeft&&) ;
        MoveLeft& operator=(MoveLeft&&) ;
        MoveLeft(const MoveLeft&)            = delete ;
        MoveLeft& operator=(const MoveLeft&) = delete ;
    } ;


    class MoveRight : public BindedFuncCreator<MoveRight> {
    private:
        struct Impl ;
        std::unique_ptr<Impl> pimpl ;

    public:
        void sprocess(int delta=1) const ;
        void sprocess(NTypeLogger& parent_lgr) const ;
        void sprocess(const CharLogger& parent_lgr) const ;

        explicit MoveRight() ;
        virtual ~MoveRight() noexcept ;

        MoveRight(MoveRight&&) ;
        MoveRight& operator=(MoveRight&&) ;
        MoveRight(const MoveRight&)            = delete ;
        MoveRight& operator=(const MoveRight&) = delete ;
    } ;


    class MoveUp : public BindedFuncCreator<MoveUp> {
    private:
        struct Impl ;
        std::unique_ptr<Impl> pimpl ;

    public:
        void sprocess(int delta=1) const ;
        void sprocess(NTypeLogger& parent_lgr) const ;
        void sprocess(const CharLogger& parent_lgr) const ;

        explicit MoveUp() ;
        virtual ~MoveUp() noexcept ;

        MoveUp(MoveUp&&) ;
        MoveUp& operator=(MoveUp&&) ;
        MoveUp(const MoveUp&)            = delete ;
        MoveUp& operator=(const MoveUp&) = delete ;
    } ;


    class MoveDown : public BindedFuncCreator<MoveDown> {
    private:
        struct Impl ;
        std::unique_ptr<Impl> pimpl ;

    public:
        void sprocess(int delta=1) const ;
        void sprocess(NTypeLogger& parent_lgr) const ;
        void sprocess(const CharLogger& parent_lgr) const ;

        explicit MoveDown() ;
        virtual ~MoveDown() noexcept ;

        MoveDown(MoveDown&&) ;
        MoveDown& operator=(MoveDown&&) ;
        MoveDown(const MoveDown&)            = delete ;
        MoveDown& operator=(const MoveDown&) = delete ;
    } ;
}
#endif
