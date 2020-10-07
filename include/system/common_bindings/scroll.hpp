#ifndef _SCROLL_HPP
#define _SCROLL_HPP
#include "binded_func_with_creator.hpp"

struct ScrollUp : public BindedFuncWithCreator<ScrollUp>
{
    bool sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger) const ;
    static const std::string sname() noexcept ;

    explicit ScrollUp() ;
    virtual ~ScrollUp() noexcept ;

    ScrollUp(ScrollUp&&) ;
    ScrollUp& operator=(ScrollUp&&) ;

    ScrollUp(const ScrollUp&) = delete ;
    ScrollUp& operator=(const ScrollUp&) = delete ;

private:
    struct Impl ;
    std::unique_ptr<Impl> pimpl ;
} ;


struct ScrollDown : public BindedFuncWithCreator<ScrollDown>
{
    bool sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger) const ;
    static const std::string sname() noexcept ;

    explicit ScrollDown() ;
    virtual ~ScrollDown() noexcept ;

    ScrollDown(ScrollDown&&) ;
    ScrollDown& operator=(ScrollDown&&) ;

    ScrollDown(const ScrollDown&) = delete ;
    ScrollDown& operator=(const ScrollDown&) = delete ;

private:
    struct Impl ;
    std::unique_ptr<Impl> pimpl ;
} ;


struct ScrollMidUp : public BindedFuncWithCreator<ScrollMidUp>
{
    bool sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger) const ;
    static const std::string sname() noexcept ;

    explicit ScrollMidUp() ;
    virtual ~ScrollMidUp() noexcept ;

    ScrollMidUp(ScrollMidUp&&) ;
    ScrollMidUp& operator=(ScrollMidUp&&) ;

    ScrollMidUp(const ScrollMidUp&) = delete ;
    ScrollMidUp& operator=(const ScrollMidUp&) = delete ;

private:
    struct Impl ;
    std::unique_ptr<Impl> pimpl ;
} ;


struct ScrollMidDown : public BindedFuncWithCreator<ScrollMidDown>
{
    bool sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger) const ;
    static const std::string sname() noexcept ;

    explicit ScrollMidDown() ;
    virtual ~ScrollMidDown() noexcept ;

    ScrollMidDown(ScrollMidDown&&) ;
    ScrollMidDown& operator=(ScrollMidDown&&) ;

    ScrollMidDown(const ScrollMidDown&) = delete ;
    ScrollMidDown& operator=(const ScrollMidDown&) = delete ;

private:
    struct Impl ;
    std::unique_ptr<Impl> pimpl ;
} ;


struct ScrollPageUp : public BindedFuncWithCreator<ScrollPageUp>
{
    bool sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger) const ;
    static const std::string sname() noexcept ;

    explicit ScrollPageUp() ;
    virtual ~ScrollPageUp() noexcept ;

    ScrollPageUp(ScrollPageUp&&) ;
    ScrollPageUp& operator=(ScrollPageUp&&) ;

    ScrollPageUp(const ScrollPageUp&) = delete ;
    ScrollPageUp& operator=(const ScrollPageUp&) = delete ;

private:
    struct Impl ;
    std::unique_ptr<Impl> pimpl ;
} ;


struct ScrollPageDown : public BindedFuncWithCreator<ScrollPageDown>
{
    bool sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger) const ;
    static const std::string sname() noexcept ;

    explicit ScrollPageDown() ;
    virtual ~ScrollPageDown() noexcept ;

    ScrollPageDown(ScrollPageDown&&) ;
    ScrollPageDown& operator=(ScrollPageDown&&) ;

    ScrollPageDown(const ScrollPageDown&) = delete ;
    ScrollPageDown& operator=(const ScrollPageDown&) = delete ;

private:
    struct Impl ;
    std::unique_ptr<Impl> pimpl ;
} ;


struct ScrollLeft : public BindedFuncWithCreator<ScrollLeft>
{
    bool sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger) const ;
    static const std::string sname() noexcept ;

    explicit ScrollLeft() ;
    virtual ~ScrollLeft() noexcept ;

    ScrollLeft(ScrollLeft&&) ;
    ScrollLeft& operator=(ScrollLeft&&) ;

    ScrollLeft(const ScrollLeft&) = delete ;
    ScrollLeft& operator=(const ScrollLeft&) = delete ;

private:
    struct Impl ;
    std::unique_ptr<Impl> pimpl ;
} ;


struct ScrollRight : public BindedFuncWithCreator<ScrollRight>
{
    bool sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger) const ;
    static const std::string sname() noexcept ;

    explicit ScrollRight() ;
    virtual ~ScrollRight() noexcept ;

    ScrollRight(ScrollRight&&) ;
    ScrollRight& operator=(ScrollRight&&) ;

    ScrollRight(const ScrollRight&) = delete ;
    ScrollRight& operator=(const ScrollRight&) = delete ;

private:
    struct Impl ;
    std::unique_ptr<Impl> pimpl ;
} ;


struct ScrollMidLeft : public BindedFuncWithCreator<ScrollMidLeft>
{
    bool sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger) const ;
    static const std::string sname() noexcept ;

    explicit ScrollMidLeft() ;
    virtual ~ScrollMidLeft() noexcept ;

    ScrollMidLeft(ScrollMidLeft&&) ;
    ScrollMidLeft& operator=(ScrollMidLeft&&) ;

    ScrollMidLeft(const ScrollMidLeft&) = delete ;
    ScrollMidLeft& operator=(const ScrollMidLeft&) = delete ;

private:
    struct Impl ;
    std::unique_ptr<Impl> pimpl ;
} ;


struct ScrollMidRight : public BindedFuncWithCreator<ScrollMidRight>
{
    bool sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger) const ;
    static const std::string sname() noexcept ;

    explicit ScrollMidRight() ;
    virtual ~ScrollMidRight() noexcept ;

    ScrollMidRight(ScrollMidRight&&) ;
    ScrollMidRight& operator=(ScrollMidRight&&) ;

    ScrollMidRight(const ScrollMidRight&) = delete ;
    ScrollMidRight& operator=(const ScrollMidRight&) = delete ;

private:
    struct Impl ;
    std::unique_ptr<Impl> pimpl ;
} ;
#endif
