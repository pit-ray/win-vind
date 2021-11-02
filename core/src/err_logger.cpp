#include "err_logger.hpp"
#include "path.hpp"
#include "version.hpp"

#include <unordered_map>
#include <windows.h>

#include <algorithm>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

#include "util/winwrap.hpp"

#ifdef DEBUG
#include <iostream>
#endif

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

    void remove_files_over(
            const std::filesystem::path& log_dir,
            std::string pattern_withex,
            std::size_t num) {
        std::vector<std::filesystem::path> files ;

        WIN32_FIND_DATAW wfd = {} ;

        auto query = log_dir / pattern_withex ;
        query.make_preferred() ;

        auto handle = FindFirstFileW(query.wstring().c_str(), &wfd) ;
        if(handle == INVALID_HANDLE_VALUE) {
            return ;
        }

        do {
            auto log_path = log_dir / wfd.cFileName ;
            log_path.make_preferred() ;
            files.push_back(std::move(log_path)) ;
        } while(FindNextFileW(handle, &wfd)) ;

        FindClose(handle) ;

        if(files.size() <= num) {
            return ;
        }

        std::sort(files.begin(), files.end(), std::greater<std::wstring>{}) ;
        for(std::size_t i = num ; i < files.size() ; i ++) {
            std::error_code ec ;
            if(!std::filesystem::remove(files[i], ec)) {
                PRINT_ERROR(ec.message()) ;
            }
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
    namespace errlogger {
        void initialize() {
            auto log_dir = path::ROOT_PATH() / "log" ;

            SYSTEMTIME stime ;
            GetLocalTime(&stime) ;

            std::ostringstream ss ;
            ss << stime.wYear \
                << std::setw(2) << std::setfill('0') << stime.wMonth \
                << std::setw(2) << std::setfill('0') << stime.wDay \
                << std::setw(2) << std::setfill('0') << stime.wHour \
                << std::setw(2) << std::setfill('0') << stime.wMinute ;

            if(!std::filesystem::exists(log_dir)) {
                std::filesystem::create_directories(log_dir) ;
            }

            auto efile = log_dir / ("error_" + ss.str() + ".log") ;
            auto mfile = log_dir / ("message_" + ss.str() + ".log") ;

            g_init_error_stream.open(efile, std::ios::trunc) ;
            g_error_stream.open(efile, std::ios::app) ;

            g_init_msg_stream.open(mfile, std::ios::trunc) ;
            g_msg_stream.open(mfile, std::ios::app) ;

            // Export system infomation for handling issues.
            constexpr auto align_width_of_header = 15 ;

            g_error_stream << "========== System Infomation ==========\n" ;
            g_error_stream << "[Windows]\n" ;

            auto [major, minor, build] = util::get_Windows_versions() ;

            g_error_stream << std::right << std::setw(align_width_of_header) << "Edition: " ;
            g_error_stream << std::left << std::setw(0) << util::get_Windows_edition(major, minor) << std::endl ;

            g_error_stream << std::right << std::setw(align_width_of_header) << "Version: " ;
            g_error_stream << std::left << std::setw(0) << util::get_Windows_display_version() << std::endl ;

            g_error_stream << std::right << std::setw(align_width_of_header) << "Build Numbers: " ;
            g_error_stream << std::left << std::setw(0) << major << "." << minor << "." << build << std::endl ;

            g_error_stream << std::right << std::setw(align_width_of_header) << "Architecture: " ;
            g_error_stream << std::left << std::setw(0) << util::get_Windows_architecture() << std::endl ;

            g_error_stream << std::endl ;

            g_error_stream << "[win-vind]\n" ;
            g_error_stream << std::right << std::setw(align_width_of_header) << "Version: " ;
            g_error_stream << std::left << std::setw(0) << WIN_VIND_VERSION << std::endl ;
            g_error_stream << std::endl ;

            g_error_stream << "=======================================\n" ;

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
