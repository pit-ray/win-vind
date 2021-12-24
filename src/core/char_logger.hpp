#ifndef _CHAR_LOGGER_HPP
#define _CHAR_LOGGER_HPP

#include "key_logger_base.hpp"
#include "keycode.hpp"
#include <initializer_list>
#include <set>

#define CHAR_EMPTY(result)  ((result) == 0)
#define CHAR_LOGGED(result) ((result) > 0)

namespace vind
{
    namespace core
    {
        class CharLogger : public KeyLoggerBase {
        private:
            struct Impl ;
            std::unique_ptr<Impl> pimpl ;

        public:
            explicit CharLogger() ;

            explicit CharLogger(const std::initializer_list<KeyCode>& enabled_non_chars) ;
            explicit CharLogger(std::initializer_list<KeyCode>&& enabled_non_chars) ;

            explicit CharLogger(const std::set<KeyCode>& enabled_non_chars) ;
            explicit CharLogger(std::set<KeyCode>&& enabled_non_chars) ;

            virtual ~CharLogger() noexcept ;

            CharLogger(const CharLogger&) ;
            CharLogger& operator=(const CharLogger&) ;

            CharLogger(CharLogger&&) ;
            CharLogger& operator=(CharLogger&&) ;

            virtual void logging_without_multi_ascii(const KeyLog& log) ;

            void enable_non_character(KeyCode keycode) ;
            void disable_non_character(KeyCode keycode) ;

            virtual void sync_state_with(const CharLogger& rhs) ;

            virtual int logging_state(KeyLog log) override ;
            virtual std::string to_str() const ;
        } ;
    }
}

#endif
