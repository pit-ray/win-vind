#ifndef _EDI_EDIT_HPP
#define _EDI_EDIT_HPP

#include "binded_func_with_creator.hpp"

namespace TextAnalyzer {
    class SelRes ;
}

//Copy
struct EdiCopyHighlightText : public BindedFuncWithCreator<EdiCopyHighlightText>
{
    static bool sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger) ;
    static const std::string sname() noexcept ;
} ;

struct EdiNCopyLine : public BindedFuncWithCreator<EdiNCopyLine>
{
    static bool sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger, const TextAnalyzer::SelRes* const exres=nullptr) ;
    static const std::string sname() noexcept ;
} ;


//Paste
struct EdiNPasteAfter : public BindedFuncWithCreator<EdiNPasteAfter>
{
    bool sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger) const ;
    static const std::string sname() noexcept ;

    explicit EdiNPasteAfter() ;
    virtual ~EdiNPasteAfter() noexcept ;

    EdiNPasteAfter(EdiNPasteAfter&&) noexcept ;
    EdiNPasteAfter& operator=(EdiNPasteAfter&&) noexcept ;

    EdiNPasteAfter(const EdiNPasteAfter&) = delete ;
    EdiNPasteAfter& operator=(const EdiNPasteAfter&) = delete ;

private:
    struct Impl ;
    std::unique_ptr<Impl> pimpl ;
} ;

struct EdiNPasteBefore : public BindedFuncWithCreator<EdiNPasteBefore>
{
    bool sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger) const ;
    static const std::string sname() noexcept ;

    explicit EdiNPasteBefore() ;
    virtual ~EdiNPasteBefore() noexcept ;

    EdiNPasteBefore(EdiNPasteBefore&&) noexcept ;
    EdiNPasteBefore& operator=(EdiNPasteBefore&&) noexcept ;

    EdiNPasteBefore(const EdiNPasteBefore&) = delete ;
    EdiNPasteBefore& operator=(const EdiNPasteBefore&) = delete ;

private:
    struct Impl ;
    std::unique_ptr<Impl> pimpl ;
} ;


//Delete
struct EdiDeleteHighlightText : public BindedFuncWithCreator<EdiDeleteHighlightText>
{
    static bool sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger) ;
    static const std::string sname() noexcept ;
} ;

struct EdiNDeleteLine : public BindedFuncWithCreator<EdiNDeleteLine>
{
    bool sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger, const TextAnalyzer::SelRes* const exres=nullptr) const ;
    static const std::string sname() noexcept ;

    explicit EdiNDeleteLine() ;
    virtual ~EdiNDeleteLine() noexcept ;

    EdiNDeleteLine(EdiNDeleteLine&&) noexcept ;
    EdiNDeleteLine& operator=(EdiNDeleteLine&&) noexcept ;

    EdiNDeleteLine(const EdiNDeleteLine&) = delete ;
    EdiNDeleteLine& operator=(const EdiNDeleteLine&) = delete ;

private:
    struct Impl ;
    std::unique_ptr<Impl> pimpl ;
} ;


struct EdiNDeleteLineUntilEOL : public BindedFuncWithCreator<EdiNDeleteLineUntilEOL>
{
    bool sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger, const TextAnalyzer::SelRes* const exres=nullptr) const ;
    static const std::string sname() noexcept ;

    explicit EdiNDeleteLineUntilEOL() ;
    virtual ~EdiNDeleteLineUntilEOL() noexcept ;

    EdiNDeleteLineUntilEOL(EdiNDeleteLineUntilEOL&&) noexcept ;
    EdiNDeleteLineUntilEOL& operator=(EdiNDeleteLineUntilEOL&&) noexcept ;

    EdiNDeleteLineUntilEOL(const EdiNDeleteLineUntilEOL&) = delete ;
    EdiNDeleteLineUntilEOL& operator=(const EdiNDeleteLineUntilEOL&) = delete ;

private:
    struct Impl ;
    std::unique_ptr<Impl> pimpl ;
} ;

struct EdiNDeleteAfter : public BindedFuncWithCreator<EdiNDeleteAfter>
{
    bool sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger) const ;
    static const std::string sname() noexcept ;

    explicit EdiNDeleteAfter() ;
    virtual ~EdiNDeleteAfter() noexcept ;

    EdiNDeleteAfter(EdiNDeleteAfter&&) noexcept ;
    EdiNDeleteAfter& operator=(EdiNDeleteAfter&&) noexcept ;

    EdiNDeleteAfter(const EdiNDeleteAfter&) = delete ;
    EdiNDeleteAfter& operator=(const EdiNDeleteAfter&) = delete ;

private:
    struct Impl ;
    std::unique_ptr<Impl> pimpl ;
} ;

struct EdiNDeleteBefore : public BindedFuncWithCreator<EdiNDeleteBefore>
{
    bool sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger) const ;
    static const std::string sname() noexcept ;

    explicit EdiNDeleteBefore() ;
    virtual ~EdiNDeleteBefore() noexcept ;

    EdiNDeleteBefore(EdiNDeleteBefore&&) noexcept ;
    EdiNDeleteBefore& operator=(EdiNDeleteBefore&&) noexcept ;

    EdiNDeleteBefore(const EdiNDeleteBefore&) = delete ;
    EdiNDeleteBefore& operator=(const EdiNDeleteBefore&) = delete ;

private:
    struct Impl ;
    std::unique_ptr<Impl> pimpl ;
} ;

struct EdiDeleteMotionAndStartInsert : public BindedFuncWithCreator<EdiDeleteMotionAndStartInsert>
{
    static bool sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger) ;
    static const std::string sname() noexcept ;
} ;

struct EdiDeleteLinesAndStartInsert : public BindedFuncWithCreator<EdiDeleteLinesAndStartInsert>
{
    static bool sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger) ;
    static const std::string sname() noexcept ;
} ;

struct EdiDeleteCharsAndStartInsert : public BindedFuncWithCreator<EdiDeleteCharsAndStartInsert>
{
    static bool sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger) ;
    static const std::string sname() noexcept ;
} ;

struct EdiDeleteUntilEOLAndStartInsert : public BindedFuncWithCreator<EdiDeleteUntilEOLAndStartInsert>
{
    static bool sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger, const TextAnalyzer::SelRes* const exres=nullptr) ;
    static const std::string sname() noexcept ;
} ;

#endif
