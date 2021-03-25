#ifndef _CHAR_LOGGER_HPP
#define _CHAR_LOGGER_HPP

#include "key_logger_base.hpp"

namespace vind
{
    class CharLogger : public KeyLoggerBase {
    private:
        struct Impl ;
        std::unique_ptr<Impl> pimpl ;

    public:
        explicit CharLogger() ;
        virtual ~CharLogger() noexcept ;

        CharLogger(const CharLogger&) ;
        CharLogger& operator=(const CharLogger&) ;

        CharLogger(CharLogger&&) ;
        CharLogger& operator=(CharLogger&&) ;

        virtual void update() override ;
        virtual bool is_changed() const noexcept override ;

        virtual const std::string to_str() const ;
    } ;
}

#endif
