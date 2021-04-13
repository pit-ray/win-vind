#ifndef _EDI_EDIT_HPP
#define _EDI_EDIT_HPP

#include "bind/base/binded_func_with_creator.hpp"

namespace vind
{
    namespace textanalyze {
        struct SelRes ;
    }

    //Copy
    struct EdiCopyHighlightText : public BindedFuncCreator<EdiCopyHighlightText> {
        static void sprocess() ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
        static const std::string sname() noexcept ;
    } ;

    struct EdiNCopyLine : public BindedFuncCreator<EdiNCopyLine> {
        static void sprocess(
                unsigned int repeat_num=1,
                const textanalyze::SelRes* const exres=nullptr) ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
        static const std::string sname() noexcept ;
    } ;


    struct EdiCopyMotion : public BindedFuncCreator<EdiCopyMotion> {
        static void sprocess(bool repeat_num=1) ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
        static const std::string sname() noexcept ;
    } ;


    //Paste
    class EdiNPasteAfter : public BindedFuncCreator<EdiNPasteAfter> {
    private:
        struct Impl ;
        std::unique_ptr<Impl> pimpl ;

    public:
        void sprocess(unsigned int repeat_num=1) const ;
        void sprocess(NTypeLogger& parent_lgr) const ;
        void sprocess(const CharLogger& parent_lgr) const ;
        static const std::string sname() noexcept ;

        explicit EdiNPasteAfter() ;
        virtual ~EdiNPasteAfter() noexcept ;

        EdiNPasteAfter(EdiNPasteAfter&&) ;
        EdiNPasteAfter& operator=(EdiNPasteAfter&&) ;
        EdiNPasteAfter(const EdiNPasteAfter&)            = delete ;
        EdiNPasteAfter& operator=(const EdiNPasteAfter&) = delete ;
    } ;

    class EdiNPasteBefore : public BindedFuncCreator<EdiNPasteBefore> {
    private:
        struct Impl ;
        std::unique_ptr<Impl> pimpl ;

    public:
        void sprocess(unsigned int repeat_num=1) const ;
        void sprocess(NTypeLogger& parent_lgr) const ;
        void sprocess(const CharLogger& parent_lgr) const ;
        static const std::string sname() noexcept ;

        explicit EdiNPasteBefore() ;
        virtual ~EdiNPasteBefore() noexcept ;

        EdiNPasteBefore(EdiNPasteBefore&&) ;
        EdiNPasteBefore& operator=(EdiNPasteBefore&&) ;
        EdiNPasteBefore(const EdiNPasteBefore&)            = delete ;
        EdiNPasteBefore& operator=(const EdiNPasteBefore&) = delete ;
    } ;


    //Delete
    struct EdiDeleteHighlightText : public BindedFuncCreator<EdiDeleteHighlightText> {
        static void sprocess() ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
        static const std::string sname() noexcept ;
    } ;

    class EdiNDeleteLine : public BindedFuncCreator<EdiNDeleteLine> {
    private:
        struct Impl ;
        std::unique_ptr<Impl> pimpl ;

    public:
        void sprocess(unsigned int repeat_num=1) const ;
        void sprocess(NTypeLogger& parent_lgr) const ;
        void sprocess(const CharLogger& parent_lgr) const ;
        static const std::string sname() noexcept ;

        explicit EdiNDeleteLine() ;
        virtual ~EdiNDeleteLine() noexcept ;

        EdiNDeleteLine(EdiNDeleteLine&&) ;
        EdiNDeleteLine& operator=(EdiNDeleteLine&&) ;
        EdiNDeleteLine(const EdiNDeleteLine&)            = delete ;
        EdiNDeleteLine& operator=(const EdiNDeleteLine&) = delete ;
    } ;


    class EdiNDeleteLineUntilEOL : public BindedFuncCreator<EdiNDeleteLineUntilEOL> {
    private:
        struct Impl ;
        std::unique_ptr<Impl> pimpl ;

    public:
        void sprocess(unsigned int repeat_num=1) const ;
        void sprocess(NTypeLogger& parent_lgr) const ;
        void sprocess(const CharLogger& parent_lgr) const ;
        static const std::string sname() noexcept ;

        explicit EdiNDeleteLineUntilEOL() ;
        virtual ~EdiNDeleteLineUntilEOL() noexcept ;

        EdiNDeleteLineUntilEOL(EdiNDeleteLineUntilEOL&&) ;
        EdiNDeleteLineUntilEOL& operator=(EdiNDeleteLineUntilEOL&&) ;
        EdiNDeleteLineUntilEOL(const EdiNDeleteLineUntilEOL&)            = delete ;
        EdiNDeleteLineUntilEOL& operator=(const EdiNDeleteLineUntilEOL&) = delete ;
    } ;

    class EdiNDeleteAfter : public BindedFuncCreator<EdiNDeleteAfter> {
    private:
        struct Impl ;
        std::unique_ptr<Impl> pimpl ;

    public:
        void sprocess(unsigned int repeat_num=1) const ;
        void sprocess(NTypeLogger& parent_lgr) const ;
        void sprocess(const CharLogger& parent_lgr) const ;
        static const std::string sname() noexcept ;

        explicit EdiNDeleteAfter() ;
        virtual ~EdiNDeleteAfter() noexcept ;

        EdiNDeleteAfter(EdiNDeleteAfter&&) ;
        EdiNDeleteAfter& operator=(EdiNDeleteAfter&&) ;
        EdiNDeleteAfter(const EdiNDeleteAfter&)            = delete ;
        EdiNDeleteAfter& operator=(const EdiNDeleteAfter&) = delete ;
    } ;

    class EdiNDeleteBefore : public BindedFuncCreator<EdiNDeleteBefore> {
    private:
        struct Impl ;
        std::unique_ptr<Impl> pimpl ;

    public:
        void sprocess(unsigned int repeat_num=1) const ;
        void sprocess(NTypeLogger& parent_lgr) const ;
        void sprocess(const CharLogger& parent_lgr) const ;
        static const std::string sname() noexcept ;

        explicit EdiNDeleteBefore() ;
        virtual ~EdiNDeleteBefore() noexcept ;

        EdiNDeleteBefore(EdiNDeleteBefore&&) ;
        EdiNDeleteBefore& operator=(EdiNDeleteBefore&&) ;
        EdiNDeleteBefore(const EdiNDeleteBefore&)            = delete ;
        EdiNDeleteBefore& operator=(const EdiNDeleteBefore&) = delete ;
    } ;

    struct EdiDeleteMotion : public BindedFuncCreator<EdiDeleteMotion> {
        static void sprocess(
                unsigned int repeat_num=1,
                NTypeLogger* const lgrptr=nullptr) ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
        static const std::string sname() noexcept ;
    } ;

    struct EdiDeleteMotionAndStartInsert : public BindedFuncCreator<EdiDeleteMotionAndStartInsert> {
        static void sprocess(
                unsigned int repeat_num=1,
                NTypeLogger* const lgrptr=nullptr) ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
        static const std::string sname() noexcept ;
    } ;

    struct EdiDeleteLinesAndStartInsert : public BindedFuncCreator<EdiDeleteLinesAndStartInsert> {
        static void sprocess(unsigned int repeat_num=1) ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
        static const std::string sname() noexcept ;
    } ;

    struct EdiDeleteCharsAndStartInsert : public BindedFuncCreator<EdiDeleteCharsAndStartInsert> {
        static void sprocess(unsigned int repeat_num=1) ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
        static const std::string sname() noexcept ;
    } ;

    struct EdiDeleteUntilEOLAndStartInsert : public BindedFuncCreator<EdiDeleteUntilEOLAndStartInsert> {
        static void sprocess(
                unsigned int repeat_num=1,
                const textanalyze::SelRes* const exres=nullptr) ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
        static const std::string sname() noexcept ;
    } ;
}

#endif
