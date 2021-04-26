#include "bind/uia/uia.hpp"

#include "util/def.hpp"

#include "disable_gcc_warning.hpp"

#include <windows.h>
#include "bind/uia/guid_definition.hpp"
#include <oleauto.h>
#include <winerror.h>
#include <winuser.h>

#include "enable_gcc_warning.hpp"

#include <memory>

namespace vind
{
    namespace uiauto {

        CUIA::CUIA()
        : cuia(nullptr)
        {
            CoInitialize(NULL) ;
            if(FAILED(create_UIAutomation(&cuia))) {
                throw LOGIC_EXCEPT("Could not create UIAutomation.") ;
            }
            if(!cuia) {
                throw LOGIC_EXCEPT("Could not initialize UIAutomation.") ;
            }
        }

        CUIA::~CUIA() {
            if(cuia != nullptr) cuia->Release() ;
            CoUninitialize() ;
        }

        IUIAutomation* CUIA::get() const noexcept {
            return cuia ;
        }
        CUIA::operator IUIAutomation*() const noexcept {
            return cuia ;
        }
        IUIAutomation* CUIA::operator->() const noexcept {
            return cuia ;
        }

        HRESULT create_UIAutomation(IUIAutomation** ptr) {
            return CoCreateInstance(CLSID_CUIAutomation, NULL,
                    CLSCTX_INPROC_SERVER, IID_IUIAutomation,
                    reinterpret_cast<void**>(ptr)) ;
        }

        const CUIA& get_global_cuia() {
            static CUIA g_cuia{} ;
            return g_cuia ;
        }

        inline void delete_com (IUnknown* com) noexcept {
            if(com != nullptr) com->Release() ;
        }

        SmartElement make_SmartElement(IUIAutomationElement* ptr) {
            return SmartElement(ptr, delete_com) ;
        }
        SmartElementArray make_SmartElementArray(IUIAutomationElementArray* ptr) {
            return SmartElementArray(ptr, delete_com) ;
        }
        SmartCacheReq make_SmartCacheReq(IUIAutomationCacheRequest* ptr) {
            return SmartCacheReq(ptr, delete_com) ;
        }
    }
}
