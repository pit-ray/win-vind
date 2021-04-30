#include <doctest.h>

#include <vector>

#define _WINDOWS_
#define CP_UTF8 1

int test_return_vars[10] ;
std::size_t test_return_index = 0 ;

static int WideCharToMultiByte(int, int, const wchar_t*, int, char*, int, char*, bool*) {
    auto rv = test_return_vars[test_return_index] ;
    test_return_index ++ ;
    return rv ;
}
static int MultiByteToWideChar(int, int, const char*, int, wchar_t*, int) {
    auto rv = test_return_vars[test_return_index] ;
    test_return_index ++ ;
    return rv ;
}

#include "util/string.cpp"
using namespace vind::util ;

TEST_CASE("(util::s_to_ws) Exception Test: The output size is invalid") {
    test_return_vars[0] = -1 ; //invalid output size
    test_return_index = 0 ;
    CHECK_THROWS_AS(s_to_ws("Hello"), std::logic_error) ;
}

TEST_CASE("(util::ws_to_s) Exception Test: The output size is invalid") {
    test_return_vars[0] = -1 ; // invalid output size
    test_return_index = 0 ;
    CHECK_THROWS_AS(ws_to_s(L"Hello"), std::logic_error) ;
}


TEST_CASE("(util::s_to_ws) Exception Test: Failed Conversion") {
    test_return_vars[0] = 10 ; // Size is good
    test_return_vars[1] = -1 ; // but, conversion is failed
    test_return_index = 0 ;
    CHECK_THROWS_AS(s_to_ws("Hello"), std::logic_error) ;
}

TEST_CASE("(util::ws_to_s) Exception Test: Conversion is failed") {
    test_return_vars[0] = 10 ; // Size is good
    test_return_vars[1] = -1 ; // but, conversion is failed
    test_return_index = 0 ;
    CHECK_THROWS_AS(ws_to_s(L"Hello"), std::logic_error) ;
}
