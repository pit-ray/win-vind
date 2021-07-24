#ifndef _CUIA_HPP
#define _CUIA_HPP

#include "uia/uiafwd.hpp"

namespace vind
{
    class CUIA {
    private:
        IUIAutomation* cuia ;

    public:
        explicit CUIA() ;
        virtual ~CUIA() noexcept ;

        CUIA(CUIA&&)                 = delete ;
        CUIA& operator=(CUIA&&)      = delete ;
        CUIA(const CUIA&)            = delete ;
        CUIA& operator=(const CUIA&) = delete ;

        IUIAutomation* get() const noexcept ;
        operator IUIAutomation*() const noexcept ;
        operator bool() const noexcept ;
        IUIAutomation* operator->() const noexcept ;
    } ;
}

#endif
