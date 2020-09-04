#ifndef _EDI_MOVE_CARET_HPP
#define _EDI_MOVE_CARET_HPP

#include "key_binding_with_creator.hpp"


struct EdiMoveCaretLeft : public KeyBindingWithCreator<EdiMoveCaretLeft>
{
    bool sprocess(const bool first_call) const ;
    static const std::string sname() noexcept ;

    explicit EdiMoveCaretLeft() ;
    virtual ~EdiMoveCaretLeft() noexcept ;

    EdiMoveCaretLeft(EdiMoveCaretLeft&&) noexcept ;
    EdiMoveCaretLeft& operator=(EdiMoveCaretLeft&&) noexcept ;

    EdiMoveCaretLeft(const EdiMoveCaretLeft&) = delete ;
    EdiMoveCaretLeft& operator=(const EdiMoveCaretLeft&) = delete ;

private:
    struct Impl ;
    std::unique_ptr<Impl> pimpl ;
} ;

struct EdiMoveCaretRight : public KeyBindingWithCreator<EdiMoveCaretRight>
{
    bool sprocess(const bool first_call) const ;
    static const std::string sname() noexcept ;

    explicit EdiMoveCaretRight() ;
    virtual ~EdiMoveCaretRight() noexcept ;

    EdiMoveCaretRight(EdiMoveCaretRight&&) noexcept ;
    EdiMoveCaretRight& operator=(EdiMoveCaretRight&&) noexcept ;

    EdiMoveCaretRight(const EdiMoveCaretRight&) = delete ;
    EdiMoveCaretRight& operator=(const EdiMoveCaretRight&) = delete ;

private:
    struct Impl ;
    std::unique_ptr<Impl> pimpl ;
} ;

struct EdiMoveCaretUp : public KeyBindingWithCreator<EdiMoveCaretUp>
{
    bool sprocess(const bool first_call) const ;
    static const std::string sname() noexcept ;

    explicit EdiMoveCaretUp() ;
    virtual ~EdiMoveCaretUp() noexcept ;

    EdiMoveCaretUp(EdiMoveCaretUp&&) noexcept ;
    EdiMoveCaretUp& operator=(EdiMoveCaretUp&&) noexcept ;

    EdiMoveCaretUp(const EdiMoveCaretUp&) = delete ;
    EdiMoveCaretUp& operator=(const EdiMoveCaretUp&) = delete ;

private:
    struct Impl ;
    std::unique_ptr<Impl> pimpl ;
} ;

struct EdiMoveCaretDown : public KeyBindingWithCreator<EdiMoveCaretDown>
{
    bool sprocess(const bool first_call) const ;
    static const std::string sname() noexcept ;

    explicit EdiMoveCaretDown() ;
    virtual ~EdiMoveCaretDown() noexcept ;

    EdiMoveCaretDown(EdiMoveCaretDown&&) noexcept ;
    EdiMoveCaretDown& operator=(EdiMoveCaretDown&&) noexcept ;

    EdiMoveCaretDown(const EdiMoveCaretDown&) = delete ;
    EdiMoveCaretDown& operator=(const EdiMoveCaretDown&) = delete ;

private:
    struct Impl ;
    std::unique_ptr<Impl> pimpl ;
} ;


struct EdiNMoveCaretwordsForward : public KeyBindingWithCreator<EdiNMoveCaretwordsForward>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;

struct EdiNMoveCaretwordsBackward : public KeyBindingWithCreator<EdiNMoveCaretwordsBackward>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;


struct EdiNMoveCaretWORDSForward : public KeyBindingWithCreator<EdiNMoveCaretWORDSForward>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;

struct EdiNMoveCaretWORDSBackward : public KeyBindingWithCreator<EdiNMoveCaretWORDSBackward>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;

#endif