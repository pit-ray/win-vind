#ifndef _EDI_PUT_HPP
#define _EDI_PUT_HPP

#include "bind/binded_func_creator.hpp"

namespace vind
{
    //Paste
    class PutAfter : public BindedFuncCreator<PutAfter> {
    private:
        struct Impl ;
        std::unique_ptr<Impl> pimpl ;

    public:
        void sprocess(unsigned int repeat_num=1) const ;
        void sprocess(NTypeLogger& parent_lgr) const ;
        void sprocess(const CharLogger& parent_lgr) const ;

        explicit PutAfter() ;
        virtual ~PutAfter() noexcept ;

        PutAfter(PutAfter&&) ;
        PutAfter& operator=(PutAfter&&) ;
        PutAfter(const PutAfter&)            = delete ;
        PutAfter& operator=(const PutAfter&) = delete ;
    } ;

    class PutBefore : public BindedFuncCreator<PutBefore> {
    private:
        struct Impl ;
        std::unique_ptr<Impl> pimpl ;

    public:
        void sprocess(unsigned int repeat_num=1) const ;
        void sprocess(NTypeLogger& parent_lgr) const ;
        void sprocess(const CharLogger& parent_lgr) const ;

        explicit PutBefore() ;
        virtual ~PutBefore() noexcept ;

        PutBefore(PutBefore&&) ;
        PutBefore& operator=(PutBefore&&) ;
        PutBefore(const PutBefore&)            = delete ;
        PutBefore& operator=(const PutBefore&) = delete ;
    } ;
}

#endif
