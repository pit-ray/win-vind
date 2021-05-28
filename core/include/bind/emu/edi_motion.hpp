#ifndef _EDI_MOTION_HPP
#define _EDI_MOTION_HPP

#include "bind/binded_func_creator.hpp"

namespace vind
{
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

        void reconstruct() override ;
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

        void reconstruct() override ;
    } ;

    class EdiChangeMotion : public BindedFuncCreator<EdiChangeMotion> {
    private:
        struct Impl ;
        std::unique_ptr<Impl> pimpl ;

    public:
        void sprocess() const ;
        void sprocess(NTypeLogger& parent_lgr) const ;
        void sprocess(const CharLogger& parent_lgr) const ;

        explicit EdiChangeMotion() ;

        virtual ~EdiChangeMotion() noexcept ;
        EdiChangeMotion(EdiChangeMotion&&) ;
        EdiChangeMotion& operator=(EdiChangeMotion&&) ;
        EdiChangeMotion(const EdiChangeMotion&)            = delete ;
        EdiChangeMotion& operator=(const EdiChangeMotion&) = delete ;

        void reconstruct() override ;
    } ;
}

#endif
