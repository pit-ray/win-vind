#ifndef _KEYCODE_LOGGER_HPP
#define _KEYCODE_LOGGER_HPP

#include "key_logger_base.hpp"

namespace vind
{
    class KeycodeLogger : public KeyLoggerBase {
    private:
        struct Impl ;
        std::unique_ptr<Impl> pimpl ;

    public:
        explicit KeycodeLogger() ;
        virtual ~KeycodeLogger() noexcept ;

        KeycodeLogger(const KeycodeLogger&) ;
        KeycodeLogger& operator=(const KeycodeLogger&) ;

        KeycodeLogger(KeycodeLogger&&) ;
        KeycodeLogger& operator=(KeycodeLogger&&) ;

        virtual void update() override ;
        virtual bool is_changed() const noexcept override ;
    } ;
}

#endif
