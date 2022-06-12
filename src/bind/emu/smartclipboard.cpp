#include "smartclipboard.hpp"

#include <cstring>

#include "core/errlogger.hpp"

#include "util/debug.hpp"
#include "util/def.hpp"
#include "util/string.hpp"

//std::string is 8bit-based object, so CF_UNICODETEXT (UTF-16) does not appropriate.
//Therefore, only used CF_OEMTEXT in CF_TEXT.
#define COMMON_FORMAT CF_UNICODETEXT

namespace vind
{
    namespace bind
    {
        struct SmartClipboard::Impl
        {
            bool opening ;
            HWND hwnd ;
            HGLOBAL cache ;

            explicit Impl(HWND window_handle)
            : opening(false),
              hwnd(window_handle),
              cache(NULL)
            {}
        } ;

        SmartClipboard::SmartClipboard(HWND hwnd)
        : pimpl(std::make_unique<Impl>(hwnd))
        {}

        SmartClipboard::~SmartClipboard() noexcept {
            try {
                close() ;
                //Unused handles should be released.
                if(pimpl->cache != NULL) {
                    GlobalFree(pimpl->cache) ;
                    pimpl->cache = NULL ;
                }
            }
            catch(const std::exception& e) {
                PRINT_ERROR(e.what()) ;
            }
        }

        void SmartClipboard::open() {
            if(pimpl->opening) {
                return ;
            }
            if(!OpenClipboard(pimpl->hwnd)) {
                throw RUNTIME_EXCEPT("failed opening clipboard") ;
            }
            pimpl->opening = true ;
        }

        void SmartClipboard::close() {
            if(!pimpl->opening) {
                return ;
            }
            if(!CloseClipboard()) {
                throw RUNTIME_EXCEPT("failed closing clipboard") ;
            }
            pimpl->opening = false ;
        }

        bool SmartClipboard::get_as_str(std::string& str) {
            if(!pimpl->opening) {
                throw LOGIC_EXCEPT("Thread does not have a clipboard open.") ;
            }

            if(!IsClipboardFormatAvailable(COMMON_FORMAT)) {
                throw RUNTIME_EXCEPT("the current clipboard data is not supported unicode") ;
            }

            auto data = GetClipboardData(COMMON_FORMAT) ;
            if(data == NULL) {
                throw RUNTIME_EXCEPT("cannot get clipboard data") ;
            }

            auto unlocker = [](void* ptr) {GlobalUnlock(ptr) ;} ;
            std::unique_ptr<void, decltype(unlocker)> locked_data(GlobalLock(data), unlocker) ;
            if(locked_data == NULL) {
                throw RUNTIME_EXCEPT("cannot lock global clipboard data ") ;
            }

            auto data_size = GlobalSize(locked_data.get()) ;
            if(!data_size) {
                throw RUNTIME_EXCEPT("the clipboard does not have data.") ; 
            }
            std::wstring wstr(reinterpret_cast<wchar_t*>(locked_data.get())) ;

            str = util::ws_to_s(wstr) ;

            if(wstr.empty()) {
                return false ;
            }

            // If there is a visible line break mark in an application such
            // as MS Word, there are two margins, including the null character.
            return data_size == (sizeof(wchar_t) * (wstr.length() + 2)) ;
        }

        //backup current clipboard to cache
        void SmartClipboard::backup() {
            if(!pimpl->opening) {
                throw LOGIC_EXCEPT("Thread does not have a clipboard open.") ;
            }

            if(pimpl->cache != NULL) {
                if(!GlobalFree(pimpl->cache)) {
                    throw RUNTIME_EXCEPT("Cannot free cache for backup") ;
                }
            }

            auto data = GetClipboardData(COMMON_FORMAT) ;
            if(data == NULL) {
                throw RUNTIME_EXCEPT("cannot get clipboard data") ;
            }

            auto unlocker = [](void* ptr) {GlobalUnlock(ptr) ;} ;
            std::unique_ptr<void, decltype(unlocker)> locked_data(GlobalLock(data), unlocker) ;
            if(locked_data == NULL) {
                throw RUNTIME_EXCEPT("cannot lock global clipboard data") ;
            }

            auto data_size = GlobalSize(locked_data.get()) ;
            if(!data_size) {
                throw RUNTIME_EXCEPT("the clipboard's data is not valid.") ; 
                return ;
            }

            auto gmem = GlobalAlloc(GHND, data_size) ;
            if(gmem == NULL) {
                throw RUNTIME_EXCEPT("cannot alloc memories in global area.") ;
            }

            std::unique_ptr<void, decltype(unlocker)> locked_gmem(GlobalLock(gmem), unlocker) ;
            if(locked_gmem == NULL) {
                GlobalFree(gmem) ;
                throw RUNTIME_EXCEPT("cannot lock global memory for backup.") ;
            }

            std::memcpy(locked_gmem.get(), locked_data.get(), data_size) ;
            pimpl->cache = gmem ;
        }

        //restore cache to clipboard
        void SmartClipboard::restore_backup() {
            if(!pimpl->opening) {
                throw LOGIC_EXCEPT("Thread does not have a clipboard open.") ;
            }

            if(pimpl->cache == NULL) {
                throw LOGIC_EXCEPT("cache does not have backup") ;
            }

            if(!EmptyClipboard()) {
                throw RUNTIME_EXCEPT("Failed initalization of clipboard") ;
            }

            //If SetClipboardData succeeds, the system owns gmem, so should not free gmem.
            if(!SetClipboardData(COMMON_FORMAT, pimpl->cache)) {
                throw RUNTIME_EXCEPT("cannot set data into clipboard") ;
            }
            pimpl->cache = NULL ;
        }

        void SmartClipboard::set(const std::string& str) {
            if(!pimpl->opening) {
                throw LOGIC_EXCEPT("Thread does not have a clipboard open.") ;
            }

            auto wstr = util::s_to_ws(str) ;
            auto size = sizeof(wchar_t) * (wstr.size() + 1) ;

            auto gmem = GlobalAlloc(GHND, size) ;
            if(gmem == NULL) {
                throw RUNTIME_EXCEPT("cannot alloc memories in global area.") ;
            }

            auto unlocker = [](void* ptr) {GlobalUnlock(ptr) ;} ;
            std::unique_ptr<void, decltype(unlocker)> locked_gmem(GlobalLock(gmem), unlocker) ;
            if(locked_gmem == NULL) {
                GlobalFree(gmem) ;
                throw RUNTIME_EXCEPT("cannot lock global memory for backup.") ;
            }

            if(!EmptyClipboard()) {
                throw RUNTIME_EXCEPT("Failed initalization of clipboard") ;
            }

            std::memcpy(locked_gmem.get(), wstr.c_str(), size) ;
            locked_gmem.reset() ; //unlock

            //If SetClipboardData succeeds, the system owns gmem, so should not free gmem.
            if(!SetClipboardData(COMMON_FORMAT, gmem)) {
                throw RUNTIME_EXCEPT("cannot set data into clipboard") ;
            }
        }
    }
}
