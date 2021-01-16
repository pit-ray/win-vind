#include "smart_clipboard.hpp"

#include <cstring>

#include "msg_logger.hpp"

//std::string is 8bit-based object, so CF_UNICODETEXT (UTF-16) does not appropriate.
//Therefore, only used CF_OEMTEXT in CF_TEXT.
#define COMMON_FORMAT CF_OEMTEXT

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
        ERROR_PRINT(e.what()) ;
    }
}

void SmartClipboard::open() {
    if(pimpl->opening) {
        return ;
    }
    if(!OpenClipboard(pimpl->hwnd)) {
        throw RUNTIME_EXCEPT("failed opening clipboard") ;
        return ;
    }
    pimpl->opening = true ;
}

void SmartClipboard::close() {
    if(!pimpl->opening) {
        return ;
    }
    if(!CloseClipboard()) {
        throw RUNTIME_EXCEPT("failed closing clipboard") ;
        return ;
    }
    pimpl->opening = false ;
}

void SmartClipboard::get_as_str(std::string& str, bool& having_EOL) {
    if(!pimpl->opening) {
        throw LOGIC_EXCEPT("Thread does not have a clipboard open.") ;
        return ;
    }

    if(!IsClipboardFormatAvailable(COMMON_FORMAT)) {
        throw RUNTIME_EXCEPT("the current clipboard data is not supported unicode") ;
        return ;
    }

    auto data = GetClipboardData(COMMON_FORMAT) ;
    if(data == NULL) {
        throw RUNTIME_EXCEPT("cannot get clipboard data") ;
        return ;
    }

    auto unlocker = [](void* ptr) {GlobalUnlock(ptr) ;} ;
    std::unique_ptr<void, decltype(unlocker)> locked_data(GlobalLock(data), unlocker) ;
    if(locked_data == NULL) {
        throw RUNTIME_EXCEPT("cannot lock global clipboard data ") ;
        return ;
    }

    auto data_size = GlobalSize(locked_data.get()) ;
    if(!data_size) {
        throw RUNTIME_EXCEPT("the clipboard does not have data.") ; 
        return ;
    }
    auto rawstr = reinterpret_cast<char*>(locked_data.get()) ;
    str = rawstr ;

    if(data_size < 2) {
        having_EOL = false ;
    }
    else {
        //if includes EOL, the four back words is 0x0000, else 0x??00.
        having_EOL = rawstr[data_size - 1] == '\0' &&\
                     rawstr[data_size - 2] == '\0' ;
    }
}

//backup current clipboard to cache
void SmartClipboard::backup() {
    if(!pimpl->opening) {
        throw LOGIC_EXCEPT("Thread does not have a clipboard open.") ;
        return ;
    }

    if(pimpl->cache != NULL) {
        if(!GlobalFree(pimpl->cache)) {
            throw RUNTIME_EXCEPT("Cannot free cache for backup") ;
            return ;
        }
    }

    auto data = GetClipboardData(COMMON_FORMAT) ;
    if(data == NULL) {
        throw RUNTIME_EXCEPT("cannot get clipboard data") ;
        return ;
    }

    auto unlocker = [](void* ptr) {GlobalUnlock(ptr) ;} ;
    std::unique_ptr<void, decltype(unlocker)> locked_data(GlobalLock(data), unlocker) ;
    if(locked_data == NULL) {
        throw RUNTIME_EXCEPT("cannot lock global clipboard data") ;
        return ;
    }

    auto data_size = GlobalSize(locked_data.get()) ;
    if(!data_size) {
        throw RUNTIME_EXCEPT("the clipboard's data is not valid.") ; 
        return ;
    }

    auto gmem = GlobalAlloc(GHND, data_size) ;
    if(gmem == NULL) {
        throw RUNTIME_EXCEPT("cannot alloc memories in global area.") ;
        return ;
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
        return ;
    }

    if(pimpl->cache == NULL) {
        throw LOGIC_EXCEPT("cache does not have backup") ;
        return ;
    }

    if(!EmptyClipboard()) {
        throw RUNTIME_EXCEPT("Failed initalization of clipboard") ;
        return ;
    }

    //If SetClipboardData succeeds, the system owns gmem, so should not free gmem.
    if(!SetClipboardData(COMMON_FORMAT, pimpl->cache)) {
        throw RUNTIME_EXCEPT("cannot set data into clipboard") ;
        return ;
    }
    pimpl->cache = NULL ;
}

void SmartClipboard::set(const char* const ar, const std::size_t size) {
    if(!pimpl->opening) {
        throw LOGIC_EXCEPT("Thread does not have a clipboard open.") ;
        return ;
    }

    auto gmem = GlobalAlloc(GHND, size) ;
    if(gmem == NULL) {
        throw RUNTIME_EXCEPT("cannot alloc memories in global area.") ;
        return ;
    }

    auto unlocker = [](void* ptr) {GlobalUnlock(ptr) ;} ;
    std::unique_ptr<void, decltype(unlocker)> locked_gmem(GlobalLock(gmem), unlocker) ;
    if(locked_gmem == NULL) {
        throw RUNTIME_EXCEPT("cannot lock global memory for backup.") ;
        GlobalFree(gmem) ;
        return ;
    }

    if(!EmptyClipboard()) {
        throw RUNTIME_EXCEPT("Failed initalization of clipboard") ;
        return ;
    }

    std::memcpy(locked_gmem.get(), ar, size) ;
    locked_gmem.reset() ; //unlock

    //If SetClipboardData succeeds, the system owns gmem, so should not free gmem.
    if(!SetClipboardData(COMMON_FORMAT, gmem)) {
        throw RUNTIME_EXCEPT("cannot set data into clipboard") ;
        return ;
    }
}
