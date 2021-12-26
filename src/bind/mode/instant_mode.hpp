#ifndef _INSTANT_MODE_HPP
#define _INSTANT_MODE_HPP

#include "bind/bindedfunc.hpp"

namespace vind
{
    namespace bind
    {
        class ToInstantGUINormal : public BindedFuncFlex<ToInstantGUINormal> {
        private:
            struct Impl ;
            std::unique_ptr<Impl> pimpl ;

        public:
            explicit ToInstantGUINormal() ;
            virtual ~ToInstantGUINormal() noexcept ;

            SystemCall sprocess() const ;
            SystemCall sprocess(core::NTypeLogger& parent_lgr) const ;
            SystemCall sprocess(const core::CharLogger& parent_lgr) const ;

            void reconstruct() override ;

            ToInstantGUINormal(ToInstantGUINormal&&) ;
            ToInstantGUINormal& operator=(ToInstantGUINormal&&) ;
            ToInstantGUINormal(const ToInstantGUINormal&)            = delete ;
            ToInstantGUINormal& operator=(const ToInstantGUINormal&) = delete ;
        } ;
    }
}

#endif
