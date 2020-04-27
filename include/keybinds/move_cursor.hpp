#ifndef _MOVE_CURSOR_HPP
#define _MOVE_CURSOR_HPP

#include "binded_function_with_creator.hpp"

struct MoveLeft : public BindedFunctionWithCreator<MoveLeft>
{
    bool sprocess(const bool first_call) const ;
    static const std::string sname() noexcept ;

    explicit MoveLeft() ;
    ~MoveLeft() ;

    MoveLeft(MoveLeft&&) ;
    MoveLeft& operator=(MoveLeft&&) ;

    MoveLeft(const MoveLeft&) = delete ;
    MoveLeft& operator=(const MoveLeft&) = delete ;

private:
    struct Impl ;
    std::unique_ptr<Impl> pimpl ;
} ;


struct MoveRight : public BindedFunctionWithCreator<MoveRight>
{
    bool sprocess(const bool first_call) const ;
    static const std::string sname() noexcept ;

    explicit MoveRight() ;
    ~MoveRight() ;

    MoveRight(MoveRight&&) ;
    MoveRight& operator=(MoveRight&&) ;

    MoveRight(const MoveRight&) = delete ;
    MoveRight& operator=(const MoveRight&) = delete ;

private:
    struct Impl ;
    std::unique_ptr<Impl> pimpl ;
} ;


struct MoveUp : public BindedFunctionWithCreator<MoveUp>
{
    bool sprocess(const bool first_call) const ;
    static const std::string sname() noexcept ;

    explicit MoveUp() ;
    ~MoveUp() ;

    MoveUp(MoveUp&&) ;
    MoveUp& operator=(MoveUp&&) ;

    MoveUp(const MoveUp&) = delete ;
    MoveUp& operator=(const MoveUp&) = delete ;

private:
    struct Impl ;
    std::unique_ptr<Impl> pimpl ;
} ;


struct MoveDown : public BindedFunctionWithCreator<MoveDown>
{
    bool sprocess(const bool first_call) const ;
    static const std::string sname() noexcept ;

    explicit MoveDown() ;
    ~MoveDown() ;

    MoveDown(MoveDown&&) ;
    MoveDown& operator=(MoveDown&&) ;

    MoveDown(const MoveDown&) = delete ;
    MoveDown& operator=(const MoveDown&) = delete ;

private:
    struct Impl ;
    std::unique_ptr<Impl> pimpl ;
} ;
#endif