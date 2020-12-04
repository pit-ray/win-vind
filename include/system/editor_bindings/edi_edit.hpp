#ifndef _EDI_EDIT_HPP
#define _EDI_EDIT_HPP

#include "binded_func_with_creator.hpp"

namespace TextAnalyzer
{
    class SelRes ;
}

//Copy
struct EdiCopyHighlightText : public BindedFuncWithCreator<EdiCopyHighlightText>
{
    static void sprocess(
            const bool first_call,
            const unsigned int repeat_num,
            const KeyLogger* parent_vkclgr,
            const KeyLogger* const parent_charlgr) ;
    static const std::string sname() noexcept ;
} ;

struct EdiNCopyLine : public BindedFuncWithCreator<EdiNCopyLine>
{
    static void sprocess(
            const bool first_call,
            const unsigned int repeat_num,
            const KeyLogger* parent_vkclgr,
            const KeyLogger* const parent_charlgr,
            const TextAnalyzer::SelRes* const exres=nullptr) ;
    static const std::string sname() noexcept ;
} ;


//Paste
class EdiNPasteAfter : public BindedFuncWithCreator<EdiNPasteAfter>
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

    explicit EdiNPasteAfter() ;
    virtual ~EdiNPasteAfter() noexcept ;

    EdiNPasteAfter(EdiNPasteAfter&&) ;
    EdiNPasteAfter& operator=(EdiNPasteAfter&&) ;
    EdiNPasteAfter(const EdiNPasteAfter&)            = delete ;
    EdiNPasteAfter& operator=(const EdiNPasteAfter&) = delete ;
} ;

class EdiNPasteBefore : public BindedFuncWithCreator<EdiNPasteBefore>
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

    explicit EdiNPasteBefore() ;
    virtual ~EdiNPasteBefore() noexcept ;

    EdiNPasteBefore(EdiNPasteBefore&&) ;
    EdiNPasteBefore& operator=(EdiNPasteBefore&&) ;
    EdiNPasteBefore(const EdiNPasteBefore&)            = delete ;
    EdiNPasteBefore& operator=(const EdiNPasteBefore&) = delete ;
} ;


//Delete
struct EdiDeleteHighlightText : public BindedFuncWithCreator<EdiDeleteHighlightText>
{
    static void sprocess(
            const bool first_call,
            const unsigned int repeat_num,
            const KeyLogger* parent_vkclgr,
            const KeyLogger* const parent_charlgr) ;
    static const std::string sname() noexcept ;
} ;

class EdiNDeleteLine : public BindedFuncWithCreator<EdiNDeleteLine>
{
private:
    struct Impl ;
    std::unique_ptr<Impl> pimpl ;

public:
    void sprocess(
            const bool first_call,
            const unsigned int repeat_num,
            const KeyLogger* parent_vkclgr,
            const KeyLogger* const parent_charlgr,
            const TextAnalyzer::SelRes* const exres=nullptr) const ;
    static const std::string sname() noexcept ;

    explicit EdiNDeleteLine() ;
    virtual ~EdiNDeleteLine() noexcept ;

    EdiNDeleteLine(EdiNDeleteLine&&) ;
    EdiNDeleteLine& operator=(EdiNDeleteLine&&) ;
    EdiNDeleteLine(const EdiNDeleteLine&)            = delete ;
    EdiNDeleteLine& operator=(const EdiNDeleteLine&) = delete ;
} ;


class EdiNDeleteLineUntilEOL : public BindedFuncWithCreator<EdiNDeleteLineUntilEOL>
{
private:
    struct Impl ;
    std::unique_ptr<Impl> pimpl ;

public:
    void sprocess(
            const bool first_call,
            const unsigned int repeat_num,
            const KeyLogger* parent_vkclgr,
            const KeyLogger* const parent_charlgr,
            const TextAnalyzer::SelRes* const exres=nullptr) const ;
    static const std::string sname() noexcept ;

    explicit EdiNDeleteLineUntilEOL() ;
    virtual ~EdiNDeleteLineUntilEOL() noexcept ;

    EdiNDeleteLineUntilEOL(EdiNDeleteLineUntilEOL&&) ;
    EdiNDeleteLineUntilEOL& operator=(EdiNDeleteLineUntilEOL&&) ;
    EdiNDeleteLineUntilEOL(const EdiNDeleteLineUntilEOL&)            = delete ;
    EdiNDeleteLineUntilEOL& operator=(const EdiNDeleteLineUntilEOL&) = delete ;
} ;

class EdiNDeleteAfter : public BindedFuncWithCreator<EdiNDeleteAfter>
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

    explicit EdiNDeleteAfter() ;
    virtual ~EdiNDeleteAfter() noexcept ;

    EdiNDeleteAfter(EdiNDeleteAfter&&) ;
    EdiNDeleteAfter& operator=(EdiNDeleteAfter&&) ;
    EdiNDeleteAfter(const EdiNDeleteAfter&)            = delete ;
    EdiNDeleteAfter& operator=(const EdiNDeleteAfter&) = delete ;
} ;

class EdiNDeleteBefore : public BindedFuncWithCreator<EdiNDeleteBefore>
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

    explicit EdiNDeleteBefore() ;
    virtual ~EdiNDeleteBefore() noexcept ;

    EdiNDeleteBefore(EdiNDeleteBefore&&) ;
    EdiNDeleteBefore& operator=(EdiNDeleteBefore&&) ;
    EdiNDeleteBefore(const EdiNDeleteBefore&)            = delete ;
    EdiNDeleteBefore& operator=(const EdiNDeleteBefore&) = delete ;
} ;

struct EdiDeleteMotionAndStartInsert : public BindedFuncWithCreator<EdiDeleteMotionAndStartInsert>
{
    static void sprocess(
            const bool first_call,
            const unsigned int repeat_num,
            const KeyLogger* parent_vkclgr,
            const KeyLogger* const parent_charlgr) ;
    static const std::string sname() noexcept ;
} ;

struct EdiDeleteLinesAndStartInsert : public BindedFuncWithCreator<EdiDeleteLinesAndStartInsert>
{
    static void sprocess(
            const bool first_call,
            const unsigned int repeat_num,
            const KeyLogger* parent_vkclgr,
            const KeyLogger* const parent_charlgr) ;
    static const std::string sname() noexcept ;
} ;

struct EdiDeleteCharsAndStartInsert : public BindedFuncWithCreator<EdiDeleteCharsAndStartInsert>
{
    static void sprocess(
            const bool first_call,
            const unsigned int repeat_num,
            const KeyLogger* parent_vkclgr,
            const KeyLogger* const parent_charlgr) ;
    static const std::string sname() noexcept ;
} ;

struct EdiDeleteUntilEOLAndStartInsert : public BindedFuncWithCreator<EdiDeleteUntilEOLAndStartInsert>
{
    static void sprocess(
            const bool first_call,
            const unsigned int repeat_num,
            const KeyLogger* parent_vkclgr,
            const KeyLogger* const parent_charlgr,
            const TextAnalyzer::SelRes* const exres=nullptr) ;
    static const std::string sname() noexcept ;
} ;

#endif
