#ifndef _MSG_LOGGER_HPP
#define _MSG_LOGGER_HPP

#include <string>
#include <iostream>
#include <fstream>
#include <mutex>

#include <windows.h>

namespace Logger
{
    static constexpr auto efilename{"log/error.log"} ;
    static std::ofstream _init_error_stream{efilename, std::ios::trunc} ;
    static std::ofstream error_stream{efilename, std::ios::app} ;

    static constexpr auto mfilename{"log/message.log"} ;
    static std::ofstream _init_msg_stream{mfilename, std::ios::trunc} ;
    static std::ofstream msg_stream{mfilename, std::ios::app} ;
    static constexpr auto M{"[Message] "} ;

    void error(const char* msg, const char* scope) noexcept ;
    void error(std::string&& msg, const char* scope) noexcept ;
    void error(const std::string& msg, const char* scope) noexcept ;

    void win_error(const char* msg, const char* scope) noexcept ;
    void win_error(std::string&& msg, const char* scope) noexcept ;
    void win_error(const std::string& msg, const char* scope) noexcept ;
}

#define MESSAGE_STREAM (\
Logger::msg_stream << Logger::M\
)

#define ERROR_PRINT(MSG) Logger::error((MSG), __PRETTY_FUNCTION__)
#define WIN_ERROR_PRINT(MSG) Logger::win_error((MSG), __PRETTY_FUNCTION__)

#endif
