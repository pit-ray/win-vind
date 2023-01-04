#include <doctest/doctest.h>

#include "util/disable_compiler_warning.hpp"

#include <cstring>
#include <string>

#include "util/string.cpp"

#include <meekrosoft/fff.h>

DEFINE_FFF_GLOBALS ;

FAKE_VALUE_FUNC(int, MultiByteToWideChar, UINT, DWORD, const char*, int, LPWSTR, int) ;
FAKE_VALUE_FUNC(int, WideCharToMultiByte, UINT, DWORD, const wchar_t*, int, LPSTR, int, LPCCH, LPBOOL) ;

namespace
{
    std::wstring MultiByteToWideChar_dst{} ;
    int MultiByteToWideChar_custom_fake(UINT, DWORD, const char*, int, LPWSTR dst, int) {
        if(dst != NULL) {
            std::wcscpy(dst, MultiByteToWideChar_dst.c_str()) ;
        }
        return static_cast<int>(MultiByteToWideChar_dst.size()) ;
    }

    std::string WideCharToMultiByte_dst{} ;
    int WideCharToMultiByte_custom_fake(UINT, DWORD, const wchar_t*, int, LPSTR dst, int, LPCCH, LPBOOL) {
        if(dst != NULL) {
            std::strcpy(dst, WideCharToMultiByte_dst.c_str()) ;
        }
        return static_cast<int>(WideCharToMultiByte_dst.size()) ;
    }

}

using namespace vind::util ;

TEST_CASE("util/string Under Fake Windows API: ") {
    RESET_FAKE(MultiByteToWideChar) ;
    RESET_FAKE(WideCharToMultiByte) ;
    FFF_RESET_HISTORY() ;

    // util::split
    SUBCASE("(util::split) delimiter is longer than the source string") {
        std::string str("aaa-bbb") ;
        std::vector<std::string> expect{"aaa-bbb"} ;
        CHECK_EQ(split(str, "--------------"), expect) ;
    }

    SUBCASE("(util::split) the string is empty") {
        std::string str{} ;
        auto v = split(str, "") ;
        CHECK(v.empty()) ;
    }

    SUBCASE("(util::split) the string has not delimiter") {
        std::string str("abc") ;
        std::vector<std::string> expect{"abc"} ;
        CHECK_EQ(split(str, ","), expect) ;
    }

    SUBCASE("(util::split) normal") {
        std::string str("abc-def") ;
        std::vector<std::string> expect{"abc", "def"} ;
        CHECK_EQ(split(str, "-"), expect) ;
    }


    // util::remove_str
    SUBCASE("(util::remove_str) the string is empty") {
        CHECK_EQ(remove_str("", ','), "") ;
    }

    SUBCASE("(util::remove_str) normal") {
        CHECK_EQ(remove_str("AAXXBB", 'X'), "AABB") ;
    }


    // util::replace_all
    SUBCASE("(util::replace_all) There is no-target") {
        std::string str("AAA BBB CCC") ;
        CHECK_EQ(replace_all(str, "X", "Z"), "AAA BBB CCC") ;
    }

    SUBCASE("(util::replace_all) normal") {
        std::string str("AAA BBB CCC BBB") ;
        CHECK_EQ(replace_all(str, "BBB", "XXX"), "AAA XXX CCC XXX") ;
    }


    // util::a2A
    SUBCASE("(util::a2A) normal") {
        CHECK_EQ(a2A("Happy!!!"), "HAPPY!!!") ;
    }

    // util::A2a
    SUBCASE("(util::A2a) normal") {
        CHECK_EQ(A2a("HOGEEE!!!"), "hogeee!!!") ; 
    }


    // util::s_to_ws
    SUBCASE("(util::s_to_ws) The string is empty") {
        int returns[] = {10, 10} ;
        SET_RETURN_SEQ(MultiByteToWideChar, returns, 2) ;
        CHECK(s_to_ws(std::string()).empty()) ;
    }

    SUBCASE("(util::s_to_ws) Arguments Test") {
        std::string str("Hello") ;

        std::wstring expect(L"Hello") ;
        MultiByteToWideChar_dst = expect ;

        MultiByteToWideChar_fake.custom_fake = MultiByteToWideChar_custom_fake ;

        CHECK_EQ(s_to_ws(str), expect) ;
        CHECK_EQ(strncmp(MultiByteToWideChar_fake.arg2_val, str.c_str(), str.length()), 0) ;
        CHECK_EQ(MultiByteToWideChar_fake.arg3_val, str.size()) ;
    }

    SUBCASE("(util::s_to_ws) Exception Test: The output size is invalid") {
        MultiByteToWideChar_fake.return_val = -1 ;
        CHECK_THROWS_AS(s_to_ws("Hello"), std::logic_error) ;
    }

    SUBCASE("(util::s_to_ws) Exception Test: Failed Conversion") {
        int returns[2] = {10, -1} ;
        SET_RETURN_SEQ(MultiByteToWideChar, returns, 2) ;
        CHECK_THROWS_AS(s_to_ws("Hello"), std::logic_error) ;
    }

    // util::ws_to_s
    SUBCASE("(util::ws_to_s) The string is empty") {
        int returns[] = {10, 10} ;
        SET_RETURN_SEQ(WideCharToMultiByte, returns, 2) ;
        CHECK(ws_to_s(std::wstring()).empty()) ;
    }

    SUBCASE("(util::ws_to_s) Arguments Test") {
        std::wstring str(L"Hello") ;

        std::string expect("Hello") ;
        WideCharToMultiByte_dst = expect ;

        WideCharToMultiByte_fake.custom_fake = WideCharToMultiByte_custom_fake ;

        CHECK_EQ(ws_to_s(str), expect) ;
        CHECK_EQ(wcsncmp(WideCharToMultiByte_fake.arg2_val, str.c_str(), str.length()), 0) ;
        CHECK_EQ(WideCharToMultiByte_fake.arg3_val, str.size()) ;
    }

    SUBCASE("(util::ws_to_s) Exception Test: The output size is invalid") {
        WideCharToMultiByte_fake.return_val = -1 ; // invalid output size
        CHECK_THROWS_AS(ws_to_s(L"Hello"), std::logic_error) ;
    }

    SUBCASE("(util::ws_to_s) Exception Test: Conversion is failed") {
        // Size is good, but conversion is failed
        int returns[2] = {10, -1} ;
        SET_RETURN_SEQ(WideCharToMultiByte, returns, 2) ;
        CHECK_THROWS_AS(ws_to_s(L"Hello"), std::logic_error) ;
    }
}

#include "util/enable_compiler_warning.hpp"

