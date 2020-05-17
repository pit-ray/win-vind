#ifndef _MSG_LOGGER_HPP
#define _MSG_LOGGER_HPP

#include <string>
#include <iostream>
#include <fstream>

namespace Logger
{
    static constexpr auto efilename{"log/error.log"} ;
    static std::ofstream _init_error_stream{efilename, std::ios::trunc} ;
    static std::ofstream error_stream{efilename, std::ios::app} ;
    static constexpr auto E{"[Error] "} ;

    static constexpr auto mfilename{"log/message.log"} ;
    static std::ofstream _init_msg_stream{mfilename, std::ios::trunc} ;
    static std::ofstream msg_stream{mfilename, std::ios::app} ;
    static constexpr auto M{"[Message] "} ;
}

#endif