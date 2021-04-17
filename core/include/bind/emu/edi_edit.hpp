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
        explicit EdiCopyHighlightText() ;
        static void sprocess() ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;

    struct EdiNCopyLine : public BindedFuncCreator<EdiNCopyLine> {
        explicit EdiNCopyLine() ;
        static void sprocess(
                unsigned int repeat_num=1,
                const textanalyze::SelRes* const exres=nullptr) ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;


    class EdiCopyMotion : public BindedFuncCreator<EdiCopyMotion> {
    private:
        struct Impl ;
        std::unique_ptr<Impl> pimpl ;

    public:
        void sprocess() const ;
        void sprocess(NTypeLogger& parent_lgr) const ;
        void sprocess(const CharLogger& parent_lgr) const ;

        explicit EdiCopyMotion() ;
        virtual ~EdiCopyMotion() noexcept ;

        EdiCopyMotion(EdiCopyMotion&&) ;
        EdiCopyMotion& operator=(EdiCopyMotion&&) ;
        EdiCopyMotion(const EdiCopyMotion&)            = delete ;
        EdiCopyMotion& operator=(const EdiCopyMotion&) = delete ;

        void load_config() override ;
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

        explicit EdiNPasteBefore() ;
        virtual ~EdiNPasteBefore() noexcept ;

        EdiNPasteBefore(EdiNPasteBefore&&) ;
        EdiNPasteBefore& operator=(EdiNPasteBefore&&) ;
        EdiNPasteBefore(const EdiNPasteBefore&)            = delete ;
        EdiNPasteBefore& operator=(const EdiNPasteBefore&) = delete ;
    } ;


    //Delete
    struct EdiDeleteHighlightText : public BindedFuncCreator<EdiDeleteHighlightText> {
        explicit EdiDeleteHighlightText() ;
        static void sprocess() ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;

    class EdiNDeleteLine : public BindedFuncCreator<EdiNDeleteLine> {
    private:
        struct Impl ;
        std::unique_ptr<Impl> pimpl ;

    public:
        void sprocess(unsigned int repeat_num=1) const ;
        void sprocess(NTypeLogger& parent_lgr) const ;
        void sprocess(const CharLogger& parent_lgr) const ;

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

        explicit EdiNDeleteBefore() ;
        virtual ~EdiNDeleteBefore() noexcept ;

        EdiNDeleteBefore(EdiNDeleteBefore&&) ;
        EdiNDeleteBefore& operator=(EdiNDeleteBefore&&) ;
        EdiNDeleteBefore(const EdiNDeleteBefore&)            = delete ;
        EdiNDeleteBefore& operator=(const EdiNDeleteBefore&) = delete ;
    } ;

    class EdiDeleteMotion : public BindedFuncCreator<EdiDeleteMotion> {
    private:
        struct Impl ;
        std::unique_ptr<Impl> pimpl ;

    public:
        void sprocess() const ;
        void sprocess(NTypeLogger& parent_lgr) const ;
        void sprocess(const CharLogger& parent_lgr) const ;

        explicit EdiDeleteMotion() ;

        virtual ~EdiDeleteMotion() noexcept ;
        EdiDeleteMotion(EdiDeleteMotion&&) ;
        EdiDeleteMotion& operator=(EdiDeleteMotion&&) ;
        EdiDeleteMotion(const EdiDeleteMotion&)            = delete ;
        EdiDeleteMotion& operator=(const EdiDeleteMotion&) = delete ;

        void load_config() override ;
    } ;

    class EdiDeleteMotionAndStartInsert : public BindedFuncCreator<EdiDeleteMotionAndStartInsert> {
    private:
        struct Impl ;
        std::unique_ptr<Impl> pimpl ;

    public:
        void sprocess() const ;
        void sprocess(NTypeLogger& parent_lgr) const ;
        void sprocess(const CharLogger& parent_lgr) const ;

        explicit EdiDeleteMotionAndStartInsert() ;

        virtual ~EdiDeleteMotionAndStartInsert() noexcept ;
        EdiDeleteMotionAndStartInsert(EdiDeleteMotionAndStartInsert&&) ;
        EdiDeleteMotionAndStartInsert& operator=(EdiDeleteMotionAndStartInsert&&) ;
        EdiDeleteMotionAndStartInsert(const EdiDeleteMotionAndStartInsert&)            = delete ;
        EdiDeleteMotionAndStartInsert& operator=(const EdiDeleteMotionAndStartInsert&) = delete ;

        void load_config() override ;
    } ;

    struct EdiDeleteLinesAndStartInsert : public BindedFuncCreator<EdiDeleteLinesAndStartInsert> {
        explicit EdiDeleteLinesAndStartInsert() ;
        static void sprocess(unsigned int repeat_num=1) ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;

    struct EdiDeleteCharsAndStartInsert : public BindedFuncCreator<EdiDeleteCharsAndStartInsert> {
        explicit EdiDeleteCharsAndStartInsert() ;
        static void sprocess(unsigned int repeat_num=1) ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;

    struct EdiDeleteUntilEOLAndStartInsert : public BindedFuncCreator<EdiDeleteUntilEOLAndStartInsert> {
        explicit EdiDeleteUntilEOLAndStartInsert() ;
        static void sprocess(
                unsigned int repeat_num=1,
                const textanalyze::SelRes* const exres=nullptr) ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;
}

#endif
