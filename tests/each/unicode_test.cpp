#include <doctest.h>

#include "util/unicode.cpp"

#include <iostream>


TEST_SUITE("util/unicode") {
    using namespace vind::util ;

    TEST_CASE("break UTF-8 character") {
        std::string s1 = u8"😊" ;
        std::string s2 = u8"ポ" ;
        std::string s3 = u8"ボ" ;
        std::string s4 = u8"𠮷野家" ;

        std::u32string e1 = U"\U0001F60A" ;
        std::u32string e2 = U"\u30DD" ;
        std::u32string e3 = U"\u30DB\u3099" ;
        std::u32string e4 = U"\U00020BB7\u91CE\u5BB6" ;

        SUBCASE("break_unicode_grapheme") {
            auto o1 = break_unicode_grapheme(s1) ;
            CHECK_EQ(o1.size(), 1) ;
            CHECK_EQ(o1, e1) ;

            auto o2 = break_unicode_grapheme(s2) ;
            CHECK_EQ(o2.size(), 1) ;
            CHECK_EQ(o2, e2) ;

            auto o3 = break_unicode_grapheme(s3) ;
            CHECK_EQ(o3.size(), 1) ;
            CHECK_EQ(o3, e3.substr(0, 1)) ;

            auto o4 = break_unicode_grapheme(s4) ;
            CHECK_EQ(o4.size(), 3) ;
            CHECK_EQ(o4, e4) ;
        }

        SUBCASE("break_unicode_codepoint") {
            auto o1 = break_unicode_codepoint(s1) ;
            CHECK_EQ(o1.size(), 1) ;
            CHECK_EQ(o1, e1) ;

            auto o2 = break_unicode_codepoint(s2) ;
            CHECK_EQ(o2.size(), 1) ;
            CHECK_EQ(o2, e2) ;

            auto o3 = break_unicode_codepoint(s3) ;
            CHECK_EQ(o3.size(), 2) ;
            CHECK_EQ(o3, e3) ;

            auto o4 = break_unicode_codepoint(s4) ;
            CHECK_EQ(o4.size(), 3) ;
            CHECK_EQ(o4, e4) ;
        }
    }
}
