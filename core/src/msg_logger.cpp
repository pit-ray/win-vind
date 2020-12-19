#include "msg_logger.hpp"

#include <windows.h>
#include <mutex>

namespace Logger
{
    static std::mutex mtx ;
    static constexpr auto g_er_tag = "[Error] " ;

    static constexpr auto efilename{"log/error.log"} ;
    static std::ofstream _init_error_stream ;
    static std::ofstream error_stream ;

    static constexpr auto mfilename{"log/message.log"} ;
    static std::ofstream _init_msg_stream ;
    static std::ofstream msg_stream ;
    static constexpr auto M{"[Message] "} ;

    void initialize() {
        _init_error_stream.open(efilename, std::ios::trunc) ;
         error_stream.open(efilename, std::ios::app) ;

        _init_msg_stream.open(efilename, std::ios::trunc) ;
         msg_stream.open(efilename, std::ios::app) ;
    }

    template <typename T>
    inline void _error(T&& msg, const char* scope) {
        std::lock_guard<std::mutex> lock(mtx) ;
        if(error_stream.is_open()) {
            error_stream << g_er_tag << "Windows Error Code: [" << GetLastError() << "], " << msg << " (" << scope << ")" << std::endl ;
        }
    }
    void error(const char* msg, const char* scope) {
        _error(msg, scope) ;
    }
    void error(std::string&& msg, const char* scope) {
        _error(std::move(msg), scope) ;
    }
    void error(const std::string& msg, const char* scope) {
        _error(msg, scope) ;
    }
}
