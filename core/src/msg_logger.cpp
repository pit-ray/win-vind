#include "msg_logger.hpp"

#include <unordered_map>
#include <windows.h>

#include <iomanip>
#include <string>
#include <sstream>
#include <vector>

#include "path.hpp"
#include "utility.hpp"

#define KEEPING_LOG_COUNT (5)

namespace Logger
{
    static constexpr auto g_er_tag = "[Error] " ;
    static constexpr auto g_mg_tag = "[Message] " ;

    static std::ofstream _init_error_stream ;
    static std::ofstream error_stream ;

    static std::ofstream _init_msg_stream ;
    static std::ofstream msg_stream ;

    static const std::string log_dir = Path::ROOT_PATH() + "log\\" ;

    inline static void remove_files_over(const std::string pattern_withex, const std::size_t num)
    {
        std::vector<std::wstring> files ;

        WIN32_FIND_DATAW wfd = {} ;
        auto handle = FindFirstFileW(Utility::s_to_ws(pattern_withex).c_str(), &wfd) ;
        if(handle == INVALID_HANDLE_VALUE) {
            return ;
        }
        files.push_back(Utility::s_to_ws(log_dir) + wfd.cFileName) ;

        while(FindNextFileW(handle, &wfd)) {
            files.push_back(Utility::s_to_ws(log_dir) + wfd.cFileName) ;
        }
        FindClose(handle) ;

        if(files.size() <= num) {
            return ;
        }

        std::sort(files.begin(), files.end(), std::greater<std::wstring>{}) ;
        for(std::size_t i = num ; i < files.size() ; i ++) {
            DeleteFileW(files[i].c_str()) ;
        }
    }

    void initialize() {
        SYSTEMTIME stime ;
        GetLocalTime(&stime) ;

        std::ostringstream ss ;
        ss << stime.wYear \
            << std::setw(2) << std::setfill('0') << stime.wMonth \
            << std::setw(2) << std::setfill('0') << stime.wDay \
            << std::setw(2) << std::setfill('0') << stime.wHour \
            << std::setw(2) << std::setfill('0') << stime.wMinute ;

        if(!Utility::is_existed_dir(log_dir)) {
            Utility::create_directory(log_dir) ;
        }

        const auto efile = log_dir + "error_" + ss.str() + ".log" ;
        const auto mfile = log_dir + "message_" + ss.str() + ".log" ;

        _init_error_stream.open(Path::to_u8path(efile), std::ios::trunc) ;
         error_stream.open(Path::to_u8path(efile), std::ios::app) ;

        _init_msg_stream.open(Path::to_u8path(mfile), std::ios::trunc) ;
         msg_stream.open(Path::to_u8path(mfile), std::ios::app) ;

         //If the log files exists over five, remove old files.
         remove_files_over(log_dir + "error_*.log", KEEPING_LOG_COUNT) ;
         remove_files_over(log_dir + "message_*.log", KEEPING_LOG_COUNT) ;
    }

    template <typename T>
    inline static void _error(T&& msg, const char* scope) {
        if(error_stream.is_open()) {
            error_stream << g_er_tag \
                << "Windows Error Code: [" << GetLastError() << "], " \
                << msg << " (" << scope << ")" << std::endl ;
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

    template <typename T>
    inline static void _message(T&& msg, const char* scope) {
        if(msg_stream.is_open()) {
            msg_stream << g_mg_tag << msg << " (" << scope << ")" << std::endl ;
            msg_stream.flush() ;
        }
    }
    void message(const char* msg, const char* scope) {
        _message(msg, scope) ;
    }
    void message(std::string&& msg, const char* scope) {
        _message(std::move(msg), scope) ;
    }
    void message(const std::string& msg, const char* scope) {
        _message(msg, scope) ;
    }
}
