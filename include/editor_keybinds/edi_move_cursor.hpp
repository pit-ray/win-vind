#ifndef _EDI_MOVE_CURSOR_HPP
#define _EDI_MOVE_CURSOR_HPP

#include "binded_function_with_creator.hpp"


struct EdiMoveLeft : public BindedFunctionWithCreator<EdiMoveLeft>
{
    bool sprocess(const bool first_call) const ;
    static const std::string sname() noexcept ;

    explicit EdiMoveLeft() ;
    ~EdiMoveLeft() ;

    EdiMoveLeft(EdiMoveLeft&&) ;
    EdiMoveLeft& operator=(EdiMoveLeft&&) ;

    EdiMoveLeft(const EdiMoveLeft&) = delete ;
    EdiMoveLeft& operator=(const EdiMoveLeft&) = delete ;

private:
    struct Impl ;
    std::unique_ptr<Impl> pimpl ;
} ;

struct EdiMoveRight : public BindedFunctionWithCreator<EdiMoveRight>
{
    bool sprocess(const bool first_call) const ;
    static const std::string sname() noexcept ;

    explicit EdiMoveRight() ;
    ~EdiMoveRight() ;

    EdiMoveRight(EdiMoveRight&&) ;
    EdiMoveRight& operator=(EdiMoveRight&&) ;

    EdiMoveRight(const EdiMoveRight&) = delete ;
    EdiMoveRight& operator=(const EdiMoveRight&) = delete ;

private:
    struct Impl ;
    std::unique_ptr<Impl> pimpl ;
} ;

struct EdiMoveUp : public BindedFunctionWithCreator<EdiMoveUp>
{
    bool sprocess(const bool first_call) const ;
    static const std::string sname() noexcept ;

    explicit EdiMoveUp() ;
    ~EdiMoveUp() ;

    EdiMoveUp(EdiMoveUp&&) ;
    EdiMoveUp& operator=(EdiMoveUp&&) ;

    EdiMoveUp(const EdiMoveUp&) = delete ;
    EdiMoveUp& operator=(const EdiMoveUp&) = delete ;

private:
    struct Impl ;
    std::unique_ptr<Impl> pimpl ;
} ;

struct EdiMoveDown : public BindedFunctionWithCreator<EdiMoveDown>
{
    bool sprocess(const bool first_call) const ;
    static const std::string sname() noexcept ;

    explicit EdiMoveDown() ;
    ~EdiMoveDown() ;

    EdiMoveDown(EdiMoveDown&&) ;
    EdiMoveDown& operator=(EdiMoveDown&&) ;

    EdiMoveDown(const EdiMoveDown&) = delete ;
    EdiMoveDown& operator=(const EdiMoveDown&) = delete ;

private:
    struct Impl ;
    std::unique_ptr<Impl> pimpl ;
} ;

#endif