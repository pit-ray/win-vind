#ifndef _MOVE_CURSOR_HPP
#define _MOVE_CURSOR_HPP

#include "binded_func_with_creator.hpp"

class MoveLeft : public BindedFuncWithCreator<MoveLeft>
{
private:
    struct Impl ;
    std::unique_ptr<Impl> pimpl ;

public:
    void sprocess(
            const bool first_call,
            const unsigned int repeat_num,
            KeyLogger* parent_vkclgr,
            const KeyLogger* const parent_charlgr) const ;
    static const std::string sname() noexcept ;

    explicit MoveLeft() ;
    virtual ~MoveLeft() noexcept ;

    MoveLeft(MoveLeft&&) ;
    MoveLeft& operator=(MoveLeft&&) ;
    MoveLeft(const MoveLeft&)            = delete ;
    MoveLeft& operator=(const MoveLeft&) = delete ;
} ;


class MoveRight : public BindedFuncWithCreator<MoveRight>
{
private:
    struct Impl ;
    std::unique_ptr<Impl> pimpl ;

public:
    void sprocess(
            const bool first_call,
            const unsigned int repeat_num,
            KeyLogger* parent_vkclgr,
            const KeyLogger* const parent_charlgr) const ;
    static const std::string sname() noexcept ;

    explicit MoveRight() ;
    virtual ~MoveRight() noexcept ;

    MoveRight(MoveRight&&) ;
    MoveRight& operator=(MoveRight&&) ;
    MoveRight(const MoveRight&)            = delete ;
    MoveRight& operator=(const MoveRight&) = delete ;
} ;


class MoveUp : public BindedFuncWithCreator<MoveUp>
{
private:
    struct Impl ;
    std::unique_ptr<Impl> pimpl ;

public:
    void sprocess(
            const bool first_call,
            const unsigned int repeat_num,
            KeyLogger* parent_vkclgr,
            const KeyLogger* const parent_charlgr) const ;
    static const std::string sname() noexcept ;

    explicit MoveUp() ;
    virtual ~MoveUp() noexcept ;

    MoveUp(MoveUp&&) ;
    MoveUp& operator=(MoveUp&&) ;
    MoveUp(const MoveUp&)            = delete ;
    MoveUp& operator=(const MoveUp&) = delete ;
} ;


class MoveDown : public BindedFuncWithCreator<MoveDown>
{
private:
    struct Impl ;
    std::unique_ptr<Impl> pimpl ;

public:
    void sprocess(
            const bool first_call,
            const unsigned int repeat_num,
            KeyLogger* parent_vkclgr,
            const KeyLogger* const parent_charlgr) const ;
    static const std::string sname() noexcept ;

    explicit MoveDown() ;
    virtual ~MoveDown() noexcept ;

    MoveDown(MoveDown&&) ;
    MoveDown& operator=(MoveDown&&) ;
    MoveDown(const MoveDown&)            = delete ;
    MoveDown& operator=(const MoveDown&) = delete ;
} ;
#endif
