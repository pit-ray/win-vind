#ifndef _VKC_LOGGER_HPP
#define _VKC_LOGGER_HPP

#include "key_logger_base.hpp"

namespace vind
{
    class VKCLogger : public KeyLoggerBase {
    private:
        struct Impl ;
        std::unique_ptr<Impl> pimpl ;

    public:
        explicit VKCLogger() ;
        virtual ~VKCLogger() noexcept ;

        VKCLogger(const VKCLogger&) ;
        VKCLogger& operator=(const VKCLogger&) ;

        VKCLogger(VKCLogger&&) ;
        VKCLogger& operator=(VKCLogger&&) ;

        virtual void update() override ;
        virtual bool is_changed() const noexcept override ;
    } ;
}

#endif
