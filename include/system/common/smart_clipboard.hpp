#ifndef _SMART_CLIPBOARD_HPP
#define _SMART_CLIPBOARD_HPP

#include <windows.h>

#include <memory>
#include <string>

#include "utility.hpp"

class SmartClipboard
{
private:
    struct Impl ;
    std::unique_ptr<Impl> pimpl ;

public:
    explicit SmartClipboard(HWND hwnd) ;

    virtual ~SmartClipboard() noexcept ;

    bool open() noexcept ;
    bool close() noexcept ;
    bool get_as_str(std::string& str, bool& having_EOL) noexcept ;

    //backup current clipboard to cache
    bool backup() noexcept ;

    //restore cache to clipboard
    bool restore_backup() noexcept ;

    bool set(const char* const ar, const std::size_t size) noexcept ;

    template <typename T>
    bool set(const T& arref) noexcept {
        return set(arref, Utility::sizeof_array(arref)) ;
    }

    SmartClipboard(SmartClipboard&&) = delete ;
    SmartClipboard& operator=(SmartClipboard&&) = delete ;
    SmartClipboard(const SmartClipboard&) = delete ;
    SmartClipboard& operator=(const SmartClipboard&) = delete ;
} ;
#endif
