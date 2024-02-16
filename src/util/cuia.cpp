#include "cuia.hpp"

#include "def.hpp"
#include "winwrap.hpp"

#include "disable_compiler_warning.hpp"

#include <oleauto.h>
#include <windows.h>
#include <winerror.h>
#include <winuser.h>

#include "enable_compiler_warning.hpp"

#include <memory>


namespace vind
{
    namespace util
    {
        CUIA::CUIA()
        : cuia_(nullptr)
        {
            CoInitialize(NULL) ;
            if(util::is_failed(CoCreateInstance(
                            CLSID_CUIAutomation, NULL,
                            CLSCTX_INPROC_SERVER, IID_IUIAutomation,
                            reinterpret_cast<void**>(&cuia_)))) {
                throw LOGIC_EXCEPT("Could not create UIAutomation.") ;
            }
            if(!cuia_) {
                throw LOGIC_EXCEPT("Could not initialize UIAutomation.") ;
            }
        }

        CUIA::~CUIA() noexcept {
            if(cuia_) {
                cuia_->Release() ;
                cuia_ = nullptr ;
            }
            CoUninitialize() ;
        }

        CUIA& CUIA::get_instance() {
            static CUIA instance{} ;
            return instance ;
        }

        IUIAutomation* CUIA::get() const noexcept {
            return cuia_ ;
        }
        CUIA::operator IUIAutomation*() const noexcept {
            return cuia_ ;
        }
        CUIA::operator bool() const noexcept {
            return cuia_ != nullptr ;
        }
        IUIAutomation* CUIA::operator->() const noexcept {
            return cuia_ ;
        }

        SmartCacheReq CUIA::create_cache_request() {
            IUIAutomationCacheRequest* cr_raw ;
            if(util::is_failed(cuia_->CreateCacheRequest(&cr_raw))) {
                throw RUNTIME_EXCEPT("Could not create IUIAutomationCacheRequest.") ;
            }
            if(!cr_raw) {
                throw RUNTIME_EXCEPT("Could not get IUIAutomationCacheRequest properly.") ;
            }
            return SmartCacheReq(cr_raw) ;
        }

        SmartElement CUIA::get_root_element(HWND hwnd) {
            IUIAutomationElement* elem_raw ;
            if(util::is_failed(cuia_->ElementFromHandle(hwnd, &elem_raw))) {
                throw RUNTIME_EXCEPT("Could not get IUIAutomationElement from HWND by COM method.") ;
            }
            if(!elem_raw) {
                throw RUNTIME_EXCEPT("Could not get UIAutomationElement from HWND.") ;
            }
            return SmartElement(elem_raw) ;
        }
    }
}
