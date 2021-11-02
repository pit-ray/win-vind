#include "uia/uia.hpp"

#include "util/def.hpp"
#include "util/winwrap.hpp"

#include "disable_gcc_warning.hpp"

#include <windows.h>
#include "uia/guid_definition.hpp"
#include <oleauto.h>
#include <winerror.h>
#include <winuser.h>

#include "enable_gcc_warning.hpp"

#include <memory>

namespace vind
{
    CUIA::CUIA()
    : cuia(nullptr)
    {
        CoInitialize(NULL) ;
        if(util::is_failed(CoCreateInstance(
                        CLSID_CUIAutomation, NULL,
                        CLSCTX_INPROC_SERVER, IID_IUIAutomation,
                        reinterpret_cast<void**>(&cuia)))) {
            throw LOGIC_EXCEPT("Could not create UIAutomation.") ;
        }
        if(!cuia) {
            throw LOGIC_EXCEPT("Could not initialize UIAutomation.") ;
        }
    }

    CUIA::~CUIA() noexcept {
        if(cuia) {
            cuia->Release() ;
            cuia = nullptr ;
        }
        CoUninitialize() ;
    }

    IUIAutomation* CUIA::get() const noexcept {
        return cuia ;
    }
    CUIA::operator IUIAutomation*() const noexcept {
        return cuia ;
    }
    CUIA::operator bool() const noexcept {
        return cuia != nullptr ;
    }
    IUIAutomation* CUIA::operator->() const noexcept {
        return cuia ;
    }
}
