#include "explorer_util.hpp"

#include <filesystem>
#include <memory>

#include <exdisp.h>
#include <shlobj.h>

#include "core/errlogger.hpp"
#include "util/def.hpp"
#include "util/smartcom.hpp"
#include "util/string.hpp"
#include "util/winwrap.hpp"


namespace vind
{
    namespace bind {
        //This is based on https://devblogs.microsoft.com/oldnewthing/?p=38393 .
        std::filesystem::path get_current_explorer_path() {
            auto hwnd = util::get_foreground_window() ;
            if(!hwnd) {
                throw std::runtime_error("There is no foreground window.") ;
            }

            if(util::is_failed(CoInitialize(NULL))) {
                throw RUNTIME_EXCEPT("initialization failed") ;
            }

            auto co_uninit = [] (char* ptr) {
                if(ptr) {
                    delete ptr ;
                    ptr = nullptr ;
                }
                CoUninitialize() ;
            } ;
            std::unique_ptr<char, decltype(co_uninit)> smart_uninit{new char(), co_uninit} ;

            //we can get explorer handle from IShellWindows.
            util::SmartCom<IShellWindows> psw{} ;
            if(util::is_failed(CoCreateInstance(
                CLSID_ShellWindows,
                NULL,
                CLSCTX_ALL,
                IID_IShellWindows,
                reinterpret_cast<void**>(&psw)
            ))) {
                throw RUNTIME_EXCEPT("cannot create IShellWindows.") ;
            }

            long win_num = 0 ;
            if(util::is_failed(psw->get_Count(&win_num))) {
                throw RUNTIME_EXCEPT("No explorer is opened.") ;
            }

            for(long i = 0 ; i < win_num ; i ++) {
                VARIANT v ;
                v.vt = VT_I4 ;
                V_I4(&v) = i ;

                //IDispatch is an interface to object, method or property
                util::SmartCom<IDispatch> pdisp{} ;
                if(util::is_failed(psw->Item(v, &pdisp))) {
                    continue ;
                }

                //Is this shell foreground window??
                util::SmartCom<IWebBrowserApp> pwba{} ;
                if(util::is_failed(pdisp->QueryInterface(IID_IWebBrowserApp, reinterpret_cast<void**>(&pwba)))) {
                    continue ;
                }

                HWND shell_hwnd = NULL ;
                if(util::is_failed(pwba->get_HWND(reinterpret_cast<LONG_PTR*>(&shell_hwnd)))) {
                    continue ;
                }
                if(shell_hwnd != hwnd) {
                    continue ; //it is not foreground window
                }

                //access to shell window
                util::SmartCom<IServiceProvider> psp{} ;
                if(util::is_failed(pwba->QueryInterface(IID_IServiceProvider, reinterpret_cast<void**>(&psp)))) {
                    throw RUNTIME_EXCEPT("cannot access a top service provider.") ;
                }

                //access to shell browser
                util::SmartCom<IShellBrowser> psb{} ;
                if(util::is_failed(psp->QueryService(SID_STopLevelBrowser, IID_IShellBrowser, reinterpret_cast<void**>(&psb)))) {
                    throw RUNTIME_EXCEPT("cannot access a shell browser.") ;
                }

                //access to shell view
                util::SmartCom<IShellView> psv{} ;
                if(util::is_failed(psb->QueryActiveShellView(&psv))) {
                    throw RUNTIME_EXCEPT("cannot access a shell view.") ;
                }

                //get IFolerView Interface
                util::SmartCom<IFolderView> pfv{} ;
                if(util::is_failed(psv->QueryInterface(IID_IFolderView, reinterpret_cast<void**>(&pfv)))) {
                    throw RUNTIME_EXCEPT("cannot access a foler view.") ;
                }

                //get IPersistantFolder2 in order to use GetCurFolder method
                util::SmartCom<IPersistFolder2> ppf2{} ;
                if(util::is_failed(pfv->GetFolder(IID_IPersistFolder2, reinterpret_cast<void**>(&ppf2)))) {
                    throw RUNTIME_EXCEPT("cannot access a persist folder 2.") ;
                }

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

                return std::filesystem::path(path) ;
            }

            return std::filesystem::path() ;
        }
    }
}
