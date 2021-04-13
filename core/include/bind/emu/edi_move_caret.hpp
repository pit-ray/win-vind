#ifndef _EDI_MOVE_CARET_HPP
#define _EDI_MOVE_CARET_HPP

#include "bind/base/binded_func_with_creator.hpp"

namespace vind
{
    class EdiMoveCaretLeft : public BindedFuncCreator<EdiMoveCaretLeft> {
    private:
        struct Impl ;
        std::unique_ptr<Impl> pimpl ;

    public:
        void sprocess(unsigned int repeat_num=1) const ;
        void sprocess(NTypeLogger& parent_lgr) const ;
        void sprocess(const CharLogger& parent_lgr) const ;
        static const std::string sname() noexcept ;

        bool is_for_moving_caret() const noexcept override ;

        explicit EdiMoveCaretLeft() ;
        virtual ~EdiMoveCaretLeft() noexcept ;

        EdiMoveCaretLeft(EdiMoveCaretLeft&&) ;
        EdiMoveCaretLeft& operator=(EdiMoveCaretLeft&&) ;
        EdiMoveCaretLeft(const EdiMoveCaretLeft&)            = delete ;
        EdiMoveCaretLeft& operator=(const EdiMoveCaretLeft&) = delete ;
    } ;

    class EdiMoveCaretRight : public BindedFuncCreator<EdiMoveCaretRight> {
    private:
        struct Impl ;
        std::unique_ptr<Impl> pimpl ;

    public:
        void sprocess(unsigned int repeat_num=1) const ;
        void sprocess(NTypeLogger& parent_lgr) const ;
        void sprocess(const CharLogger& parent_lgr) const ;
        static const std::string sname() noexcept ;

        bool is_for_moving_caret() const noexcept override ;

        explicit EdiMoveCaretRight() ;
        virtual ~EdiMoveCaretRight() noexcept ;

        EdiMoveCaretRight(EdiMoveCaretRight&&) ;
        EdiMoveCaretRight& operator=(EdiMoveCaretRight&&) ;
        EdiMoveCaretRight(const EdiMoveCaretRight&)            = delete ;
        EdiMoveCaretRight& operator=(const EdiMoveCaretRight&) = delete ;
    } ;

    class EdiMoveCaretUp : public BindedFuncCreator<EdiMoveCaretUp> {
    private:
        struct Impl ;
        std::unique_ptr<Impl> pimpl ;

    public:
        void sprocess(unsigned int repeat_num=1) const ;
        void sprocess(NTypeLogger& parent_lgr) const ;
        void sprocess(const CharLogger& parent_lgr) const ;
        static const std::string sname() noexcept ;

        bool is_for_moving_caret() const noexcept override ;

        explicit EdiMoveCaretUp() ;
        virtual ~EdiMoveCaretUp() noexcept ;

        EdiMoveCaretUp(EdiMoveCaretUp&&) ;
        EdiMoveCaretUp& operator=(EdiMoveCaretUp&&) ;
        EdiMoveCaretUp(const EdiMoveCaretUp&)            = delete ;
        EdiMoveCaretUp& operator=(const EdiMoveCaretUp&) = delete ;
    } ;

    class EdiMoveCaretDown : public BindedFuncCreator<EdiMoveCaretDown> {
    private:
        struct Impl ;
        std::unique_ptr<Impl> pimpl ;

    public:
        void sprocess(unsigned int repeat_num=1) const ;
        void sprocess(NTypeLogger& parent_lgr) const ;
        void sprocess(const CharLogger& parent_lgr) const ;
        static const std::string sname() noexcept ;

        bool is_for_moving_caret() const noexcept override ;

        explicit EdiMoveCaretDown() ;
        virtual ~EdiMoveCaretDown() noexcept ;

        EdiMoveCaretDown(EdiMoveCaretDown&&) ;
        EdiMoveCaretDown& operator=(EdiMoveCaretDown&&) ;
        EdiMoveCaretDown(const EdiMoveCaretDown&)            = delete ;
        EdiMoveCaretDown& operator=(const EdiMoveCaretDown&) = delete ;
    } ;


    class EdiNMoveCaretwordsForward : public BindedFuncCreator<EdiNMoveCaretwordsForward> {
    private:
        struct Impl ;
        std::unique_ptr<Impl> pimpl ;

    public:
        void sprocess(unsigned int repeat_num=1) const ;
        void sprocess(NTypeLogger& parent_lgr) const ;
        void sprocess(const CharLogger& parent_lgr) const ;
        static const std::string sname() noexcept ;

        bool is_for_moving_caret() const noexcept override ;

        explicit EdiNMoveCaretwordsForward() ;
        virtual ~EdiNMoveCaretwordsForward() noexcept ;

        EdiNMoveCaretwordsForward(EdiNMoveCaretwordsForward&&) ;
        EdiNMoveCaretwordsForward& operator=(EdiNMoveCaretwordsForward&&) ;
        EdiNMoveCaretwordsForward(const EdiNMoveCaretwordsForward&)            = delete ;
        EdiNMoveCaretwordsForward& operator=(const EdiNMoveCaretwordsForward&) = delete ;
    } ;

    class EdiNMoveCaretwordsBackward : public BindedFuncCreator<EdiNMoveCaretwordsBackward> {
    private:
        struct Impl ;
        std::unique_ptr<Impl> pimpl ;

    public:
        void sprocess(unsigned int repeat_num=1) const ;
        void sprocess(NTypeLogger& parent_lgr) const ;
        void sprocess(const CharLogger& parent_lgr) const ;
        static const std::string sname() noexcept ;

        bool is_for_moving_caret() const noexcept override ;

        explicit EdiNMoveCaretwordsBackward() ;
        virtual ~EdiNMoveCaretwordsBackward() noexcept ;

        EdiNMoveCaretwordsBackward(EdiNMoveCaretwordsBackward&&) ;
        EdiNMoveCaretwordsBackward& operator=(EdiNMoveCaretwordsBackward&&) ;
        EdiNMoveCaretwordsBackward(const EdiNMoveCaretwordsBackward&)            = delete ;
        EdiNMoveCaretwordsBackward& operator=(const EdiNMoveCaretwordsBackward&) = delete ;
    } ;


    class EdiNMoveCaretWORDSForward : public BindedFuncCreator<EdiNMoveCaretWORDSForward> {
    private:
        struct Impl ;
        std::unique_ptr<Impl> pimpl ;

    public:
        void sprocess(unsigned int repeat_num=1) const ;
        void sprocess(NTypeLogger& parent_lgr) const ;
        void sprocess(const CharLogger& parent_lgr) const ;
        static const std::string sname() noexcept ;

        bool is_for_moving_caret() const noexcept override ;

        explicit EdiNMoveCaretWORDSForward() ;
        virtual ~EdiNMoveCaretWORDSForward() noexcept ;

        EdiNMoveCaretWORDSForward(EdiNMoveCaretWORDSForward&&) ;
        EdiNMoveCaretWORDSForward& operator=(EdiNMoveCaretWORDSForward&&) ;
        EdiNMoveCaretWORDSForward(const EdiNMoveCaretWORDSForward&)            = delete ;
        EdiMoveCaretDown& operator=(const EdiMoveCaretDown&) = delete ;
    } ;

    class EdiNMoveCaretWORDSBackward : public BindedFuncCreator<EdiNMoveCaretWORDSBackward> {
    private:
        struct Impl ;
        std::unique_ptr<Impl> pimpl ;

    public:
        void sprocess(unsigned int repeat_num=1) const ;
        void sprocess(NTypeLogger& parent_lgr) const ;
        void sprocess(const CharLogger& parent_lgr) const ;
        static const std::string sname() noexcept ;

        bool is_for_moving_caret() const noexcept override ;

        explicit EdiNMoveCaretWORDSBackward() ;
        virtual ~EdiNMoveCaretWORDSBackward() noexcept ;

        EdiNMoveCaretWORDSBackward(EdiNMoveCaretWORDSBackward&&) ;
        EdiNMoveCaretWORDSBackward& operator=(EdiNMoveCaretWORDSBackward&&) ;
        EdiNMoveCaretWORDSBackward(const EdiNMoveCaretWORDSBackward&)            = delete ;
        EdiNMoveCaretWORDSBackward& operator=(const EdiNMoveCaretWORDSBackward&) = delete ;
    } ;
}

#endif
