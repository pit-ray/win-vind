#include "disable_gcc_warning.hpp"

#include <windows.h>

#include <fff.h>

#include <iostream>
#include <string>

#include "fakes/util_fake_string.hpp"

#include "fakes/coreio_fake_path.hpp"

namespace vind
{
    namespace util {
        bool is_existed_dir(const std::string&) {
            return true ;
        }

        void create_directory(const std::string&) {
        }
    }
}
using namespace vind::util ;

DEFINE_FFF_GLOBALS ;

FAKE_VALUE_FUNC(HANDLE, FindFirstFileW, LPCWSTR, LPWIN32_FIND_DATAW) ;
FAKE_VALUE_FUNC(BOOL, FindNextFileW, HANDLE, WIN32_FIND_DATAW*) ;
FAKE_VALUE_FUNC(BOOL, FindClose, HANDLE) ;
FAKE_VALUE_FUNC(BOOL, DeleteFileW, LPCWSTR) ;

namespace
{
    long valid_value = 0 ;
    auto VALID_HANDLE = reinterpret_cast<HANDLE>(&valid_value) ;

    //custom fake functions
    std::string FindFirstFileW_custom_fake_arg0_val{} ;
    HANDLE FindFirstFileW_custom_fake_return_val = VALID_HANDLE ;
    HANDLE FindFirstFileW_custom_fake(const wchar_t* path, WIN32_FIND_DATAW* wfd) {
        if(!path || !wfd) {
            return INVALID_HANDLE_VALUE ;
        }
        FindFirstFileW_custom_fake_arg0_val = from_fake_wstr(path) ;
        auto n = s_to_ws("C") ;
        std::memcpy(wfd->cFileName, n.data(), sizeof(wchar_t) * n.size()) ;
        return FindFirstFileW_custom_fake_return_val ;
    }

    std::string DeleteFileW_custom_fake_arg0_val{} ;
    BOOL DeleteFileW_custom_fake(const wchar_t* f) {
        if(!f) return FALSE ;

        DeleteFileW_custom_fake_arg0_val = from_fake_wstr(f) ;
        return TRUE ;
    }
}

#include "coreio/err_logger.cpp"

namespace
{
    using namespace vind::errlogger ;
}

#include <doctest.h>

TEST_CASE("(coreio/errlogger/remove_files_over) Under Fake Windows API: ")  {
    RESET_FAKE(FindFirstFileW) ;
    RESET_FAKE(FindNextFileW) ;
    RESET_FAKE(FindClose) ;
    RESET_FAKE(DeleteFileW) ;

    FindFirstFileW_custom_fake_return_val = VALID_HANDLE ;

    std::string log_dir("A") ;
    std::string pattern("B") ;

    SUBCASE("Return invalid handle") {
        FindFirstFileW_fake.return_val = INVALID_HANDLE_VALUE ;
        CHECK_NOTHROW(remove_files_over(log_dir, pattern, 5)) ;
        CHECK_EQ(FindNextFileW_fake.call_count, 0) ;
    }

    SUBCASE("FindFirstFileW Arguments Test") {
        FindFirstFileW_fake.custom_fake = FindFirstFileW_custom_fake ;
        FindFirstFileW_custom_fake_return_val = INVALID_HANDLE_VALUE ;

        CHECK_NOTHROW(remove_files_over(log_dir, pattern, 5)) ;
        CHECK_EQ(std::strcmp(FindFirstFileW_custom_fake_arg0_val.c_str(), "AB"), 0) ;
    }

    SUBCASE("Find smaller files than the passed number") {
        FindFirstFileW_fake.custom_fake = FindFirstFileW_custom_fake ;

        BOOL returns[] = {TRUE, FALSE} ;
        SET_RETURN_SEQ(FindNextFileW, returns, 2) ;

        FindClose_fake.return_val = TRUE ;

        CHECK_NOTHROW(remove_files_over(log_dir, pattern, 5)) ;
        CHECK_EQ(DeleteFileW_fake.call_count, 0) ;
    }

    SUBCASE("Find bigger files than the passed number") {
        FindFirstFileW_fake.custom_fake = FindFirstFileW_custom_fake ;
        DeleteFileW_fake.custom_fake = DeleteFileW_custom_fake ;

        BOOL returns[] = {TRUE, TRUE, TRUE, FALSE} ;
        SET_RETURN_SEQ(FindNextFileW, returns, 4) ;

        FindClose_fake.return_val = TRUE ;

        CHECK_NOTHROW(remove_files_over(log_dir, pattern, 2)) ;
        CHECK_GT(DeleteFileW_fake.call_count, 0) ;

        CHECK_EQ(std::strncmp(DeleteFileW_custom_fake_arg0_val.c_str(), "A\0C", 2), 0) ;
    }
}

/*
TEST_CASE("coreio/errlogger: Initialize errlogger under the fake Windows API: ") {
}
*/

#include "enable_gcc_warning.hpp"
