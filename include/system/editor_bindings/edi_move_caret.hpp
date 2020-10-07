#ifndef _EDI_MOVE_CARET_HPP
#define _EDI_MOVE_CARET_HPP

#include "binded_func_with_creator.hpp"


struct EdiMoveCaretLeft : public BindedFuncWithCreator<EdiMoveCaretLeft>
{
    bool sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger) const ;
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

struct EdiMoveCaretRight : public BindedFuncWithCreator<EdiMoveCaretRight>
{
    bool sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger) const ;
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

struct EdiMoveCaretUp : public BindedFuncWithCreator<EdiMoveCaretUp>
{
    bool sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger) const ;
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

struct EdiMoveCaretDown : public BindedFuncWithCreator<EdiMoveCaretDown>
{
    bool sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger) const ;
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


struct EdiNMoveCaretwordsForward : public BindedFuncWithCreator<EdiNMoveCaretwordsForward>
{
    static bool sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger) ;
    static const std::string sname() noexcept ;
} ;

struct EdiNMoveCaretwordsBackward : public BindedFuncWithCreator<EdiNMoveCaretwordsBackward>
{
    static bool sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger) ;
    static const std::string sname() noexcept ;
} ;


struct EdiNMoveCaretWORDSForward : public BindedFuncWithCreator<EdiNMoveCaretWORDSForward>
{
    static bool sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger) ;
    static const std::string sname() noexcept ;
} ;

struct EdiNMoveCaretWORDSBackward : public BindedFuncWithCreator<EdiNMoveCaretWORDSBackward>
{
    static bool sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger) ;
    static const std::string sname() noexcept ;
} ;

#endif
