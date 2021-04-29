#include <doctest.h>

#include "util/string.cpp"

using namespace vind::util ;

// util::split
TEST_CASE("(util::split) delimiter is longer than the source string") {
    std::string str("aaa-bbb") ;
    std::vector<std::string> expect{"aaa-bbb"} ;
    CHECK_EQ(split(str, "--------------"), expect) ;
}

TEST_CASE("(util::split) the string is empty") {
    std::string str("") ;
    auto v = split(str, "") ;
    CHECK(v.empty()) ;
}

TEST_CASE("(util::split) the string has not delimiter") {
    std::string str("abc") ;
    std::vector<std::string> expect{"abc"} ;
    CHECK_EQ(split(str, ","), expect) ;
}

TEST_CASE("(util::split) normal") {
    std::string str("abc-def") ;
    std::vector<std::string> expect{"abc", "def"} ;
    CHECK_EQ(split(str, "-"), expect) ;
}


// util::remove_str
TEST_CASE("(util::remove_str) the string is empty") {
    CHECK_EQ(remove_str("", ','), "") ;
}

TEST_CASE("(util::remove_str) normal") {
    CHECK_EQ(remove_str("AAXXBB", 'X'), "AABB") ;
}


// util::replace_all
TEST_CASE("(util::replace_all) There is no-target") {
    std::string str("AAA BBB CCC") ;
    replace_all(str, "X", "Z") ;
    CHECK_EQ(str, "AAA BBB CCC") ;
}

TEST_CASE("(util::replace_all) normal") {
    std::string str("AAA BBB CCC BBB") ;
    replace_all(str, "BBB", "XXX") ;
    CHECK_EQ(str, "AAA XXX CCC XXX") ;
}


// util::a2A
TEST_CASE("(util::a2A) normal") {
    CHECK_EQ(a2A("Happy!!!"), "HAPPY!!!") ;
}

// util::A2a
TEST_CASE("(util::A2a) normal") {
    CHECK_EQ(A2a("HOGEEE!!!"), "hogeee!!!") ; 
}

// util::s_to_ws
TEST_CASE("(util::s_to_ws) The string is empty") {
    CHECK_EQ(s_to_ws(""), L"") ;
}

TEST_CASE("(util::s_to_ws) normal") {
    CHECK_EQ(s_to_ws("Hello"), L"Hello") ;
}


// util::ws_to_s
TEST_CASE("(util::ws_to_s) The string is empty") {
    CHECK_EQ(ws_to_s(L""), "") ;
}

TEST_CASE("(util::ws_to_s) normal") {
    CHECK_EQ(ws_to_s(L"Hello"), "Hello") ;
}
