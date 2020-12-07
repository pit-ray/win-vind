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

    void open() ;
    void close() ;
    void get_as_str(std::string& str, bool& having_EOL) ;

    //backup current clipboard to cache
    void backup() ;

    //restore cache to clipboard
    void restore_backup() ;

    void set(const char* const ar, const std::size_t size) ;

    template <typename T>
    void set(const T& arref) {
        return set(arref, Utility::sizeof_array(arref)) ;
    }

    SmartClipboard(SmartClipboard&&)                    = delete ;
    SmartClipboard& operator=(SmartClipboard&&)         = delete ;
    SmartClipboard(const SmartClipboard&)               = delete ;
    SmartClipboard& operator=(const SmartClipboard&)    = delete ;
} ;
#endif
