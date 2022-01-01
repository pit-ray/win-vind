#ifndef _SMART_CLIPBOARD_HPP
#define _SMART_CLIPBOARD_HPP

#include <windows.h>

#include <memory>
#include <string>

#include "util/container.hpp"

namespace vind
{
    namespace bind
    {
        class SmartClipboard {
        private:
            struct Impl ;
            std::unique_ptr<Impl> pimpl ;

        public:
            explicit SmartClipboard(HWND hwnd) ;

            virtual ~SmartClipboard() noexcept ;

            void open() ;
            void close() ;

            // If there is a visible EOL mark, then True is returned.
            bool get_as_str(std::string& str) ;

            //backup current clipboard to cache
            void backup() ;

            //restore cache to clipboard
            void restore_backup() ;

            void set(const std::string& str) ;

            SmartClipboard(SmartClipboard&&)                    = delete ;
            SmartClipboard& operator=(SmartClipboard&&)         = delete ;
            SmartClipboard(const SmartClipboard&)               = delete ;
            SmartClipboard& operator=(const SmartClipboard&)    = delete ;
        } ;
    }
}

#endif
