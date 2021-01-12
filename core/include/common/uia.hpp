#ifndef _UIA_HPP
#define _UIA_HPP

#include "disable_gcc_warning.hpp"
#include <um/uiautomationclient.h>
#include "enable_gcc_warning.hpp"

#include <memory>

namespace UIA
{
    inline void delete_com (IUnknown* com) noexcept {
        if(com != nullptr) com->Release() ;
    }

    using SmartElement      = std::unique_ptr<IUIAutomationElement, decltype(&delete_com)> ;
    using SmartElementArray = std::unique_ptr<IUIAutomationElementArray, decltype(&delete_com)> ;
    using SmartCacheReq     = std::unique_ptr<IUIAutomationCacheRequest, decltype(&delete_com)> ;

    inline SmartElement make_SmartElement(IUIAutomationElement* ptr) {
        return SmartElement(ptr, delete_com) ;
    }
    inline SmartElementArray make_SmartElementArray(IUIAutomationElementArray* ptr) {
        return SmartElementArray(ptr, delete_com) ;
    }
    inline SmartCacheReq make_SmartCacheReq(IUIAutomationCacheRequest* ptr) {
        return SmartCacheReq(ptr, delete_com) ;
    }

    HRESULT create_UIAutomation(IUIAutomation** ptr) ;

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
        IUIAutomation* operator->() const noexcept ;
    } ;
    const CUIA& get_global_cuia() ;
}

#endif
