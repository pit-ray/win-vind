#include <doctest.h>

#include <cstring>
#include <string>

namespace string_exception_test
{
    constexpr int CP_UTF8 = 1 ;

    int test_return_vars[10] ;
    std::size_t test_return_index = 0 ;

    std::string MultiByteToWideChar_src{} ;
    std::wstring MultiByteToWideChar_dst{} ;
    int MultiByteToWideChar_size = 0 ;
    int MultiByteToWideChar(int, int, const char* src, int size, wchar_t* dst, int) {
        if(dst != NULL) {
            MultiByteToWideChar_src = src ;
            MultiByteToWideChar_size = size ;
            std::wcscpy(dst, MultiByteToWideChar_dst.c_str()) ;
        }
        auto rv = test_return_vars[test_return_index] ;
        test_return_index ++ ;
        return rv ;
    }

    std::wstring WideCharToMultiByte_src{} ;
    std::string WideCharToMultiByte_dst{} ;
    int WideCharToMultiByte_size = 0 ;
    int WideCharToMultiByte(int, int, const wchar_t* src, int size, char* dst, int, char*, bool*) {
        if(dst != NULL) {
            WideCharToMultiByte_src = src ;
            WideCharToMultiByte_size = size ;
            std::strcpy(dst, WideCharToMultiByte_dst.c_str()) ;
        }
        auto rv = test_return_vars[test_return_index] ;
        test_return_index ++ ;
        return rv ;
    }

    void reset_stub() {
        test_return_index = 0 ;
    }
}

namespace 
{
    using namespace string_exception_test ;
}
#define _WINDOWS_
#include "util/string.cpp"
#undef _WINDOWS_

using namespace vind::util ;

namespace string_exception_test
{
    TEST_CASE("Test Set:") {
        reset_stub() ;

        // util::split
        SUBCASE("(util::split) delimiter is longer than the source string") {
            std::string str("aaa-bbb") ;
            std::vector<std::string> expect{"aaa-bbb"} ;
            CHECK_EQ(split(str, "--------------"), expect) ;
        }

        SUBCASE("(util::split) the string is empty") {
            std::string str("") ;
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
            replace_all(str, "X", "Z") ;
            CHECK_EQ(str, "AAA BBB CCC") ;
        }

        SUBCASE("(util::replace_all) normal") {
            std::string str("AAA BBB CCC BBB") ;
            replace_all(str, "BBB", "XXX") ;
            CHECK_EQ(str, "AAA XXX CCC XXX") ;
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
            test_return_vars[0] = 10 ;
            test_return_vars[1] = 10 ;
            CHECK(s_to_ws(std::string()).empty()) ;
        }

        SUBCASE("(util::s_to_ws) Arguments Test") {
            std::string str("Hello") ;

            std::wstring expect(L"Hello") ;
            MultiByteToWideChar_dst = expect ;

            test_return_vars[0] = static_cast<int>(expect.size()) ;
            test_return_vars[1] = static_cast<int>(expect.size()) ;

            CHECK_EQ(s_to_ws(str), expect) ;
            CHECK_EQ(MultiByteToWideChar_src, str) ;
            CHECK_EQ(MultiByteToWideChar_size, str.size()) ;
        }

        SUBCASE("(util::s_to_ws) Exception Test: The output size is invalid") {
            test_return_vars[0] = -1 ; //invalid output size
            CHECK_THROWS_AS(s_to_ws("Hello"), std::logic_error) ;
        }

        SUBCASE("(util::s_to_ws) Exception Test: Failed Conversion") {
            test_return_vars[0] = 10 ; // Size is good
            test_return_vars[1] = -1 ; // but, conversion is failed
            CHECK_THROWS_AS(s_to_ws("Hello"), std::logic_error) ;
        }


        // util::ws_to_s
        SUBCASE("(util::ws_to_s) The string is empty") {
            test_return_vars[0] = 10 ;
            test_return_vars[1] = 10 ;
            CHECK(ws_to_s(std::wstring()).empty()) ;
        }

        SUBCASE("(util::ws_to_s) Arguments Test") {
            std::wstring str(L"Hello") ;

            std::string expect("Hello") ;
            WideCharToMultiByte_dst = expect ;

            test_return_vars[0] = static_cast<int>(expect.size()) ;
            test_return_vars[1] = static_cast<int>(expect.size()) ;

            CHECK_EQ(ws_to_s(str), expect) ;
            CHECK_EQ(WideCharToMultiByte_src, str) ;
            CHECK_EQ(WideCharToMultiByte_size, str.size()) ;
        }

        SUBCASE("(util::ws_to_s) Exception Test: The output size is invalid") {
            test_return_vars[0] = -1 ; // invalid output size
            CHECK_THROWS_AS(ws_to_s(L"Hello"), std::logic_error) ;
        }

        SUBCASE("(util::ws_to_s) Exception Test: Conversion is failed") {
            test_return_vars[0] = 10 ; // Size is good
            test_return_vars[1] = -1 ; // but, conversion is failed
            CHECK_THROWS_AS(ws_to_s(L"Hello"), std::logic_error) ;
        }
    }
}
