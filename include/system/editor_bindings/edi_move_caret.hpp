#ifndef _EDI_MOVE_CARET_HPP
#define _EDI_MOVE_CARET_HPP

#include "binded_function_with_creator.hpp"


struct EdiMoveCaretLeft : public BindedFunctionWithCreator<EdiMoveCaretLeft>
{
    bool sprocess(const bool first_call) const ;
    static const std::string sname() noexcept ;

    explicit EdiMoveCaretLeft() ;
    virtual ~EdiMoveCaretLeft() ;

    EdiMoveCaretLeft(EdiMoveCaretLeft&&) ;
    EdiMoveCaretLeft& operator=(EdiMoveCaretLeft&&) ;

    EdiMoveCaretLeft(const EdiMoveCaretLeft&) = delete ;
    EdiMoveCaretLeft& operator=(const EdiMoveCaretLeft&) = delete ;

private:
    struct Impl ;
    std::unique_ptr<Impl> pimpl ;
} ;

struct EdiMoveCaretRight : public BindedFunctionWithCreator<EdiMoveCaretRight>
{
    bool sprocess(const bool first_call) const ;
    static const std::string sname() noexcept ;

    explicit EdiMoveCaretRight() ;
    virtual ~EdiMoveCaretRight() ;

    EdiMoveCaretRight(EdiMoveCaretRight&&) ;
    EdiMoveCaretRight& operator=(EdiMoveCaretRight&&) ;

    EdiMoveCaretRight(const EdiMoveCaretRight&) = delete ;
    EdiMoveCaretRight& operator=(const EdiMoveCaretRight&) = delete ;

private:
    struct Impl ;
    std::unique_ptr<Impl> pimpl ;
} ;

struct EdiMoveCaretUp : public BindedFunctionWithCreator<EdiMoveCaretUp>
{
    bool sprocess(const bool first_call) const ;
    static const std::string sname() noexcept ;

    explicit EdiMoveCaretUp() ;
    virtual ~EdiMoveCaretUp() ;

    EdiMoveCaretUp(EdiMoveCaretUp&&) ;
    EdiMoveCaretUp& operator=(EdiMoveCaretUp&&) ;

    EdiMoveCaretUp(const EdiMoveCaretUp&) = delete ;
    EdiMoveCaretUp& operator=(const EdiMoveCaretUp&) = delete ;

private:
    struct Impl ;
    std::unique_ptr<Impl> pimpl ;
} ;

struct EdiMoveCaretDown : public BindedFunctionWithCreator<EdiMoveCaretDown>
{
    bool sprocess(const bool first_call) const ;
    static const std::string sname() noexcept ;

    explicit EdiMoveCaretDown() ;
    virtual ~EdiMoveCaretDown() ;

    EdiMoveCaretDown(EdiMoveCaretDown&&) ;
    EdiMoveCaretDown& operator=(EdiMoveCaretDown&&) ;

    EdiMoveCaretDown(const EdiMoveCaretDown&) = delete ;
    EdiMoveCaretDown& operator=(const EdiMoveCaretDown&) = delete ;

private:
    struct Impl ;
    std::unique_ptr<Impl> pimpl ;
} ;


struct EdiNMoveCaretwordsForward : public BindedFunctionWithCreator<EdiNMoveCaretwordsForward>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;

struct EdiNMoveCaretwordsBackward : public BindedFunctionWithCreator<EdiNMoveCaretwordsBackward>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;


struct EdiNMoveCaretWORDSForward : public BindedFunctionWithCreator<EdiNMoveCaretWORDSForward>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;

struct EdiNMoveCaretWORDSBackward : public BindedFunctionWithCreator<EdiNMoveCaretWORDSBackward>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;

#endif