#ifndef _KEYCODE_HPP
#define _KEYCODE_HPP

#include <string>


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
            KeyCode(const std::string& name) ;
            KeyCode(const char* name) ;

            template <typename T>
            KeyCode(T keycode)
            : KeyCode(static_cast<unsigned char>(keycode))
            {}

            char to_ascii() const noexcept ;
            int to_number() const noexcept ;

            KeyCode to_representative() const noexcept ;
            KeyCode to_physical() const noexcept ;

            bool is_ascii() const noexcept ;
            bool is_shifted() const noexcept ;
            bool is_unreal() const noexcept ;
            bool is_number() const noexcept ;
            bool is_toggle() const noexcept ;

            bool has_code() const noexcept ;

            const std::string& name() const noexcept ;

            unsigned char get() const noexcept ;

            operator bool() const noexcept ;
            operator char() const noexcept ;
            operator unsigned char() const noexcept ;
            operator int() const noexcept ;
            operator std::string() const noexcept ;
            operator const char*() const noexcept ;

            bool operator!() const noexcept ;
        } ;

        template <typename Stream>
        Stream& operator<<(Stream&& stream, const KeyCode& rhs) {
            stream << rhs.name() ;
            return stream ;
        }
    }
}

#endif
