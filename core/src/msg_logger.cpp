#include "msg_logger.hpp"

#include <stdexcept>
#include <windows.h>

inline static bool is_existed_dir(std::string path) noexcept
{
  auto flag = GetFileAttributesA(path.c_str());
  return (flag != INVALID_FILE_ATTRIBUTES && (flag & FILE_ATTRIBUTE_DIRECTORY));
}

namespace Logger
{
    static constexpr auto g_er_tag = "[Error] " ;
    static constexpr auto M{"[Message] "} ;

    static std::ofstream _init_error_stream ;
    static std::ofstream error_stream ;

    static std::ofstream _init_msg_stream ;
    static std::ofstream msg_stream ;

    void initialize() {
        const std::string log_dir = "log" ;

        if(!is_existed_dir(log_dir)) {
            if(!CreateDirectoryA(log_dir.c_str(), NULL)) {
                throw std::logic_error("Cannot create log directory.") ;
            }
        }

        const auto efile = log_dir + "/error.log" ;
        const auto mfile = log_dir + "/message.log" ;

        _init_error_stream.open(efile, std::ios::trunc) ;
         error_stream.open(efile, std::ios::app) ;

        _init_msg_stream.open(mfile, std::ios::trunc) ;
         msg_stream.open(mfile, std::ios::app) ;
    }

    template <typename T>
    inline void _error(T&& msg, const char* scope) {
        if(error_stream.is_open()) {
            error_stream << g_er_tag << "Windows Error Code: [" << GetLastError() << "], " << msg << " (" << scope << ")" << std::endl ;
            error_stream.flush() ;
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
