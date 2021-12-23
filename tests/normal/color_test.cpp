#include <doctest.h>

#include "util/color.cpp"

using namespace vind::util ;

// util::hex2rgb
TEST_SUITE("util/color") {
    TEST_CASE("(util::hex2rgb) with header") {
        auto [r, g, b] = hex2rgb("#") ;
        CHECK_EQ(r, 0) ;
        CHECK_EQ(g, 0) ;
        CHECK_EQ(b, 0) ;
    }

    TEST_CASE("(util::hex2rgb) normal") {
        auto [r, g, b] = hex2rgb("#ffee00") ;
        CHECK_EQ(r, 255) ;
        CHECK_EQ(g, 238) ;
        CHECK_EQ(b, 0) ;
    }

    TEST_CASE("(util::hex2rgb) invalid length") {
        auto [r, g, b] = hex2rgb("#fbe") ;
        CHECK_EQ(r, 0) ;
        CHECK_EQ(g, 0) ;
        CHECK_EQ(b, 0) ;
    }

    TEST_CASE("(util::hex2rgb) invalid characters") {
        auto [r, g, b] = hex2rgb("#zx140c") ;
        CHECK_EQ(r, 0) ;
        CHECK_EQ(g, 20) ;
        CHECK_EQ(b, 12) ;
    }

    TEST_CASE("(util::hex2COLORREF) normal") {
        CHECK_EQ(hex2COLORREF("#ee00ff"), 0xff00ee) ;
    }


    // util::to_complementary_rgb
    TEST_CASE("(util::to_complementary_rgb) normal1 with (r, g, b)") {
        auto [r, g, b] = to_complementary_rgb(255, 100, 50) ;
        CHECK_EQ(r, 50) ;
        CHECK_EQ(g, 205) ;
        CHECK_EQ(b, 255) ;
    }
    TEST_CASE("(util::to_complementary_rgb) normal1 with RGBTuple{r, g, b}") {
        auto [r, g, b] = to_complementary_rgb(RGBTuple{255, 100, 50}) ;
        CHECK_EQ(r, 50) ;
        CHECK_EQ(g, 205) ;
        CHECK_EQ(b, 255) ;
    }

    TEST_CASE("(util::to_complementary_rgb) normal2 with (r, g, b)") {
        auto [r, g, b] = to_complementary_rgb(100, 150, 230) ;
        CHECK_EQ(r, 230) ;
        CHECK_EQ(g, 180) ;
        CHECK_EQ(b, 100) ;
    }
    TEST_CASE("(util::to_complementary_rgb) normal2 with RGBTuple{r, g, b}") {
        auto [r, g, b] = to_complementary_rgb(RGBTuple{100, 150, 230}) ;
        CHECK_EQ(r, 230) ;
        CHECK_EQ(g, 180) ;
        CHECK_EQ(b, 100) ;
    }


    // util::to_complementary_COLORREF
    TEST_CASE("(util::to_complementary_COLORREF) normal1 with (r, g, b)") {
        CHECK_EQ(to_complementary_COLORREF(255, 100, 50) , 0xffcd32) ;
    }
    TEST_CASE("(util::to_complementary_COLORREF) normal1 with RGBTuple{r, g, b}") {
        CHECK_EQ(to_complementary_COLORREF(RGBTuple{255, 100, 50}) , 0xffcd32) ;
    }

    TEST_CASE("(util::to_complementary_COLORREF) normal2 with (r, g, b)") {
        CHECK_EQ(to_complementary_COLORREF(100, 150, 230), 0x64b4e6) ;
    }
    TEST_CASE("(util::to_complementary_COLORREF) normal2 with RGBTuple{r, g, b}") {
        CHECK_EQ(to_complementary_COLORREF(RGBTuple{100, 150, 230}), 0x64b4e6) ;
    }

    TEST_CASE("(util::to_gray) normal") {
        CHECK_EQ(to_gray(100, 100, 100), 100) ;
    }
}
