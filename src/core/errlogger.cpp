#include "errlogger.hpp"

#include "path.hpp"
#include "util/debug.hpp"
#include "util/winwrap.hpp"
#include "version.hpp"

#include <unordered_map>
#include <windows.h>

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <mutex>
#include <sstream>
#include <string>
#include <vector>


namespace
{
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
}

namespace vind
{
    namespace core {
        struct Logger::Impl {
            std::ofstream stream_{} ;

            // When writing to a stream in a multi-threaded manner,
            // an exclusion process is performed so that the contents do not get mixed up.
            std::mutex mtx_{} ;
        } ;

        Logger::Logger() noexcept
        : pimpl(nullptr)
        {}

        Logger::~Logger() noexcept = default ;

        /**
         * NOTE: Inspired by bitcoin's logger implementation,
         * it dynamically allocates local static variables.
         * This allows us to leave the freeing of variables to
         * the OS/libc and survive until the end in the process.
         * Using this design pattern, Logger is available
         * in destructors of objects with undefined release order,
         * such as static/global.
         *
         * ref.(https://github.com/bitcoin/bitcoin/blob/57982f419e36d0023c83af2dd0d683ca3160dc2a/src/logging.cpp#L17-L36)
         */
        Logger& Logger::get_instance() {
            static auto instance = new Logger() ;
            return *instance ;
        }

        void Logger::init(
                const std::string& filename_head,
                std::size_t keeping_log_num,
                std::size_t align_width_of_header) {

            if(!pimpl) {
                pimpl = std::make_unique<Impl>() ;
            }

            auto log_dir = ROOT_PATH() / "log" ;

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

            auto filepath = log_dir / (filename_head + ss.str() + ".log") ;

            pimpl->stream_.open(filepath, std::ios::app) ;

            auto add_spec = [
                &st = pimpl->stream_,
                align_width_of_header
            ](auto&& key, auto&&... vals) {
                st << std::right ;
                st << std::setw(align_width_of_header) ;
                st << std::forward<decltype(key)>(key) ;
                st << std::left ;
                st << std::setw(0) ;
                ((st << std::forward<decltype(vals)>(vals)), ...) ;
                st << std::endl ;
                st.flush() ;
            } ;

            // Export system infomation for handling issues.
            pimpl->stream_ << "========== System Infomation ==========\n" ;
            pimpl->stream_ << "[Windows]\n" ;

            auto [major, minor, build] = util::get_Windows_versions() ;

            add_spec("Edition: ", util::get_Windows_edition(major, minor)) ;
            add_spec("Version: ", util::get_Windows_display_version()) ;
            add_spec("Build Numbers: ", major, ".", minor, ".", build) ;
            add_spec("Architecture: ", util::get_Windows_architecture()) ;

            pimpl->stream_ << std::endl ;

            pimpl->stream_ << "[win-vind]\n" ;
            add_spec("Version: ", WIN_VIND_VERSION) ;

            pimpl->stream_ << "=======================================\n" ;
            pimpl->stream_.flush() ;

             //If the log files exists over five, remove old files.
            remove_files_over(log_dir, filename_head + "*.log", keeping_log_num) ;
        }

        /*
         * Note: Since error, message, and warning may be called
         *       in a destructor, make strong guarantees and output
         *       them only if they are writable.
         */
        void Logger::error(const std::string& msg, const std::string& scope) noexcept {
            if(!pimpl || !pimpl->stream_.is_open()) {
                return ;
            }

            try {
                std::lock_guard<std::mutex> scoped_lock{pimpl->mtx_} ;

                auto win_ercode = GetLastError() ;
                if(win_ercode) {
                    pimpl->stream_ << "[Error] " ;

                    LPSTR msgbuf = nullptr ;
                    auto size = FormatMessageA(
                                FORMAT_MESSAGE_ALLOCATE_BUFFER | \
                                FORMAT_MESSAGE_FROM_SYSTEM | \
                                FORMAT_MESSAGE_IGNORE_INSERTS,
                                NULL, win_ercode,
                                MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),
                                reinterpret_cast<LPSTR>(&msgbuf),
                                0, NULL) ;

                    if(size > 2) {
                        // print message without \r\n
                        pimpl->stream_ << std::string(msgbuf, size - 2) ;
                    }
                    else {
                        pimpl->stream_ << "Windows Error Code: [" << win_ercode << "]" ;
                    }

                    if(size > 0) {
                        LocalFree(msgbuf) ;
                    }

                    pimpl->stream_ <<  std::endl ;
                }

                pimpl->stream_ << "[Error] " << msg ;
                if(!scope.empty()) {
                    pimpl->stream_ << " (" << scope << ")" ;
                }
                pimpl->stream_ << std::endl ;

                pimpl->stream_.flush() ;
            }
            catch(...) {
                return ;
            }
        }

        void Logger::message(const std::string& msg, const std::string& scope) noexcept {
            if(!pimpl || !pimpl->stream_.is_open()) {
                return ;
            }

            try {
                std::lock_guard<std::mutex> scoped_lock{pimpl->mtx_} ;

                pimpl->stream_ << "[Message] " << msg ;
                if(!scope.empty()) {
                    pimpl->stream_ << " (" << scope << ")" ;
                }
                pimpl->stream_ << std::endl ;
                pimpl->stream_.flush() ;
            }
            catch(...) {
                return ;
            }
        }

        void Logger::warning(const std::string& msg, const std::string& scope) noexcept {
            if(!pimpl || !pimpl->stream_.is_open()) {
                return ;
            }

            try {
                std::lock_guard<std::mutex> scoped_lock{pimpl->mtx_} ;

                pimpl->stream_ << "[Warning] " << msg ;
                if(!scope.empty()) {
                    pimpl->stream_ << " (" << scope << ")" ;
                }
                pimpl->stream_ << std::endl ;
                pimpl->stream_.flush() ;
            }
            catch(...) {
                return ;
            }
        }
    }
}
