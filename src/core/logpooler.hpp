#ifndef _LOGPOOLER_HPP
#define _LOGPOOLER_HPP

#include <memory>


namespace vind
{
    namespace core
    {
        class KeyLog ;

        class LogPooler {
        private:
            struct Impl ;
            std::unique_ptr<Impl> pimpl ;

            explicit LogPooler() ;
            virtual ~LogPooler() noexcept ;

        public:
            static LogPooler& get_instance() ;

            void push_log(const KeyLog& log) ;
            void push_log(KeyLog&& log) ;

            KeyLog pop_log() ;

            LogPooler(LogPooler&&)                 = delete ;
            LogPooler& operator=(LogPooler&&)      = delete ;
            LogPooler(const LogPooler&)            = delete ;
            LogPooler& operator=(const LogPooler&) = delete ;
        } ;

    }
}

#endif
