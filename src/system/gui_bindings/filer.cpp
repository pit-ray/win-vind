#include "filer.hpp"

#include <memory>

#include <windows.h>
#include <exdisp.h>
#include <shlobj.h>

#include "keybrd_eventer.hpp"
#include "msg_logger.hpp"
#include "path.hpp"
#include "mode_manager.hpp"
#include "utility.hpp"
#include "change_mode.hpp"

using namespace std ;

//SaveOpenedFile
const string SaveOpenedFile::sname() noexcept
{
    return "save_opened_file" ;
}

bool SaveOpenedFile::sprocess(const string UNUSED(cmd))
{
    auto hwnd = GetForegroundWindow() ;
    if(!KeybrdEventer::pushup(VKC_LCTRL, VKC_S)) {
        return false ;
    }
    Sleep(500) ;
    if(hwnd != GetForegroundWindow()) { //opened popup
        return Change2Normal::sprocess(true) ;
    }
    return true ; //over write
}


//CloseOpenedFile
const string CloseOpenedFile::sname() noexcept
{
    return "close_opened_file" ;
}

bool CloseOpenedFile::sprocess(const string UNUSED(cmd))
{
    if(!Change2Normal::sprocess(true)) return false ; //cursor is avaiable
    return KeybrdEventer::pushup(VKC_LCTRL, VKC_F4) ; //close a browser's tab (the file is HTML).
}


//OpenOtherFile
const string OpenOtherFile::sname() noexcept
{
    return "open_other_file" ;
}

bool OpenOtherFile::sprocess(const string UNUSED(cmd))
{
    if(!Change2Normal::sprocess(true)) return false ; //cursor is avaiable
    return KeybrdEventer::pushup(VKC_LCTRL, VKC_O) ;
}


//MakeDir
const string MakeDir::sname() noexcept
{
    return "make_dir" ;
}


namespace FilerUtility
{
    //This algorithm is based on https://devblogs.microsoft.com/oldnewthing/?p=38393 .
    inline static auto get_current_explorer_path() noexcept {

        using path_t = std::basic_string<TCHAR> ;

        const auto hwnd = GetForegroundWindow() ;

        if(hwnd == NULL) {
            return path_t{} ;
        }

        if(FAILED(CoInitialize(NULL))) {
            WIN_ERROR_STREAM << "initialization is failed (MakeDir::CoInitialize)\n" ;
            return path_t() ;
        }

        //we can get explorer handle from IShellWindows.
        IShellWindows* raw_psw = nullptr ;
        if(FAILED(CoCreateInstance(
            CLSID_ShellWindows,
            NULL,
            CLSCTX_ALL,
            IID_IShellWindows,
            reinterpret_cast<void**>(&raw_psw)
        ))) {
            WIN_ERROR_STREAM << " cannot create IShellWindows. (MakeDir::CoCreateInstance)\n" ;
            return path_t() ;
        }
        auto sw_deleter = [](IShellWindows* ptr) {ptr->Release() ;} ;
        std::unique_ptr<IShellWindows, decltype(sw_deleter)> psw(raw_psw, sw_deleter) ;

        long win_num = 0 ;
        if(FAILED(psw->get_Count(&win_num))) {
            WIN_ERROR_STREAM << "No explorer is opened. (MakeDir::IShellWindows::get_Count)\n" ;
            return path_t() ;
        }

        for(long i = 0 ; i < win_num ; i ++) {
            VARIANT v ;
            v.vt = VT_I4 ;
            V_I4(&v) = i ;

            //IDispatch is an interface to object, method or property
            IDispatch* raw_pdisp = nullptr ;
            if(FAILED(psw->Item(v, &raw_pdisp))) {
                continue ;
            }
            auto disp_deleter = [](IDispatch* ptr) {ptr->Release() ;} ;
            std::unique_ptr<IDispatch, decltype(disp_deleter)> pdisp(raw_pdisp, disp_deleter) ;

            //Is this shell foreground window??
            IWebBrowserApp* raw_pwba = nullptr ;
            if(FAILED(pdisp->QueryInterface(IID_IWebBrowserApp, reinterpret_cast<void**>(&raw_pwba)))) {
                continue ;
            }
            auto wba_deleter = [](IWebBrowserApp* ptr) {ptr->Release() ;} ;
            std::unique_ptr<IWebBrowserApp, decltype(wba_deleter)> pwba(raw_pwba, wba_deleter) ;
            HWND shell_hwnd = NULL ;
            if(FAILED(pwba->get_HWND(reinterpret_cast<LONG_PTR*>(&shell_hwnd)))) {
                continue ;
            }
            if(shell_hwnd != hwnd) {
                continue ; //it is not foreground window
            }

            //access to shell window
            IServiceProvider* raw_psp = nullptr ;
            if(FAILED(pwba->QueryInterface(IID_IServiceProvider, reinterpret_cast<void**>(&raw_psp)))) {
                WIN_ERROR_STREAM << "cannot access a top service provider. (MakeDir::IWebBrowserApp::QueryInterface\n)" ;
                return path_t() ;
            }
            auto sp_deleter = [](IServiceProvider* ptr) {ptr->Release() ;} ;
            std::unique_ptr<IServiceProvider, decltype(sp_deleter)> psp(raw_psp, sp_deleter) ;

            //access to shell browser
            IShellBrowser* raw_psb = nullptr ;
            if(FAILED(psp->QueryService(SID_STopLevelBrowser, IID_IShellBrowser, reinterpret_cast<void**>(&raw_psb)))) {
                WIN_ERROR_STREAM << "cannot access a shell browser. (MakeDir::IServiceProvider::QueryService\n)" ;
                return path_t() ;
            }
            auto sb_deleter = [](IShellBrowser* ptr) {ptr->Release() ;} ;
            std::unique_ptr<IShellBrowser, decltype(sb_deleter)> psb(raw_psb, sb_deleter) ;

            //access to shell view
            IShellView* raw_psv = nullptr ;
            if(FAILED(psb->QueryActiveShellView(&raw_psv))) {
                WIN_ERROR_STREAM << "cannot access a shell view. (MakeDir::IShellBrowser::QueryActiveShellView\n)" ;
                return path_t() ;
            }
            auto sv_deleter = [](IShellView* ptr) {ptr->Release() ;} ;
            std::unique_ptr<IShellView, decltype(sv_deleter)> psv(raw_psv, sv_deleter) ;

            //get IFolerView Interface
            IFolderView* raw_pfv = nullptr ;
            if(FAILED(psv->QueryInterface(IID_IFolderView, reinterpret_cast<void**>(&raw_pfv)))) {
                WIN_ERROR_STREAM << "cannot access a foler view. (MakeDir::IShellView::QueryInterface\n)" ;
                return path_t() ;
            }
            auto fv_deleter = [](IFolderView* ptr) {ptr->Release() ;} ;
            std::unique_ptr<IFolderView, decltype(fv_deleter)> pfv(raw_pfv, fv_deleter) ;

            //get IPersistantFolder2 in order to use GetCurFolder method
            IPersistFolder2* raw_ppf2 = nullptr ;
            if(FAILED(pfv->GetFolder(IID_IPersistFolder2, reinterpret_cast<void**>(&raw_ppf2)))) {
                WIN_ERROR_STREAM << "cannot access a persist folder 2. (MakeDir::IFolderView::GetFolder\n)" ;
                return path_t() ;
            }
            auto pf2_deleter = [](IPersistFolder2* ptr) {ptr->Release() ;} ;
            std::unique_ptr<IPersistFolder2, decltype(pf2_deleter)> ppf2(raw_ppf2, pf2_deleter) ;

            LPITEMIDLIST raw_pidl = nullptr ;
            if(FAILED(ppf2->GetCurFolder(&raw_pidl))) {
                WIN_ERROR_STREAM << "cannot get current folder. (MakeDir::IPersistFolder::GetCurFolder\n)" ;
                return path_t() ;
            }
            auto idl_deleter = [](ITEMIDLIST* ptr) {CoTaskMemFree(ptr) ;} ;
            std::unique_ptr<ITEMIDLIST, decltype(idl_deleter)> pidl(raw_pidl, idl_deleter) ;

            //convert to path
            TCHAR path[MAX_PATH] = {0} ;
            if(!SHGetPathFromIDList(pidl.get(), path)) {
                WIN_ERROR_STREAM << "cannot convert an item ID to a file system path. (MakeDir::SHGetPathFromIDList\n)" ;
                return path_t() ;
            }

            return path_t(path) ;
        }

        return path_t() ;
    }
}

bool MakeDir::sprocess(const string cmd)
{
    auto catch_error = [](auto&& path) {
        const auto ercode = GetLastError() ;
        ERROR_STREAM << "windows.h: " << ercode << ", " ;

        if(ercode == ERROR_ALREADY_EXISTS) {
            Logger::error_stream << "This directory is already existed." ;
        }
        else if(ercode == ERROR_PATH_NOT_FOUND) {
            Logger::error_stream << "This path is not found." ;
        }
        else {
            Logger::error_stream << "Cannot make directory." ;
        }
        Logger::error_stream << " (" << path << "). (MakeDir::CreateDirectory)\n" ;
    } ;

    const auto pos = cmd.find_first_of(" ") ;
    auto arg = cmd.substr(pos + 1) ;

    if(arg.find("\\") != std::string::npos || arg.find("/") != std::string::npos) {
        //argument is directory path
        if(arg.length() > 248) {
            //over max path num
            arg = arg.substr(0, 248) ;
        }

        if(!CreateDirectoryA(arg.c_str(), NULL)) {
            catch_error(arg) ;
            return false ;
        }
    }

    //argument is directory name
    //get current directory
    auto current_path = FilerUtility::get_current_explorer_path() ;
    if(current_path.empty()) {
        current_path = Path::HOME_PATH() + "/Desktop" ;
    }

    auto full_path = current_path + "\\" + arg ;

    if(!CreateDirectory(full_path.c_str(), NULL)) {
        catch_error(full_path) ;
        return false ;
    }

    return true ;
}