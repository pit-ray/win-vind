#include <doctest.h>

#include "util/color.cpp"

#include <iostream>

using namespace vind ;
using namespace vind::util ;

TEST_CASE("util::hex2rgb io") {
    auto [r1, g1, b1] = hex2rgb("#") ;
    CHECK(r1 == 0) ;
    CHECK(g1 == 0) ;
    CHECK(b1 == 0) ;

    auto [r2, g2, b2] = hex2rgb("#ffee00") ;
    CHECK(r2 == 255) ;
    CHECK(g2 == 238) ;
    CHECK(b2 == 0) ;


    auto [r3, g3, b3] = hex2rgb("#fbe") ;
    CHECK(r3 == 0) ;
    CHECK(g3 == 0) ;
    CHECK(b3 == 0) ;

    auto [r4, g4, b4] = hex2rgb("#zx140c") ;
    CHECK(r4 == 0) ;
    CHECK(g4 == 20) ;
    CHECK(b4 == 12) ;
}

TEST_CASE("util::hex2COLORREF io") {
    CHECK(hex2COLORREF("#ee00ff") == 0xff00ee) ;
}

TEST_CASE("util::to_complementary_rgb io") {
    auto [r1, g1, b1] = to_complementary_rgb(255, 100, 50) ;
    CHECK(r1 == 50) ;
    CHECK(g1 == 205) ;
    CHECK(b1 == 255) ;

    auto [r2, g2, b2] = to_complementary_rgb(100, 150, 230) ;
    CHECK(r2 == 230) ;
    CHECK(g2 == 180) ;
    CHECK(b2 == 100) ;


    auto [r3, g3, b3] = to_complementary_rgb(RGBTuple{255, 100, 50}) ;
    CHECK(r3 == 50) ;
    CHECK(g3 == 205) ;
    CHECK(b3 == 255) ;

    auto [r4, g4, b4] = to_complementary_rgb(RGBTuple{100, 150, 230}) ;
    CHECK(r4 == 230) ;
    CHECK(g4 == 180) ;
    CHECK(b4 == 100) ;
}

TEST_CASE("util::to_complementary_COLORREF io") {
    CHECK(to_complementary_COLORREF(255, 100, 50)  == 0xffcd32) ;
    CHECK(to_complementary_COLORREF(100, 150, 230) == 0x64b4e6) ;

    CHECK(to_complementary_COLORREF(RGBTuple{255, 100, 50})  == 0xffcd32) ;
    CHECK(to_complementary_COLORREF(RGBTuple{100, 150, 230}) == 0x64b4e6) ;
}

TEST_CASE("util::to_gray io") {
    CHECK(to_gray(100, 100, 100) == 100) ;
}
