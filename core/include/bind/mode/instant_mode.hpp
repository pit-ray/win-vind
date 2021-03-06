#ifndef _INSTANT_MODE_HPP
#define _INSTANT_MODE_HPP

#include "bind/binded_func_creator.hpp"

namespace vind
{
    class ToInstantGUINormal : public BindedFuncCreator<ToInstantGUINormal> {
    private:
        struct Impl ;
        std::unique_ptr<Impl> pimpl ;

    public:
        explicit ToInstantGUINormal() ;
        virtual ~ToInstantGUINormal() noexcept ;

        void sprocess() const ;
        void sprocess(NTypeLogger& parent_lgr) const ;
        void sprocess(const CharLogger& parent_lgr) const ;

        void reconstruct() override ;

        ToInstantGUINormal(ToInstantGUINormal&&) ;
        ToInstantGUINormal& operator=(ToInstantGUINormal&&) ;
        ToInstantGUINormal(const ToInstantGUINormal&)            = delete ;
        ToInstantGUINormal& operator=(const ToInstantGUINormal&) = delete ;
    } ;
}

#endif
