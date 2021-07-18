#ifndef _UIA_HPP
#define _UIA_HPP

#include <windows.h>

#include "disable_gcc_warning.hpp"

#if defined(_MSC_VER) && _MSC_VER >= 1500
#include <uiautomationclient.h>
#else
#include <um/uiautomationclient.h>
#endif

#include "enable_gcc_warning.hpp"

#include "uia/smartcom.hpp"

#include <memory>


namespace vind
{
    namespace uiauto {
        inline void delete_com (IUnknown* com) noexcept {
            if(com != nullptr) {
                com->Release() ;
            }
        }

        using SmartElement      = SmartCom<IUIAutomationElement> ;
        using SmartElementArray = SmartCom<IUIAutomationElementArray> ;
        using SmartCacheReq     = SmartCom<IUIAutomationCacheRequest> ;

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
}

#endif
