#include "bind/file/explorer_util.hpp"

#include <memory>

#include <exdisp.h>
#include <shlobj.h>

#include "err_logger.hpp"
#include "util/def.hpp"
#include "util/string.hpp"
#include "util/winwrap.hpp"


namespace vind
{
    namespace explorer {
        //This is based on https://devblogs.microsoft.com/oldnewthing/?p=38393 .
        std::string get_current_explorer_path() {
            using PathType = std::string ;

            const auto hwnd = GetForegroundWindow() ;

            if(hwnd == NULL) {
                return PathType{} ;
            }

            if(util::is_failed(CoInitialize(NULL))) {
                throw RUNTIME_EXCEPT("initialization failed") ;
            }

            //we can get explorer handle from IShellWindows.
            IShellWindows* raw_psw = nullptr ;
            if(util::is_failed(CoCreateInstance(
                CLSID_ShellWindows,
                NULL,
                CLSCTX_ALL,
                IID_IShellWindows,
                reinterpret_cast<void**>(&raw_psw)
            ))) {
                throw RUNTIME_EXCEPT("cannot create IShellWindows.") ;
            }
            auto sw_deleter = [](IShellWindows* ptr) {ptr->Release() ;} ;
            std::unique_ptr<IShellWindows, decltype(sw_deleter)> psw(raw_psw, sw_deleter) ;

            long win_num = 0 ;
            if(util::is_failed(psw->get_Count(&win_num))) {
                throw RUNTIME_EXCEPT("No explorer is opened.") ;
            }

            for(long i = 0 ; i < win_num ; i ++) {
                VARIANT v ;
                v.vt = VT_I4 ;
                V_I4(&v) = i ;

                //IDispatch is an interface to object, method or property
                IDispatch* raw_pdisp = nullptr ;
                if(util::is_failed(psw->Item(v, &raw_pdisp))) {
                    continue ;
                }
                auto disp_deleter = [](IDispatch* ptr) {ptr->Release() ;} ;
                std::unique_ptr<IDispatch, decltype(disp_deleter)> pdisp(raw_pdisp, disp_deleter) ;

                //Is this shell foreground window??
                IWebBrowserApp* raw_pwba = nullptr ;
                if(util::is_failed(pdisp->QueryInterface(IID_IWebBrowserApp, reinterpret_cast<void**>(&raw_pwba)))) {
                    continue ;
                }
                auto wba_deleter = [](IWebBrowserApp* ptr) {ptr->Release() ;} ;
                std::unique_ptr<IWebBrowserApp, decltype(wba_deleter)> pwba(raw_pwba, wba_deleter) ;
                HWND shell_hwnd = NULL ;
                if(util::is_failed(pwba->get_HWND(reinterpret_cast<LONG_PTR*>(&shell_hwnd)))) {
                    continue ;
                }
                if(shell_hwnd != hwnd) {
                    continue ; //it is not foreground window
                }

                //access to shell window
                IServiceProvider* raw_psp = nullptr ;
                if(util::is_failed(pwba->QueryInterface(IID_IServiceProvider, reinterpret_cast<void**>(&raw_psp)))) {
                    throw RUNTIME_EXCEPT("cannot access a top service provider.") ;
                }
                auto sp_deleter = [](IServiceProvider* ptr) {ptr->Release() ;} ;
                std::unique_ptr<IServiceProvider, decltype(sp_deleter)> psp(raw_psp, sp_deleter) ;

                //access to shell browser
                IShellBrowser* raw_psb = nullptr ;
                if(util::is_failed(psp->QueryService(SID_STopLevelBrowser, IID_IShellBrowser, reinterpret_cast<void**>(&raw_psb)))) {
                    throw RUNTIME_EXCEPT("cannot access a shell browser.") ;
                }
                auto sb_deleter = [](IShellBrowser* ptr) {ptr->Release() ;} ;
                std::unique_ptr<IShellBrowser, decltype(sb_deleter)> psb(raw_psb, sb_deleter) ;

                //access to shell view
                IShellView* raw_psv = nullptr ;
                if(util::is_failed(psb->QueryActiveShellView(&raw_psv))) {
                    throw RUNTIME_EXCEPT("cannot access a shell view.") ;
                }
                auto sv_deleter = [](IShellView* ptr) {ptr->Release() ;} ;
                std::unique_ptr<IShellView, decltype(sv_deleter)> psv(raw_psv, sv_deleter) ;

                //get IFolerView Interface
                IFolderView* raw_pfv = nullptr ;
                if(util::is_failed(psv->QueryInterface(IID_IFolderView, reinterpret_cast<void**>(&raw_pfv)))) {
                    throw RUNTIME_EXCEPT("cannot access a foler view.") ;
                }
                auto fv_deleter = [](IFolderView* ptr) {ptr->Release() ;} ;
                std::unique_ptr<IFolderView, decltype(fv_deleter)> pfv(raw_pfv, fv_deleter) ;

                //get IPersistantFolder2 in order to use GetCurFolder method
                IPersistFolder2* raw_ppf2 = nullptr ;
                if(util::is_failed(pfv->GetFolder(IID_IPersistFolder2, reinterpret_cast<void**>(&raw_ppf2)))) {
                    throw RUNTIME_EXCEPT("cannot access a persist folder 2.") ;
                }
                auto pf2_deleter = [](IPersistFolder2* ptr) {ptr->Release() ;} ;
                std::unique_ptr<IPersistFolder2, decltype(pf2_deleter)> ppf2(raw_ppf2, pf2_deleter) ;

                ITEMIDLIST* raw_pidl = nullptr ;
                if(util::is_failed(ppf2->GetCurFolder(&raw_pidl))) {
                    throw RUNTIME_EXCEPT("cannot get current folder.") ;
                }
                auto idl_deleter = [](ITEMIDLIST* ptr) {CoTaskMemFree(ptr) ;} ;
                std::unique_ptr<ITEMIDLIST, decltype(idl_deleter)> pidl(raw_pidl, idl_deleter) ;

                //convert to path
                WCHAR path[MAX_PATH] = {0} ;
                if(!SHGetPathFromIDListW(pidl.get(), path)) {
                    throw RUNTIME_EXCEPT("cannot convert an item ID to a file system path.") ;
                }

                return vind::util::ws_to_s(path) ;
            }

            return PathType() ;
        }
    }
}
