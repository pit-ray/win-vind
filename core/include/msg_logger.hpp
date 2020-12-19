#ifndef _MSG_LOGGER_HPP
#define _MSG_LOGGER_HPP

#include <string>
#include <iostream>
#include <fstream>

namespace Logger
{
    void initialize() ;
    void error(const char* msg, const char* scope) ;
    void error(std::string&& msg, const char* scope) ;
    void error(const std::string& msg, const char* scope) ;
}

//-----------------
#if defined(__GNUC__)
#define ERROR_PRINT(MSG) Logger::error((MSG), __PRETTY_FUNCTION__)

#elif defined(_MSC_VER) && _MSC_VER >= 1500
#define ERROR_PRINT(MSG) Logger::error((MSG), __FUNCSIG__)

#else
#define ERROR_PRINT(MSG) Logger::error((MSG))

#endif
//------------------

#endif
