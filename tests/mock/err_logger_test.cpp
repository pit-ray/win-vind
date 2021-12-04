#include "util/disable_gcc_warning.hpp"

#include <windows.h>

#include <fff.h>

#include <filesystem>
#include <iostream>
#include <string>
#include <tuple>


namespace vind
{
    namespace core
    {
        const std::filesystem::path& ROOT_PATH() {
            static std::filesystem::path p{} ;
            return p ;
        }
    }
}


namespace vind
{
    namespace util
    {
        std::tuple<DWORD, DWORD, DWORD> get_Windows_versions() {
            return {10, 0, 22000} ;
        }

        std::string get_Windows_edition(DWORD, DWORD) {
            return "Windows 10" ;
        }

        std::string get_Windows_architecture() noexcept {
            return "x64" ;
        }

        std::string get_Windows_display_version() {
            return "1903" ;
        }
    }
}
using namespace vind::util ;

DEFINE_FFF_GLOBALS ;

FAKE_VALUE_FUNC(HANDLE, FindFirstFileW, LPCWSTR, LPWIN32_FIND_DATAW) ;
FAKE_VALUE_FUNC(BOOL, FindNextFileW, HANDLE, WIN32_FIND_DATAW*) ;
FAKE_VALUE_FUNC(BOOL, FindClose, HANDLE) ;

namespace
{
    long valid_value = 0 ;
    auto VALID_HANDLE = reinterpret_cast<HANDLE>(&valid_value) ;
}

#include "core/err_logger.cpp"

#include <doctest.h>

TEST_CASE("(errlogger/remove_files_over) Under Fake Windows API: ")  {
    RESET_FAKE(FindFirstFileW) ;
    RESET_FAKE(FindNextFileW) ;
    RESET_FAKE(FindClose) ;

    std::string log_dir("A") ;
    std::string pattern("B") ;

    SUBCASE("Return invalid handle") {
        FindFirstFileW_fake.return_val = INVALID_HANDLE_VALUE ;
        CHECK_NOTHROW(remove_files_over(log_dir, pattern, 5)) ;
        CHECK_EQ(FindNextFileW_fake.call_count, 0) ;
    }

    SUBCASE("Find smaller files than the passed number") {
        BOOL returns[] = {TRUE, FALSE} ;
        SET_RETURN_SEQ(FindNextFileW, returns, 2) ;

        FindClose_fake.return_val = TRUE ;

        CHECK_NOTHROW(remove_files_over(log_dir, pattern, 5)) ;
    }

    SUBCASE("Find bigger files than the passed number") {
        BOOL returns[] = {TRUE, TRUE, TRUE, FALSE} ;
        SET_RETURN_SEQ(FindNextFileW, returns, 4) ;

        FindClose_fake.return_val = TRUE ;

        CHECK_NOTHROW(remove_files_over(log_dir, pattern, 2)) ;
    }
}

#include "util/enable_gcc_warning.hpp"
