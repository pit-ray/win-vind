#ifndef _EDI_LAYOUT_HPP
#define _EDI_LAYOUT_HPP

#include "bind/base/binded_func_creator.hpp"

namespace vind
{
    class EdiNRemoveEOL : public BindedFuncCreator<EdiNRemoveEOL> {
    private:
        struct Impl ;
        std::unique_ptr<Impl> pimpl ;

    public:
        void sprocess(unsigned int repeat_num=1) const ;
        void sprocess(NTypeLogger& parent_lgr) const ;
        void sprocess(const CharLogger& parent_lgr) const ;

        explicit EdiNRemoveEOL() ;
        virtual ~EdiNRemoveEOL() noexcept ;

        EdiNRemoveEOL(EdiNRemoveEOL&&) ;
        EdiNRemoveEOL& operator=(EdiNRemoveEOL&&) ;
        EdiNRemoveEOL(const EdiNRemoveEOL&)             = delete ;
        EdiNRemoveEOL& operator=(const EdiNRemoveEOL&)  = delete ;
    } ;
}

#endif
