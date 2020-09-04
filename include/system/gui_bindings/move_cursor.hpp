#ifndef _MOVE_CURSOR_HPP
#define _MOVE_CURSOR_HPP

#include "key_binding_with_creator.hpp"

struct MoveLeft : public KeyBindingWithCreator<MoveLeft>
{
    bool sprocess(const bool first_call) const ;
    static const std::string sname() noexcept ;

    explicit MoveLeft() ;
    virtual ~MoveLeft() ;

    MoveLeft(MoveLeft&&) ;
    MoveLeft& operator=(MoveLeft&&) ;

    MoveLeft(const MoveLeft&) = delete ;
    MoveLeft& operator=(const MoveLeft&) = delete ;

private:
    struct Impl ;
    std::unique_ptr<Impl> pimpl ;
} ;


struct MoveRight : public KeyBindingWithCreator<MoveRight>
{
    bool sprocess(const bool first_call) const ;
    static const std::string sname() noexcept ;

    explicit MoveRight() ;
    virtual ~MoveRight() ;

    MoveRight(MoveRight&&) ;
    MoveRight& operator=(MoveRight&&) ;

    MoveRight(const MoveRight&) = delete ;
    MoveRight& operator=(const MoveRight&) = delete ;

private:
    struct Impl ;
    std::unique_ptr<Impl> pimpl ;
} ;


struct MoveUp : public KeyBindingWithCreator<MoveUp>
{
    bool sprocess(const bool first_call) const ;
    static const std::string sname() noexcept ;

    explicit MoveUp() ;
    virtual ~MoveUp() ;

    MoveUp(MoveUp&&) ;
    MoveUp& operator=(MoveUp&&) ;

    MoveUp(const MoveUp&) = delete ;
    MoveUp& operator=(const MoveUp&) = delete ;

private:
    struct Impl ;
    std::unique_ptr<Impl> pimpl ;
} ;


struct MoveDown : public KeyBindingWithCreator<MoveDown>
{
    bool sprocess(const bool first_call) const ;
    static const std::string sname() noexcept ;

    explicit MoveDown() ;
    virtual ~MoveDown() ;

    MoveDown(MoveDown&&) ;
    MoveDown& operator=(MoveDown&&) ;

    MoveDown(const MoveDown&) = delete ;
    MoveDown& operator=(const MoveDown&) = delete ;

private:
    struct Impl ;
    std::unique_ptr<Impl> pimpl ;
} ;
#endif