#include <doctest.h>

#include "disable_gcc_warning.hpp"

#include "fakes/util_fake_string.hpp"

#include <fff.h>

#include "util/winwrap.cpp"

#include <processthreadsapi.h>

#include <stdexcept>
#include <utility>

FAKE_VALUE_FUNC(BOOL, PeekMessageW, LPMSG, HWND, UINT, UINT, UINT) ;
FAKE_VALUE_FUNC(BOOL, TranslateMessage, const MSG*) ;
FAKE_VALUE_FUNC(LRESULT, DispatchMessageW, const MSG*) ;
FAKE_VALUE_FUNC(BOOL, InvalidateRect, HWND, const RECT*, BOOL) ;
FAKE_VALUE_FUNC(DWORD, GetFileAttributesW, const wchar_t*) ;
FAKE_VALUE_FUNC(BOOL, CreateDirectoryW, const wchar_t*, LPSECURITY_ATTRIBUTES) ;
FAKE_VALUE_FUNC(BOOL, CopyFileW, const wchar_t*, const wchar_t*, BOOL) ;
FAKE_VALUE_FUNC(BOOL, CreateProcessW, \
        const wchar_t*, wchar_t*, \
        LPSECURITY_ATTRIBUTES, LPSECURITY_ATTRIBUTES, \
        BOOL, DWORD, LPVOID, const wchar_t*, \
        STARTUPINFOW*, PROCESS_INFORMATION*) ;

// define stub ------------------------------------------
namespace
{
    using namespace vind::util ;

    BOOL PeekMessageW_custom_fake(LPMSG msg, HWND, UINT, UINT, UINT) {
        if(msg) msg->message = 29 ;
        return TRUE ;
    }

    //
    // These pointer arguments maybe cleared after calling, so they are captured with copying.
    //

    std::wstring GetFileAttributesW_custom_fake_arg0_val{} ;
    DWORD GetFileAttributesW_custom_fake(const wchar_t* str) {
        if(str) {
            GetFileAttributesW_custom_fake_arg0_val = str ;
        }
        return FILE_ATTRIBUTE_DIRECTORY ;
    }

    std::wstring CreateDirectoryW_custom_fake_arg0_val{} ;
    BOOL CreateDirectoryW_custom_fake(const wchar_t* str, LPSECURITY_ATTRIBUTES) {
        if(str) {
            CreateDirectoryW_custom_fake_arg0_val = str ;
        }
        return TRUE ;
    }

    std::wstring CopyFileW_custom_fake_arg0_val{} ;
    std::wstring CopyFileW_custom_fake_arg1_val{} ;
    BOOL CopyFileW_custom_fake(const wchar_t* src, const wchar_t* dst, BOOL) {
        if(src) {
            CopyFileW_custom_fake_arg0_val = src ;
        }
        if(dst) {
            CopyFileW_custom_fake_arg1_val = dst ;
        }
        return TRUE ;
    }

    std::wstring CreateProcessW_custom_fake_arg_cmd{} ;
    std::wstring CreateProcessW_custom_fake_arg_dir{} ;
    BOOL CreateProcessW_custom_fake(const wchar_t*, wchar_t* cmd,
        LPSECURITY_ATTRIBUTES, LPSECURITY_ATTRIBUTES,
        BOOL, DWORD, LPVOID, const wchar_t* dir,
        STARTUPINFOW*, PROCESS_INFORMATION*) {

        if(cmd) {
            CreateProcessW_custom_fake_arg_cmd = cmd ;
        }
        if(dir) {
            CreateProcessW_custom_fake_arg_dir = dir ;
        }
        return TRUE ;
    }
    // end stub ---------------------------------------------

}


TEST_CASE("util/winwrap Under Fake Windows API: ") {
    RESET_FAKE(PeekMessageW) ;
    RESET_FAKE(TranslateMessage) ;
    RESET_FAKE(DispatchMessageW) ;
    RESET_FAKE(InvalidateRect) ;
    RESET_FAKE(GetFileAttributesW) ;
    RESET_FAKE(CreateDirectoryW) ;
    RESET_FAKE(CopyFileW) ;
    RESET_FAKE(CreateProcessW) ;

    FFF_RESET_HISTORY() ;

    SUBCASE("(util::get_win_message) PeekMessage Arguments Test") {
        PeekMessageW_fake.custom_fake = PeekMessageW_custom_fake ;
        CHECK_EQ(get_win_message().message, 29) ;

        SUBCASE("(util::get_win_message) TranslateMessage Arguments Test") {
            CHECK_EQ(TranslateMessage_fake.arg0_val->message, 29) ;
        }

        SUBCASE("(util::get_win_message) DispatchMessage Arguments Test") {
            CHECK_EQ(DispatchMessageW_fake.arg0_val->message, 29) ;
        }
    }

    SUBCASE("(util::refresh_display) exception test") {
        InvalidateRect_fake.return_val = FALSE ;
        CHECK_THROWS_AS(refresh_display(NULL), std::runtime_error) ;
    }

    SUBCASE("(util::b_to_B) Type test") {
        CHECK_EQ(b_to_B(true), TRUE) ;
        CHECK_EQ(b_to_B(false), FALSE) ;
    }

    SUBCASE("(util::is_existed_dir) Return Value Test") {
        GetFileAttributesW_fake.custom_fake = GetFileAttributesW_custom_fake ;
        std::string str("Hello") ;
        CHECK(is_existed_dir(str)) ;

        SUBCASE("(util::is_existed_dir) GetFileAttributesW Arguments Test") {
            CHECK_EQ(std::strcmp(from_fake_wstr(GetFileAttributesW_custom_fake_arg0_val).c_str(), "Hello"), 0) ;
        }
    }

    SUBCASE("(util::create_directory) CreateDirectoryW Arguments Test") {
        CreateDirectoryW_fake.custom_fake = CreateDirectoryW_custom_fake ;
        create_directory("Hello") ;

        CHECK_EQ(std::strcmp(from_fake_wstr(CreateDirectoryW_custom_fake_arg0_val).c_str(), "Hello"), 0) ;
    }

    SUBCASE("(util::create_directory) Exception Test") {
        CreateDirectoryW_fake.return_val = FALSE ;
        CHECK_THROWS_AS(create_directory("Hello"), std::runtime_error) ;
    }

    SUBCASE("(util::copy_file) Arguments Test") {
        CopyFileW_fake.custom_fake = CopyFileW_custom_fake ;
        copy_file("First", "Second", true) ;

        CHECK_EQ(std::strcmp(from_fake_wstr(CopyFileW_custom_fake_arg0_val).c_str(), "First"), 0) ;
        CHECK_EQ(std::strcmp(from_fake_wstr(CopyFileW_custom_fake_arg1_val).c_str(), "Second"), 0) ;
        CHECK_EQ(CopyFileW_fake.arg2_val, FALSE) ;
    }
    SUBCASE("(util::copy_file) Exception Test") {
        CopyFileW_fake.return_val = FALSE ;
        CHECK_THROWS_AS(copy_file("S", "D", true), std::runtime_error) ;
    }

    SUBCASE("(util::create_process) Argument Test") {
        CreateProcessW_fake.custom_fake = CreateProcessW_custom_fake ;
        create_process("DIRECTORY", "COMMAND", "A", "B") ;
        CHECK_EQ(std::strcmp(from_fake_wstr(CreateProcessW_custom_fake_arg_cmd).c_str(), "COMMAND A B"), 0) ;
        CHECK_EQ(std::strcmp(from_fake_wstr(CreateProcessW_custom_fake_arg_dir).c_str(), "DIRECTORY"), 0) ;
    }
    SUBCASE("(util::create_process) Path Protect Test") {
        CreateProcessW_fake.custom_fake = CreateProcessW_custom_fake ;
        create_process("DIRECTORY", "Program Files/COMMAND", "A", "B") ;
        CHECK_EQ(std::strcmp(from_fake_wstr(CreateProcessW_custom_fake_arg_cmd).c_str(), "\"Program Files/COMMAND\" A B"), 0) ;
        CHECK_EQ(std::strcmp(from_fake_wstr(CreateProcessW_custom_fake_arg_dir).c_str(), "DIRECTORY"), 0) ;
    }
    SUBCASE("(util::create_process) Exception Error") {
        CreateProcessW_fake.return_val = FALSE ;
        CHECK_THROWS_AS(create_process("DIR", "CMD", "A", "B"), std::runtime_error) ;
    }
}

#include "enable_gcc_warning.hpp"
