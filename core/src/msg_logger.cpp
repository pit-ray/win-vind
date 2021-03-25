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

//internal linkage
namespace
{
    constexpr auto g_er_tag = "[Error] " ;
    constexpr auto g_mg_tag = "[Message] " ;

    std::ofstream g_init_error_stream ;
    std::ofstream g_error_stream ;

    std::ofstream g_init_msg_stream ;
    std::ofstream g_msg_stream ;

    inline void remove_files_over(
            const std::string& log_dir,
            const std::string pattern_withex,
            const std::size_t num) {
        std::vector<std::wstring> files ;

        WIN32_FIND_DATAW wfd = {} ;
        auto handle = FindFirstFileW(vind::utility::s_to_ws(log_dir + pattern_withex).c_str(), &wfd) ;
        if(handle == INVALID_HANDLE_VALUE) {
            return ;
        }
        files.push_back(vind::utility::s_to_ws(log_dir) + wfd.cFileName) ;

        while(FindNextFileW(handle, &wfd)) {
            files.push_back(vind::utility::s_to_ws(log_dir) + wfd.cFileName) ;
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

    template <typename T>
    inline void error_print_common_process(T&& msg, const char* scope) {
        if(g_error_stream.is_open()) {
            g_error_stream << g_er_tag \
                << "Windows Error Code: [" << GetLastError() << "], " \
                << msg << " (" << scope << ")" << std::endl ;
            g_error_stream.flush() ;
        }
    }

    template <typename T>
    inline void msg_print_common_process(T&& msg, const char* scope) {
        if(g_msg_stream.is_open()) {
            g_msg_stream << g_mg_tag << msg << " (" << scope << ")" << std::endl ;
            g_msg_stream.flush() ;
        }
    }
}

namespace vind
{
    namespace log {
        void initialize() {
            const std::string log_dir = path::ROOT_PATH() + "log\\" ;

            SYSTEMTIME stime ;
            GetLocalTime(&stime) ;

            std::ostringstream ss ;
            ss << stime.wYear \
                << std::setw(2) << std::setfill('0') << stime.wMonth \
                << std::setw(2) << std::setfill('0') << stime.wDay \
                << std::setw(2) << std::setfill('0') << stime.wHour \
                << std::setw(2) << std::setfill('0') << stime.wMinute ;

            if(!utility::is_existed_dir(log_dir)) {
                utility::create_directory(log_dir) ;
            }

            const auto efile = log_dir + "error_" + ss.str() + ".log" ;
            const auto mfile = log_dir + "message_" + ss.str() + ".log" ;

            g_init_error_stream.open(path::to_u8path(efile), std::ios::trunc) ;
             g_error_stream.open(path::to_u8path(efile), std::ios::app) ;

            g_init_msg_stream.open(path::to_u8path(mfile), std::ios::trunc) ;
             g_msg_stream.open(path::to_u8path(mfile), std::ios::app) ;

             //If the log files exists over five, remove old files.
             remove_files_over(log_dir, "error_*.log", KEEPING_LOG_COUNT) ;
             remove_files_over(log_dir, "message_*.log", KEEPING_LOG_COUNT) ;
        }

        void error(const char* msg, const char* scope) {
            error_print_common_process(msg, scope) ;
        }
        void error(std::string&& msg, const char* scope) {
            error_print_common_process(std::move(msg), scope) ;
        }
        void error(const std::string& msg, const char* scope) {
            error_print_common_process(msg, scope) ;
        }

        void message(const char* msg, const char* scope) {
            msg_print_common_process(msg, scope) ;
        }
        void message(std::string&& msg, const char* scope) {
            msg_print_common_process(std::move(msg), scope) ;
        }
        void message(const std::string& msg, const char* scope) {
            msg_print_common_process(msg, scope) ;
        }
    }
}
