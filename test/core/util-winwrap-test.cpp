#include <doctest.h>

#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>

// define stub ------------------------------------------
namespace winwrap_test
{
    using MSG = int ;
    using HWND = void* ;
    using BOOL = int ;

    constexpr int PM_REMOVE = 1 ;
    constexpr int INVALID_FILE_ATTRIBUTES = 1 ;
    constexpr int FILE_ATTRIBUTE_DIRECTORY = 2 ;
    constexpr int CREATE_NEW_CONSOLE = 1 ;
    constexpr BOOL TRUE = 20 ;
    constexpr BOOL FALSE = 0 ;

    MSG PeekMessage_msg_value = 0 ;
    int PeekMessage(MSG* msg, void*, int, int, int) {
        *msg = PeekMessage_msg_value ;
        return 1 ;
    }
    void TranslateMessage(MSG*) {}
    void DispatchMessage(MSG*) {}

    BOOL InvalidateRect(HWND, void*, BOOL) {
        return FALSE ;
    }

    std::string GetFileAttributes_first_arg{} ;
    int GetFileAttributesW(const char* path) {
        GetFileAttributes_first_arg = path ;
        return FILE_ATTRIBUTE_DIRECTORY ;
    }

    std::string CreateDirectory_first_arg{} ;
    bool CreateDirectory_return = true ;
    bool CreateDirectoryW(const char* path, void*) {
        CreateDirectory_first_arg = path ;
        return CreateDirectory_return ;
    }

    std::string CopyFile_first_arg{} ;
    std::string CopyFile_second_arg{} ;
    BOOL CopyFile_third_arg = false ;
    bool CopyFile_return = true ;
    bool CopyFileW(const char* src, const char* dst, BOOL error_overwrite) {
        CopyFile_first_arg = src ;
        CopyFile_second_arg = dst ;
        CopyFile_third_arg = error_overwrite ;
        return CopyFile_return ;
    }

    using HANDLE = std::pair<std::string, std::string> ;
    struct STARTUPINFOW {
        std::size_t cb ;
    } ;

    struct PROCESS_INFORMATION {
        HANDLE hProcess ;
    } ;

    void ZeroMemory(STARTUPINFOW*, std::size_t) {}
    void ZeroMemory(PROCESS_INFORMATION*, std::size_t) {}

    using LPWSTR = char* ;

    bool CreateProcess_return = true ;
    bool CreateProcessW(
            void*, LPWSTR cmd, void*, void*,
            BOOL, int, void*, const char* current_dir,
            STARTUPINFOW*, PROCESS_INFORMATION* pi) {
        pi->hProcess = std::make_pair(cmd, current_dir) ;
        return CreateProcess_return ;
    }
    // end stub ---------------------------------------------

#define _STRING_HPP
    namespace vind {
        namespace util {
            std::string s_to_ws(std::string str) {
                return str ;
            }
        }
    }

#define _WINDOWS_
#include "util/winwrap.cpp"
#undef _STRING_HPP
#undef _WINDOWS_

    using namespace vind::util ;

    TEST_CASE("(util::get_win_message) PeekMessage Argument Test") {
        PeekMessage_msg_value = 29 ;   //message
        CHECK_EQ(get_win_message(), 29) ;
    }

    TEST_CASE("(util::refresh_display) exception test") {
        CHECK_THROWS_AS(refresh_display(NULL), std::runtime_error) ;
    }

    TEST_CASE("(util::b_to_B) Type test") {
        CHECK_EQ(b_to_B(true), TRUE) ;
        CHECK_EQ(b_to_B(false), FALSE) ;
    }

    TEST_CASE("(util::is_existed_dir) Arguments Test") {
        is_existed_dir("Hello") ;
        CHECK_EQ(GetFileAttributes_first_arg, std::string("Hello")) ;
    }

    TEST_CASE("(util::create_directory) Arguments Test") {
        CreateDirectory_return = true ;
        create_directory("Hello") ;
        CHECK_EQ(CreateDirectory_first_arg, std::string("Hello")) ;
    }
    TEST_CASE("(util::create_directory) Exception Test") {
        CreateDirectory_return = false ;
        CHECK_THROWS_AS(create_directory("Hello"), std::runtime_error) ;
    }

    TEST_CASE("(util::copy_file) Arguments Test") {
        CopyFile_return = true ;
        copy_file("First", "Second", true) ;
        CHECK_EQ(CopyFile_first_arg, std::string("First")) ;
        CHECK_EQ(CopyFile_second_arg, std::string("Second")) ;
        CHECK_EQ(CopyFile_third_arg, FALSE) ;
    }
    TEST_CASE("(util::copy_file) Exception Test") {
        CopyFile_return = false ;
        CHECK_THROWS_AS(copy_file("S", "D", true), std::runtime_error) ;
    }

    TEST_CASE("(util::create_process) Argument Test") {
        CreateProcess_return = true ;
        auto [cmd, dir] = create_process("DIRECTORY", "COMMAND", "A", "B") ;
        CHECK_EQ(cmd, std::string("COMMAND A B")) ;
        CHECK_EQ(dir, std::string("DIRECTORY")) ;
    }
    TEST_CASE("(util::create_process) Path Protect Test") {
        CreateProcess_return = true ;
        auto [cmd, dir] = create_process("DIRECTORY", "Program Files/COMMAND", "A", "B") ;
        CHECK_EQ(cmd, std::string("\"Program Files/COMMAND\" A B")) ;
        CHECK_EQ(dir, std::string("DIRECTORY")) ;
    }
    TEST_CASE("(util::create_process) Exception Error") {
        CreateProcess_return = false ;
        CHECK_THROWS_AS(create_process("DIR", "CMD", "A", "B"), std::runtime_error) ;
    }
}
