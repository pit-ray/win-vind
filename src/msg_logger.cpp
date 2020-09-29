#include "msg_logger.hpp"

namespace Logger
{
    static std::mutex mtx ;
    static constexpr auto _er_tag = "[Error] " ;

    template <typename T>
    inline void _error(T&& msg, const char* scope) noexcept {
        std::lock_guard<std::mutex> lock(mtx) ;
        error_stream << _er_tag << msg << " (" << scope << ")\n" ;
    }
    void error(const char* msg, const char* scope) noexcept {
        _error(msg, scope) ;
    }
    void error(std::string&& msg, const char* scope) noexcept {
        _error(std::move(msg), scope) ;
    }
    void error(const std::string& msg, const char* scope) noexcept {
        _error(msg, scope) ;
    }

    template <typename T>
    inline void _win_error(T&& msg, const char* scope) noexcept {
        std::lock_guard<std::mutex> lock(mtx) ;
        error_stream << _er_tag << "windows.h: [" << GetLastError() << "] " << msg << " (" << scope << ")\n" ;
    }
    void win_error(const char* msg, const char* scope) noexcept {
        _win_error(msg, scope) ;
    }
    void win_error(std::string&& msg, const char* scope) noexcept {
        _win_error(std::move(msg), scope) ;
    }
    void win_error(const std::string& msg, const char* scope) noexcept {
        _win_error(msg, scope) ;
    }
}
