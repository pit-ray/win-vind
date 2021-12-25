#ifndef _KEYCODE_HPP
#define _KEYCODE_HPP

#include <ostream>
#include <string>
#include <vector>

#ifdef DEBUG
#include "err_logger.hpp"
#endif

#define PREFER_ASCII_CODE true
#define PREFER_SYSTEM_CODE false


namespace vind
{
    namespace core
    {
        class KeyCode {
        private:
            unsigned short code_ ;

            KeyCode(unsigned short code_wflag) ;

        public:
            KeyCode() ;
            KeyCode(char ascii) ;
            KeyCode(unsigned char keycode) ;
            KeyCode(int keycode) ;

            /**
             * @param[in] (prefer_ascii) 
             *   If there is a conflict between the name of the 
             *   system keycode and the ascii, the bool value 
             *   determines which takes precedence.
             *
             *   For example, <alt> can also be written as <a>.
             *
             *   KeyCode("a", true)  => KEYCODE_A
             *   KeyCode("a", false) => KEYCODE_ALT
             *
             */
            KeyCode(
                    const std::string& name,
                    bool prefer_ascii=PREFER_ASCII_CODE) ;

            char to_ascii() const noexcept ;
            char to_shifted_ascii() const noexcept ;
            int to_number() const noexcept ;
            unsigned char to_code() const noexcept ;

            KeyCode to_representative() const noexcept ;
            KeyCode to_physical() const noexcept ;

            bool is_ascii() const noexcept ;
            bool is_unreal() const noexcept ;
            bool is_number() const noexcept ;
            bool is_toggle() const noexcept ;

            bool empty() const noexcept ;

            const std::string& name() const noexcept ;

            unsigned short get() const noexcept ;

            operator bool() const noexcept ;
            operator char() const noexcept ;
            operator unsigned char() const noexcept ;
            operator int() const noexcept ;
            operator std::string() const noexcept ;
            operator const char*() const noexcept ;

            bool operator!() const noexcept ;

            bool operator<(const KeyCode& rhs) const noexcept ;
            bool operator>(const KeyCode& rhs) const noexcept ;
            bool operator<=(const KeyCode& rhs) const noexcept ;
            bool operator>=(const KeyCode& rhs) const noexcept ;

            bool operator==(const KeyCode& rhs) const noexcept ;
            bool operator==(KeyCode&& rhs) const noexcept ;
            bool operator==(char rhs) const noexcept ;
            bool operator==(unsigned char rhs) const noexcept ;
            bool operator==(const std::string& rhs) const noexcept ;
            bool operator==(const char* rhs) const noexcept ;

            bool operator!=(const KeyCode& rhs) const noexcept ;
            bool operator!=(KeyCode&& rhs) const noexcept ;
            bool operator!=(char rhs) const noexcept ;
            bool operator!=(unsigned char rhs) const noexcept ;
            bool operator!=(const std::string& rhs) const noexcept ;
            bool operator!=(const char* rhs) const noexcept ;

            static bool is_shifted(char ascii) noexcept ;
        } ;

        std::ostream& operator<<(std::ostream& stream, const KeyCode& rhs) ;

        using KeySet = std::vector<KeyCode> ;
        using Command = std::vector<KeySet> ;
        using CommandList = std::vector<Command> ;

        std::ostream& operator<<(std::ostream& stream, const KeySet& rhs) ;
        std::ostream& operator<<(std::ostream& stream, const Command& rhs) ;
        std::ostream& operator<<(std::ostream& stream, const CommandList& rhs) ;
    }
}


namespace std {
    template <>
    struct hash<vind::core::KeyCode> {
        size_t operator()(const vind::core::KeyCode& rhs) const {
            return static_cast<size_t>(rhs.get()) ;
        }
    } ;
}

#endif
