#ifndef _CHAR_LOGGER_HPP
#define _CHAR_LOGGER_HPP

#include "keycode.hpp"
#include "keylgrbase.hpp"
#include <initializer_list>

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

            explicit CharLogger(const KeyLog::Data& enabled_non_chars) ;
            explicit CharLogger(KeyLog::Data&& enabled_non_chars) ;

            virtual ~CharLogger() noexcept ;

            CharLogger(const CharLogger&) ;
            CharLogger& operator=(const CharLogger&) ;

            CharLogger(CharLogger&&) ;
            CharLogger& operator=(CharLogger&&) ;

            virtual void clear() noexcept override ;

            virtual void remove_from_back(std::size_t num_from_back) override ;
            virtual void remove_from_top(std::size_t num_from_top) override ;

            void enable_non_character(KeyCode keycode) ;
            void disable_non_character(KeyCode keycode) ;

            virtual void sync_state_with(const CharLogger& rhs) ;

            virtual int logging_state(const KeyLog& log) override ;
            std::string to_str() const noexcept ;
        } ;
    }
}

#endif
