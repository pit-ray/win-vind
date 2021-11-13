#ifndef _ERR_LOGGER_HPP
#define _ERR_LOGGER_HPP

#include <fstream>
#include <string>

#if defined(DEBUG)
#include <iostream>
#endif


namespace vind
{
    namespace core
    {
        void initialize_logger() ;
        void error(const char* msg, const char* scope) ;
        void error(std::string&& msg, const char* scope) ;
        void error(const std::string& msg, const char* scope) ;

        void message(const char* msg, const char* scope) ;
        void message(std::string&& msg, const char* scope) ;
        void message(const std::string& msg, const char* scope) ;
    }
}

//-----------------
#if defined(__GNUC__)
#define PRINT_ERROR(MSG) vind::core::error((MSG), __PRETTY_FUNCTION__)
#define PRINT_MSG(MSG) vind::core::message((MSG), __PRETTY_FUNCTION__)

#elif defined(_MSC_VER) && _MSC_VER >= 1500
#define PRINT_ERROR(MSG) vind::core::error((MSG), __FUNCSIG__)
#define PRINT_MSG(MSG) vind::core::message((MSG), __FUNCSIG__)

#else
#define PRINT_ERROR(MSG) vind::core::error(MSG)
#define PRINT_MSG(MSG) vind::core::message(MSG)

#endif
//------------------

#endif
