#include "uia_global.hpp"
#include "uia.hpp"

#include "utility.hpp"

#include "disable_gcc_warning.hpp"
#include <windows.h>
#include "guid_definition.hpp"
#include <oleauto.h>
#include <winerror.h>
#include <winuser.h>
#include "enable_gcc_warning.hpp"

#include <memory>

namespace UIA
{
    auto delete_com_with_clear = [] (IUnknown* com) {
        if(com != nullptr) com->Release() ;
        CoUninitialize() ;
    } ;
    using SmartCUIA = std::shared_ptr<IUIAutomation> ;

    static SmartCUIA g_cuia(nullptr, delete_com_with_clear) ;

    HRESULT create_UIAutomation(IUIAutomation** ptr) {
        return CoCreateInstance(CLSID_CUIAutomation, NULL,
                CLSCTX_INPROC_SERVER, IID_IUIAutomation,
                reinterpret_cast<void**>(ptr)) ;
    }

    void initialize() {
        CoInitialize(NULL) ;
        IUIAutomation* cuia_raw ;
        if(FAILED(create_UIAutomation(&cuia_raw))) {
            throw LOGIC_EXCEPT("Could not create UIAutomation.") ;
        }
        if(!cuia_raw) {
            throw LOGIC_EXCEPT("Could not initialize UIAutomation.") ;
        }
        g_cuia.reset(cuia_raw) ;
    }

    const SmartCUIA& get_global_cuia() {
        return g_cuia ;
    }
}
