#ifndef _ERR_LOGGER_HPP
#define _ERR_LOGGER_HPP

#include <memory>
#include <string>


namespace vind
{
    namespace core
    {
        class Logger {
        private:
            Logger(
                std::string&& filename_head,
                std::size_t keeping_log_num,
                std::size_t align_width_of_header) ;
            ~Logger() noexcept ;

            struct Impl ;
            std::unique_ptr<Impl> pimpl ;

        public:
            static Logger& get_instance() ;

            void init() ;

            void error(const char* msg, const char* scope="") ;
            void error(std::string&& msg, const char* scope="") ;
            void error(const std::string& msg, const char* scope="") ;

            void message(const char* msg, const char* scope="") ;
            void message(std::string&& msg, const char* scope="") ;
            void message(const std::string& msg, const char* scope="") ;

            Logger(Logger&&)                 = delete ;
            Logger& operator=(Logger&&)      = delete ;
            Logger(const Logger&)            = delete ;
            Logger& operator=(const Logger&) = delete ;
        } ;
    }
}

#ifndef PRINT_ERROR

#if defined(__GNUC__)
#define PRINT_ERROR(MSG) vind::core::Logger::get_instance().error((MSG), __PRETTY_FUNCTION__)

#elif defined(_MSC_VER) && _MSC_VER >= 1500
#define PRINT_ERROR(MSG) vind::core::Logger::get_instance().error((MSG), __FUNCSIG__)

#else
#define PRINT_ERROR(MSG) vind::core::Logger::get_instance().error(MSG)

#endif // __GNUC__

#endif // PRINT_ERROR



#ifndef PRINT_MSG

#if defined(__GNUC__)
#define PRINT_MSG(MSG) vind::core::Logger::get_instance().message((MSG), __PRETTY_FUNCTION__)

#elif defined(_MSC_VER) && _MSC_VER >= 1500
#define PRINT_MSG(MSG) vind::core::Logger::get_instance().message((MSG), __FUNCSIG__)

#else
#define PRINT_MSG(MSG) vind::core::Logger::get_instance().message(MSG)

#endif // __GNUC__

#endif // PRINT_MSG

#endif
