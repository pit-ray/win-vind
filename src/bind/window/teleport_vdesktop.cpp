#include "teleport_vdesktop.hpp"

#include "core/ntype_logger.hpp"
#include "util/smartcom.hpp"
#include "util/winwrap.hpp"

#include <windows.h>

// #include <shobjidl.h>


namespace vind
{
    namespace bind
    {
        TeleportOverVDesktop::TeleportOverVDesktop()
        : BindedFuncVoid("teleport_over_vdesktop")
        {}

        void TeleportOverVDesktop::sprocess(unsigned int desktop_id) {
#ifdef DEBUG
            /*
            if(util::is_failed(CoInitialize(NULL))) {
                throw RUNTIME_EXCEPT("COM initialization failed") ;
            }
            auto co_uninit = [] (char* ptr) {
                if(ptr) {
                    delete ptr ;
                    ptr = nullptr ;
                }
                CoUninitialize() ;
            } ;
            std::unique_ptr<char, decltype(co_uninit)> smart_uninit{new char(), co_uninit} ;

            SmartCom<IVirtualDesktopManager> vdm{} ;
            if(util::is_failed(CoCreateInstance(
                CLSID_VirtualDesktopManager,
                NULL,
                CLSCTX_ALL,
                IID_IVirtualDesktopManager,
                reinterpret_cast<void**>(&vdm)))) {
                throw RUNTIME_EXCEPT("IVirtualDesktopManager creation failed.") ;
            }

            auto hwnd = GetForegroundWindow() ;
            GUID id ;
            if(util::is_failed(vdm->GetWindowDesktopId(hwnd, &id))) {
                std::cout << "failed\n" ;
            }

            std::cout << "ID: " << id.Data1 << "." << id.Data2 << "." << id.Data3 << "." << id.Data4 << std::endl ;
            */
#endif
        }
        void TeleportOverVDesktop::sprocess(core::NTypeLogger& parent_lgr) {
#ifdef DEBUG
            if(!parent_lgr.is_long_pressing()) {
                sprocess(1) ;
            }
#endif
        }
        void TeleportOverVDesktop::sprocess(const core::CharLogger& UNUSED(parent_lgr)) {
#ifdef DEBUG
            sprocess(1) ;
#endif
        }
    }
}
