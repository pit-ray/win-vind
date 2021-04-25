#ifndef _CHAR_LOGGER_HPP
#define _CHAR_LOGGER_HPP

#include "key_logger_base.hpp"
#include <initializer_list>
#include <set>

#define CHAR_EMPTY(result)  ((result) == 0)
#define CHAR_LOGGED(result) ((result) > 0)

namespace vind
{
    class CharLogger : public KeyLoggerBase {
    private:
        struct Impl ;
        std::unique_ptr<Impl> pimpl ;

    public:
        explicit CharLogger() ;

        explicit CharLogger(const std::initializer_list<unsigned char>& enabled_non_chars) ;
        explicit CharLogger(std::initializer_list<unsigned char>&& enabled_non_chars) ;

        explicit CharLogger(const std::set<unsigned char>& enabled_non_chars) ;
        explicit CharLogger(std::set<unsigned char>&& enabled_non_chars) ;

        virtual ~CharLogger() noexcept ;

        CharLogger(const CharLogger&) ;
        CharLogger& operator=(const CharLogger&) ;

        CharLogger(CharLogger&&) ;
        CharLogger& operator=(CharLogger&&) ;

        void enable_non_character(unsigned char keycode) ;
        void disable_non_character(unsigned char keycode) ;

        virtual int logging_state() override ;
        virtual std::string to_str() const ;
    } ;
}

#endif
