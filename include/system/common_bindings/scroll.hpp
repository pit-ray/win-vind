#ifndef _SCROLL_HPP
#define _SCROLL_HPP
#include "binded_func_with_creator.hpp"

class ScrollUp : public BindedFuncWithCreator<ScrollUp>
{
private:
    struct Impl ;
    std::unique_ptr<Impl> pimpl ;

public:
    void sprocess(
            const bool first_call,
            const unsigned int repeat_num,
            const KeyLogger* parent_vkclgr,
            const KeyLogger* const parent_charlgr) const ;
    static const std::string sname() noexcept ;

    explicit ScrollUp() ;
    virtual ~ScrollUp() noexcept ;

    ScrollUp(ScrollUp&&) ;
    ScrollUp& operator=(ScrollUp&&) ;
    ScrollUp(const ScrollUp&)               = delete ;
    ScrollUp& operator=(const ScrollUp&)    = delete ;
} ;


class ScrollDown : public BindedFuncWithCreator<ScrollDown>
{
private:
    struct Impl ;
    std::unique_ptr<Impl> pimpl ;

public:
    void sprocess(
            const bool first_call,
            const unsigned int repeat_num,
            const KeyLogger* parent_vkclgr,
            const KeyLogger* const parent_charlgr) const ;
    static const std::string sname() noexcept ;

    explicit ScrollDown() ;
    virtual ~ScrollDown() noexcept ;

    ScrollDown(ScrollDown&&) ;
    ScrollDown& operator=(ScrollDown&&) ;
    ScrollDown(const ScrollDown&)               = delete ;
    ScrollDown& operator=(const ScrollDown&)    = delete ;
} ;


class ScrollMidUp : public BindedFuncWithCreator<ScrollMidUp>
{
private:
    struct Impl ;
    std::unique_ptr<Impl> pimpl ;

public:
    void sprocess(
            const bool first_call,
            const unsigned int repeat_num,
            const KeyLogger* parent_vkclgr,
            const KeyLogger* const parent_charlgr) const ;
    static const std::string sname() noexcept ;

    explicit ScrollMidUp() ;
    virtual ~ScrollMidUp() noexcept ;

    ScrollMidUp(ScrollMidUp&&) ;
    ScrollMidUp& operator=(ScrollMidUp&&) ;
    ScrollMidUp(const ScrollMidUp&)             = delete ;
    ScrollMidUp& operator=(const ScrollMidUp&)  = delete ;
} ;


class ScrollMidDown : public BindedFuncWithCreator<ScrollMidDown>
{
private:
    struct Impl ;
    std::unique_ptr<Impl> pimpl ;

public:
    void sprocess(
            const bool first_call,
            const unsigned int repeat_num,
            const KeyLogger* parent_vkclgr,
            const KeyLogger* const parent_charlgr) const ;
    static const std::string sname() noexcept ;

    explicit ScrollMidDown() ;
    virtual ~ScrollMidDown() noexcept ;

    ScrollMidDown(ScrollMidDown&&) ;
    ScrollMidDown& operator=(ScrollMidDown&&) ;
    ScrollMidDown(const ScrollMidDown&)             = delete ;
    ScrollMidDown& operator=(const ScrollMidDown&)  = delete ;
} ;


class ScrollPageUp : public BindedFuncWithCreator<ScrollPageUp>
{
private:
    struct Impl ;
    std::unique_ptr<Impl> pimpl ;

public:
    void sprocess(
            const bool first_call,
            const unsigned int repeat_num,
            const KeyLogger* parent_vkclgr,
            const KeyLogger* const parent_charlgr) const ;
    static const std::string sname() noexcept ;

    explicit ScrollPageUp() ;
    virtual ~ScrollPageUp() noexcept ;

    ScrollPageUp(ScrollPageUp&&) ;
    ScrollPageUp& operator=(ScrollPageUp&&) ;
    ScrollPageUp(const ScrollPageUp&)               = delete ;
    ScrollPageUp& operator=(const ScrollPageUp&)    = delete ;
} ;


class ScrollPageDown : public BindedFuncWithCreator<ScrollPageDown>
{
private:
    struct Impl ;
    std::unique_ptr<Impl> pimpl ;


public:
    void sprocess(
            const bool first_call,
            const unsigned int repeat_num,
            const KeyLogger* parent_vkclgr,
            const KeyLogger* const parent_charlgr) const ;
    static const std::string sname() noexcept ;

    explicit ScrollPageDown() ;
    virtual ~ScrollPageDown() noexcept ;

    ScrollPageDown(ScrollPageDown&&) ;
    ScrollPageDown& operator=(ScrollPageDown&&) ;
    ScrollPageDown(const ScrollPageDown&)               = delete ;
    ScrollPageDown& operator=(const ScrollPageDown&)    = delete ;
} ;


class ScrollLeft : public BindedFuncWithCreator<ScrollLeft>
{
private:
    struct Impl ;
    std::unique_ptr<Impl> pimpl ;

public:
    void sprocess(
            const bool first_call,
            const unsigned int repeat_num,
            const KeyLogger* parent_vkclgr,
            const KeyLogger* const parent_charlgr) const ;
    static const std::string sname() noexcept ;

    explicit ScrollLeft() ;
    virtual ~ScrollLeft() noexcept ;

    ScrollLeft(ScrollLeft&&) ;
    ScrollLeft& operator=(ScrollLeft&&) ;
    ScrollLeft(const ScrollLeft&)               = delete ;
    ScrollLeft& operator=(const ScrollLeft&)    = delete ;
} ;


class ScrollRight : public BindedFuncWithCreator<ScrollRight>
{
private:
    struct Impl ;
    std::unique_ptr<Impl> pimpl ;

public:
    void sprocess(
            const bool first_call,
            const unsigned int repeat_num,
            const KeyLogger* parent_vkclgr,
            const KeyLogger* const parent_charlgr) const ;
    static const std::string sname() noexcept ;

    explicit ScrollRight() ;
    virtual ~ScrollRight() noexcept ;

    ScrollRight(ScrollRight&&) ;
    ScrollRight& operator=(ScrollRight&&) ;
    ScrollRight(const ScrollRight&)             = delete ;
    ScrollRight& operator=(const ScrollRight&)  = delete ;
} ;


class ScrollMidLeft : public BindedFuncWithCreator<ScrollMidLeft>
{
private:
    struct Impl ;
    std::unique_ptr<Impl> pimpl ;

public:
    void sprocess(
            const bool first_call,
            const unsigned int repeat_num,
            const KeyLogger* parent_vkclgr,
            const KeyLogger* const parent_charlgr) const ;
    static const std::string sname() noexcept ;

    explicit ScrollMidLeft() ;
    virtual ~ScrollMidLeft() noexcept ;

    ScrollMidLeft(ScrollMidLeft&&) ;
    ScrollMidLeft& operator=(ScrollMidLeft&&) ;
    ScrollMidLeft(const ScrollMidLeft&)             = delete ;
    ScrollMidLeft& operator=(const ScrollMidLeft&)  = delete ;
} ;


class ScrollMidRight : public BindedFuncWithCreator<ScrollMidRight>
{
private:
    struct Impl ;
    std::unique_ptr<Impl> pimpl ;

public:
    void sprocess(
            const bool first_call,
            const unsigned int repeat_num,
            const KeyLogger* parent_vkclgr,
            const KeyLogger* const parent_charlgr) const ;
    static const std::string sname() noexcept ;

    explicit ScrollMidRight() ;
    virtual ~ScrollMidRight() noexcept ;

    ScrollMidRight(ScrollMidRight&&) ;
    ScrollMidRight& operator=(ScrollMidRight&&) ;
    ScrollMidRight(const ScrollMidRight&)               = delete ;
    ScrollMidRight& operator=(const ScrollMidRight&)    = delete ;
} ;
#endif
