#include <doctest.h>

#define PRINT_ERROR(msg) /*(msg)*/

#include "util/disable_compiler_warning.hpp"

#include "util/string.cpp"
#include "util/winwrap.cpp"

#include <processthreadsapi.h>

#include <stdexcept>
#include <utility>


namespace vind
{
    namespace core {
        void error(const char*, const char*) {
            return ;
        }
    }
}


// define stub ------------------------------------------
#include <fff.h>

DEFINE_FFF_GLOBALS ;

FAKE_VALUE_FUNC(BOOL, PeekMessageW, LPMSG, HWND, UINT, UINT, UINT) ;
FAKE_VALUE_FUNC(BOOL, TranslateMessage, const MSG*) ;
FAKE_VALUE_FUNC(LRESULT, DispatchMessageW, const MSG*) ;
FAKE_VALUE_FUNC(BOOL, InvalidateRect, HWND, const RECT*, BOOL) ;
FAKE_VALUE_FUNC(BOOL, CreateProcessW, \
        const wchar_t*, wchar_t*, \
        LPSECURITY_ATTRIBUTES, LPSECURITY_ATTRIBUTES, \
        BOOL, DWORD, LPVOID, const wchar_t*, \
        STARTUPINFOW*, PROCESS_INFORMATION*) ;
FAKE_VALUE_FUNC(DWORD, GetWindowThreadProcessId, HWND, LPDWORD) ;
FAKE_VALUE_FUNC(DWORD, GetModuleFileNameExW, HANDLE, HMODULE, LPWSTR, DWORD) ;
FAKE_VALUE_FUNC(HANDLE, OpenProcess, DWORD, BOOL, DWORD) ;

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
}


TEST_CASE("util/winwrap Under Fake Windows API: ") {
    RESET_FAKE(PeekMessageW) ;
    RESET_FAKE(TranslateMessage) ;
    RESET_FAKE(DispatchMessageW) ;
    RESET_FAKE(InvalidateRect) ;
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

    SUBCASE("(util::create_process) Exception Error") {
        CreateProcessW_fake.return_val = FALSE ;
        CHECK_THROWS_AS(create_process("DIR", "CMD", concat_args("A", "B")), std::runtime_error) ;
    }
}

#include "util/enable_compiler_warning.hpp"
