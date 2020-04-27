#ifndef _SCROLL_HPP
#define _SCROLL_HPP
#include "binded_function_with_creator.hpp"

struct ScrollUp : public BindedFunctionWithCreator<ScrollUp>
{
    bool sprocess(const bool first_call) const ;
    static const std::string sname() noexcept ;

    explicit ScrollUp() ;
    ~ScrollUp() ;

    ScrollUp(ScrollUp&&) ;
    ScrollUp& operator=(ScrollUp&&) ;

    ScrollUp(const ScrollUp&) = delete ;
    ScrollUp& operator=(const ScrollUp&) = delete ;

private:
    struct Impl ;
    std::unique_ptr<Impl> pimpl ;
} ;


struct ScrollDown : public BindedFunctionWithCreator<ScrollDown>
{
    bool sprocess(const bool first_call) const ;
    static const std::string sname() noexcept ;

    explicit ScrollDown() ;
    ~ScrollDown() ;

    ScrollDown(ScrollDown&&) ;
    ScrollDown& operator=(ScrollDown&&) ;

    ScrollDown(const ScrollDown&) = delete ;
    ScrollDown& operator=(const ScrollDown&) = delete ;

private:
    struct Impl ;
    std::unique_ptr<Impl> pimpl ;
} ;


struct ScrollMidUp : public BindedFunctionWithCreator<ScrollMidUp>
{
    bool sprocess(const bool first_call) const ;
    static const std::string sname() noexcept ;

    explicit ScrollMidUp() ;
    ~ScrollMidUp() ;

    ScrollMidUp(ScrollMidUp&&) ;
    ScrollMidUp& operator=(ScrollMidUp&&) ;

    ScrollMidUp(const ScrollMidUp&) = delete ;
    ScrollMidUp& operator=(const ScrollMidUp&) = delete ;

private:
    struct Impl ;
    std::unique_ptr<Impl> pimpl ;
} ;


struct ScrollMidDown : public BindedFunctionWithCreator<ScrollMidDown>
{
    bool sprocess(const bool first_call) const ;
    static const std::string sname() noexcept ;

    explicit ScrollMidDown() ;
    ~ScrollMidDown() ;

    ScrollMidDown(ScrollMidDown&&) ;
    ScrollMidDown& operator=(ScrollMidDown&&) ;

    ScrollMidDown(const ScrollMidDown&) = delete ;
    ScrollMidDown& operator=(const ScrollMidDown&) = delete ;

private:
    struct Impl ;
    std::unique_ptr<Impl> pimpl ;
} ;


struct ScrollPageUp : public BindedFunctionWithCreator<ScrollPageUp>
{
    bool sprocess(const bool first_call) const ;
    static const std::string sname() noexcept ;

    explicit ScrollPageUp() ;
    ~ScrollPageUp() ;

    ScrollPageUp(ScrollPageUp&&) ;
    ScrollPageUp& operator=(ScrollPageUp&&) ;

    ScrollPageUp(const ScrollPageUp&) = delete ;
    ScrollPageUp& operator=(const ScrollPageUp&) = delete ;

private:
    struct Impl ;
    std::unique_ptr<Impl> pimpl ;
} ;


struct ScrollPageDown : public BindedFunctionWithCreator<ScrollPageDown>
{
    bool sprocess(const bool first_call) const ;
    static const std::string sname() noexcept ;

    explicit ScrollPageDown() ;
    ~ScrollPageDown() ;

    ScrollPageDown(ScrollPageDown&&) ;
    ScrollPageDown& operator=(ScrollPageDown&&) ;

    ScrollPageDown(const ScrollPageDown&) = delete ;
    ScrollPageDown& operator=(const ScrollPageDown&) = delete ;

private:
    struct Impl ;
    std::unique_ptr<Impl> pimpl ;
} ;


struct ScrollLeft : public BindedFunctionWithCreator<ScrollLeft>
{
    bool sprocess(const bool first_call) const ;
    static const std::string sname() noexcept ;

    explicit ScrollLeft() ;
    ~ScrollLeft() ;

    ScrollLeft(ScrollLeft&&) ;
    ScrollLeft& operator=(ScrollLeft&&) ;

    ScrollLeft(const ScrollLeft&) = delete ;
    ScrollLeft& operator=(const ScrollLeft&) = delete ;

private:
    struct Impl ;
    std::unique_ptr<Impl> pimpl ;
} ;


struct ScrollRight : public BindedFunctionWithCreator<ScrollRight>
{
    bool sprocess(const bool first_call) const ;
    static const std::string sname() noexcept ;

    explicit ScrollRight() ;
    ~ScrollRight() ;

    ScrollRight(ScrollRight&&) ;
    ScrollRight& operator=(ScrollRight&&) ;

    ScrollRight(const ScrollRight&) = delete ;
    ScrollRight& operator=(const ScrollRight&) = delete ;

private:
    struct Impl ;
    std::unique_ptr<Impl> pimpl ;
} ;


struct ScrollMidLeft : public BindedFunctionWithCreator<ScrollMidLeft>
{
    bool sprocess(const bool first_call) const ;
    static const std::string sname() noexcept ;

    explicit ScrollMidLeft() ;
    ~ScrollMidLeft() ;

    ScrollMidLeft(ScrollMidLeft&&) ;
    ScrollMidLeft& operator=(ScrollMidLeft&&) ;

    ScrollMidLeft(const ScrollMidLeft&) = delete ;
    ScrollMidLeft& operator=(const ScrollMidLeft&) = delete ;

private:
    struct Impl ;
    std::unique_ptr<Impl> pimpl ;
} ;


struct ScrollMidRight : public BindedFunctionWithCreator<ScrollMidRight>
{
    bool sprocess(const bool first_call) const ;
    static const std::string sname() noexcept ;

    explicit ScrollMidRight() ;
    ~ScrollMidRight() ;

    ScrollMidRight(ScrollMidRight&&) ;
    ScrollMidRight& operator=(ScrollMidRight&&) ;

    ScrollMidRight(const ScrollMidRight&) = delete ;
    ScrollMidRight& operator=(const ScrollMidRight&) = delete ;

private:
    struct Impl ;
    std::unique_ptr<Impl> pimpl ;
} ;
#endif