#ifndef _EDI_MOVE_CARET_HPP
#define _EDI_MOVE_CARET_HPP

#include "binded_func_with_creator.hpp"


class EdiMoveCaretLeft : public BindedFuncWithCreator<EdiMoveCaretLeft>
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

    bool is_for_moving_caret() const noexcept override ;

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
    void sprocess(
            const bool first_call,
            const unsigned int repeat_num,
            KeyLogger* parent_vkclgr,
            const KeyLogger* const parent_charlgr) const ;
    static const std::string sname() noexcept ;

    bool is_for_moving_caret() const noexcept override ;

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
    void sprocess(
            const bool first_call,
            const unsigned int repeat_num,
            KeyLogger* parent_vkclgr,
            const KeyLogger* const parent_charlgr) const ;
    static const std::string sname() noexcept ;

    bool is_for_moving_caret() const noexcept override ;

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
    void sprocess(
            const bool first_call,
            const unsigned int repeat_num,
            KeyLogger* parent_vkclgr,
            const KeyLogger* const parent_charlgr) const ;
    static const std::string sname() noexcept ;

    bool is_for_moving_caret() const noexcept override ;

    explicit EdiMoveCaretDown() ;
    virtual ~EdiMoveCaretDown() noexcept ;

    EdiMoveCaretDown(EdiMoveCaretDown&&) ;
    EdiMoveCaretDown& operator=(EdiMoveCaretDown&&) ;
    EdiMoveCaretDown(const EdiMoveCaretDown&)            = delete ;
    EdiMoveCaretDown& operator=(const EdiMoveCaretDown&) = delete ;
} ;


class EdiNMoveCaretwordsForward : public BindedFuncWithCreator<EdiNMoveCaretwordsForward>
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

    bool is_for_moving_caret() const noexcept override ;

    explicit EdiNMoveCaretwordsForward() ;
    virtual ~EdiNMoveCaretwordsForward() noexcept ;

    EdiNMoveCaretwordsForward(EdiNMoveCaretwordsForward&&) ;
    EdiNMoveCaretwordsForward& operator=(EdiNMoveCaretwordsForward&&) ;
    EdiNMoveCaretwordsForward(const EdiNMoveCaretwordsForward&)            = delete ;
    EdiNMoveCaretwordsForward& operator=(const EdiNMoveCaretwordsForward&) = delete ;
} ;

class EdiNMoveCaretwordsBackward : public BindedFuncWithCreator<EdiNMoveCaretwordsBackward>
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

    bool is_for_moving_caret() const noexcept override ;

    explicit EdiNMoveCaretwordsBackward() ;
    virtual ~EdiNMoveCaretwordsBackward() noexcept ;

    EdiNMoveCaretwordsBackward(EdiNMoveCaretwordsBackward&&) ;
    EdiNMoveCaretwordsBackward& operator=(EdiNMoveCaretwordsBackward&&) ;
    EdiNMoveCaretwordsBackward(const EdiNMoveCaretwordsBackward&)            = delete ;
    EdiNMoveCaretwordsBackward& operator=(const EdiNMoveCaretwordsBackward&) = delete ;
} ;


class EdiNMoveCaretWORDSForward : public BindedFuncWithCreator<EdiNMoveCaretWORDSForward>
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

    bool is_for_moving_caret() const noexcept override ;

    explicit EdiNMoveCaretWORDSForward() ;
    virtual ~EdiNMoveCaretWORDSForward() noexcept ;

    EdiNMoveCaretWORDSForward(EdiNMoveCaretWORDSForward&&) ;
    EdiNMoveCaretWORDSForward& operator=(EdiNMoveCaretWORDSForward&&) ;
    EdiNMoveCaretWORDSForward(const EdiNMoveCaretWORDSForward&)            = delete ;
    EdiMoveCaretDown& operator=(const EdiMoveCaretDown&) = delete ;
} ;

class EdiNMoveCaretWORDSBackward : public BindedFuncWithCreator<EdiNMoveCaretWORDSBackward>
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

    bool is_for_moving_caret() const noexcept override ;

    explicit EdiNMoveCaretWORDSBackward() ;
    virtual ~EdiNMoveCaretWORDSBackward() noexcept ;

    EdiNMoveCaretWORDSBackward(EdiNMoveCaretWORDSBackward&&) ;
    EdiNMoveCaretWORDSBackward& operator=(EdiNMoveCaretWORDSBackward&&) ;
    EdiNMoveCaretWORDSBackward(const EdiNMoveCaretWORDSBackward&)            = delete ;
    EdiNMoveCaretWORDSBackward& operator=(const EdiNMoveCaretWORDSBackward&) = delete ;
} ;

#endif
