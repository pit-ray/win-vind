#ifndef _EDI_MOVE_CARET_HPP
#define _EDI_MOVE_CARET_HPP

#include "binded_func_with_creator.hpp"


class EdiMoveCaretLeft : public BindedFuncWithCreator<EdiMoveCaretLeft>
{
private:
    struct Impl ;
    std::unique_ptr<Impl> pimpl ;

public:
    void sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger) const ;
    static const std::string sname() noexcept ;

    explicit EdiMoveCaretLeft() ;
    virtual ~EdiMoveCaretLeft() noexcept ;

    EdiMoveCaretLeft(EdiMoveCaretLeft&&) ;
    EdiMoveCaretLeft& operator=(EdiMoveCaretLeft&&) ;
    EdiMoveCaretLeft(const EdiMoveCaretLeft&)            = delete ;
    EdiMoveCaretLeft& operator=(const EdiMoveCaretLeft&) = delete ;
} ;

class EdiMoveCaretRight : public BindedFuncWithCreator<EdiMoveCaretRight>
{
private:
    struct Impl ;
    std::unique_ptr<Impl> pimpl ;

public:
    void sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger) const ;
    static const std::string sname() noexcept ;

    explicit EdiMoveCaretRight() ;
    virtual ~EdiMoveCaretRight() noexcept ;

    EdiMoveCaretRight(EdiMoveCaretRight&&) ;
    EdiMoveCaretRight& operator=(EdiMoveCaretRight&&) ;
    EdiMoveCaretRight(const EdiMoveCaretRight&)            = delete ;
    EdiMoveCaretRight& operator=(const EdiMoveCaretRight&) = delete ;
} ;

class EdiMoveCaretUp : public BindedFuncWithCreator<EdiMoveCaretUp>
{
private:
    struct Impl ;
    std::unique_ptr<Impl> pimpl ;

public:
    void sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger) const ;
    static const std::string sname() noexcept ;

    explicit EdiMoveCaretUp() ;
    virtual ~EdiMoveCaretUp() noexcept ;

    EdiMoveCaretUp(EdiMoveCaretUp&&) ;
    EdiMoveCaretUp& operator=(EdiMoveCaretUp&&) ;
    EdiMoveCaretUp(const EdiMoveCaretUp&)            = delete ;
    EdiMoveCaretUp& operator=(const EdiMoveCaretUp&) = delete ;
} ;

class EdiMoveCaretDown : public BindedFuncWithCreator<EdiMoveCaretDown>
{
private:
    struct Impl ;
    std::unique_ptr<Impl> pimpl ;

public:
    void sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger) const ;
    static const std::string sname() noexcept ;

    explicit EdiMoveCaretDown() ;
    virtual ~EdiMoveCaretDown() noexcept ;

    EdiMoveCaretDown(EdiMoveCaretDown&&) ;
    EdiMoveCaretDown& operator=(EdiMoveCaretDown&&) ;
    EdiMoveCaretDown(const EdiMoveCaretDown&)            = delete ;
    EdiMoveCaretDown& operator=(const EdiMoveCaretDown&) = delete ;
} ;


struct EdiNMoveCaretwordsForward : public BindedFuncWithCreator<EdiNMoveCaretwordsForward>
{
    static void sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger) ;
    static const std::string sname() noexcept ;
} ;

struct EdiNMoveCaretwordsBackward : public BindedFuncWithCreator<EdiNMoveCaretwordsBackward>
{
    static void sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger) ;
    static const std::string sname() noexcept ;
} ;


struct EdiNMoveCaretWORDSForward : public BindedFuncWithCreator<EdiNMoveCaretWORDSForward>
{
    static void sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger) ;
    static const std::string sname() noexcept ;
} ;

struct EdiNMoveCaretWORDSBackward : public BindedFuncWithCreator<EdiNMoveCaretWORDSBackward>
{
    static void sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger) ;
    static const std::string sname() noexcept ;
} ;

#endif
