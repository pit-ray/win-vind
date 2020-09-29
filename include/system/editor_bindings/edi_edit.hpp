#ifndef _EDI_EDIT_HPP
#define _EDI_EDIT_HPP

#include "key_binding_with_creator.hpp"

namespace TextAnalyzer {
    class SelRes ;
}

//Copy
struct EdiCopyHighlightText : public KeyBindingWithCreator<EdiCopyHighlightText>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;

struct EdiNCopyLine : public KeyBindingWithCreator<EdiNCopyLine>
{
    static bool sprocess(const bool first_call, const TextAnalyzer::SelRes* const exres=nullptr) ;
    static const std::string sname() noexcept ;
} ;


//Paste
struct EdiNPasteAfter : public KeyBindingWithCreator<EdiNPasteAfter>
{
    bool sprocess(const bool first_call) const ;
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

struct EdiNPasteBefore : public KeyBindingWithCreator<EdiNPasteBefore>
{
    bool sprocess(const bool first_call) const ;
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
struct EdiDeleteHighlightText : public KeyBindingWithCreator<EdiDeleteHighlightText>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;

struct EdiNDeleteLine : public KeyBindingWithCreator<EdiNDeleteLine>
{
    bool sprocess(const bool first_call, const TextAnalyzer::SelRes* const exres=nullptr) const ;
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


struct EdiNDeleteLineUntilEOL : public KeyBindingWithCreator<EdiNDeleteLineUntilEOL>
{
    bool sprocess(const bool first_call, const TextAnalyzer::SelRes* const exres=nullptr) const ;
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

struct EdiNDeleteAfter : public KeyBindingWithCreator<EdiNDeleteAfter>
{
    bool sprocess(const bool first_call) const ;
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

struct EdiNDeleteBefore : public KeyBindingWithCreator<EdiNDeleteBefore>
{
    bool sprocess(const bool first_call) const ;
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

struct EdiDeleteMotionAndStartInsert : public KeyBindingWithCreator<EdiDeleteMotionAndStartInsert>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;

struct EdiDeleteLinesAndStartInsert : public KeyBindingWithCreator<EdiDeleteLinesAndStartInsert>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;

struct EdiDeleteCharsAndStartInsert : public KeyBindingWithCreator<EdiDeleteCharsAndStartInsert>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;

struct EdiDeleteUntilEOLAndStartInsert : public KeyBindingWithCreator<EdiDeleteUntilEOLAndStartInsert>
{
    static bool sprocess(const bool first_call, const TextAnalyzer::SelRes* const exres=nullptr) ;
    static const std::string sname() noexcept ;
} ;

#endif
