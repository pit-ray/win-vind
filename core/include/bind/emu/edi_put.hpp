#ifndef _EDI_PUT_HPP
#define _EDI_PUT_HPP

#include "bind/binded_func_creator.hpp"

namespace vind
{
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
}

#endif
