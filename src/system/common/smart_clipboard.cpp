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
    close() ;
    //Unused handles should be released.
    if(pimpl->cache != NULL) {
        GlobalFree(pimpl->cache) ;
        pimpl->cache = NULL ;
    }
}

bool SmartClipboard::open() noexcept {
    if(pimpl->opening) return true ;
    if(!OpenClipboard(pimpl->hwnd)) {
        WIN_ERROR_PRINT("failed opening clipboard") ;
        return false ;
    }
    pimpl->opening = true ;
    return true ;
}

bool SmartClipboard::close() noexcept {
    if(!pimpl->opening) return true ;
    if(!CloseClipboard()) {
        WIN_ERROR_PRINT("failed closing clipboard") ;
        return false ;
    }
    pimpl->opening = false ;
    return true ;
}

bool SmartClipboard::get_as_str(std::string& str, bool& having_EOL) noexcept {
    if(!pimpl->opening) {
        WIN_ERROR_PRINT("Thread does not have a clipboard open.") ;
        return false ;
    }

    if(!IsClipboardFormatAvailable(COMMON_FORMAT)) {
        WIN_ERROR_PRINT("the current clipboard data is not supported unicode") ;
        return false ;
    }

    auto data = GetClipboardData(COMMON_FORMAT) ;
    if(data == NULL) {
        WIN_ERROR_PRINT("cannot get clipboard data") ;
        return false ;
    }

    auto unlocker = [](void* ptr) {GlobalUnlock(ptr) ;} ;
    std::unique_ptr<void, decltype(unlocker)> locked_data(GlobalLock(data), unlocker) ;
    if(locked_data == NULL) {
        WIN_ERROR_PRINT("cannot lock global clipboard data ") ;
        return false ;
    }

    auto data_size = GlobalSize(locked_data.get()) ;
    if(!data_size) {
        WIN_ERROR_PRINT("the clipboard does not have data.") ; 
        return false ;
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
    return true ;
}

//backup current clipboard to cache
bool SmartClipboard::backup() noexcept {
    if(!pimpl->opening) {
        WIN_ERROR_PRINT("Thread does not have a clipboard open.") ;
        return false ;
    }

    if(pimpl->cache != NULL) {
        if(!GlobalFree(pimpl->cache)) {
            return false ;
        }
    }

    auto data = GetClipboardData(COMMON_FORMAT) ;
    if(data == NULL) {
        WIN_ERROR_PRINT("cannot get clipboard data") ;
        return false ;
    }

    auto unlocker = [](void* ptr) {GlobalUnlock(ptr) ;} ;
    std::unique_ptr<void, decltype(unlocker)> locked_data(GlobalLock(data), unlocker) ;
    if(locked_data == NULL) {
        WIN_ERROR_PRINT("cannot lock global clipboard data") ;
        return false ;
    }

    auto data_size = GlobalSize(locked_data.get()) ;
    if(!data_size) {
        WIN_ERROR_PRINT("the clipboard's data is not valid.") ; 
        return false ;
    }

    auto gmem = GlobalAlloc(GHND, data_size) ;
    if(gmem == NULL) {
        WIN_ERROR_PRINT("cannot alloc memories in global area.") ;
        return false ;
    }

    std::unique_ptr<void, decltype(unlocker)> locked_gmem(GlobalLock(gmem), unlocker) ;
    if(locked_gmem == NULL) {
        WIN_ERROR_PRINT("cannot lock global memory for backup.") ;
        GlobalFree(gmem) ;
        return false ;
    }

    std::memcpy(locked_gmem.get(), locked_data.get(), data_size) ;
    pimpl->cache = gmem ;

    return true ;
}

//restore cache to clipboard
bool SmartClipboard::restore_backup() noexcept {
    if(!pimpl->opening) {
        WIN_ERROR_PRINT("Thread does not have a clipboard open.") ;
        return false ;
    }

    if(pimpl->cache == NULL) {
        WIN_ERROR_PRINT("cache does not have backup") ;
        return false ;
    }

    if(!EmptyClipboard()) {
        WIN_ERROR_PRINT("Failed initalization of clipboard") ;
        return false ;
    }

    //If SetClipboardData succeeds, the system owns gmem, so should not free gmem.
    if(!SetClipboardData(COMMON_FORMAT, pimpl->cache)) {
        WIN_ERROR_PRINT("cannot set data into clipboard") ;
        return false ;
    }
    pimpl->cache = NULL ;
    return true ;
}

bool SmartClipboard::set(const char* const ar, const std::size_t size) noexcept {
    if(!pimpl->opening) {
        WIN_ERROR_PRINT("Thread does not have a clipboard open.") ;
        return false ;
    }

    auto gmem = GlobalAlloc(GHND, size) ;
    if(gmem == NULL) {
        WIN_ERROR_PRINT("cannot alloc memories in global area.") ;
        return false ;
    }

    auto unlocker = [](void* ptr) {GlobalUnlock(ptr) ;} ;
    std::unique_ptr<void, decltype(unlocker)> locked_gmem(GlobalLock(gmem), unlocker) ;
    if(locked_gmem == NULL) {
        WIN_ERROR_PRINT("cannot lock global memory for backup.") ;
        GlobalFree(gmem) ;
        return false ;
    }

    if(!EmptyClipboard()) {
        WIN_ERROR_PRINT("Failed initalization of clipboard") ;
        return false ;
    }

    std::memcpy(locked_gmem.get(), ar, size) ;
    locked_gmem.reset() ; //unlock

    //If SetClipboardData succeeds, the system owns gmem, so should not free gmem.
    if(!SetClipboardData(COMMON_FORMAT, gmem)) {
        WIN_ERROR_PRINT("cannot set data into clipboard") ;
        return false ;
    }
    return true ;
}
