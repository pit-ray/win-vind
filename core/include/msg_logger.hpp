#ifndef _MSG_LOGGER_HPP
#define _MSG_LOGGER_HPP

#include <string>
#include <iostream>
#include <fstream>

namespace vind
{
    namespace log
    {
        void initialize() ;
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
#define ERROR_PRINT(MSG) vind::log::error((MSG), __PRETTY_FUNCTION__)
#define MSG_PRINT(MSG) vind::log::message((MSG), __PRETTY_FUNCTION__)

#elif defined(_MSC_VER) && _MSC_VER >= 1500
#define ERROR_PRINT(MSG) vind::log::error((MSG), __FUNCSIG__)
#define MSG_PRINT(MSG) vind::log::message((MSG), __FUNCSIG__)

#else
#define ERROR_PRINT(MSG) vind::log::error(MSG)
#define MSG_PRINT(MSG) vind::log::message(MSG)

#endif
//------------------

#endif
