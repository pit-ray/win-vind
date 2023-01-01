#include <doctest/doctest.h>

#include "util/math.hpp"

using namespace vind::util ;

TEST_SUITE("util/math") {
    // pow_i
    TEST_CASE("(math::pow_i) exp is zero") {
        CHECK_EQ(pow_i(2, 0), 1) ;
    }
    TEST_CASE("(math::pow_i) positive base") {
        CHECK_EQ(pow_i(2, 2),  4) ;
    }
    TEST_CASE("(math::pow_i) negative base") {
        CHECK_EQ(pow_i(-2, 3), -8) ;
    }

    // pow_f
    TEST_CASE("(math::pow_f) exp is zero") {
        CHECK_EQ(pow_f(2.0f, 0), doctest::Approx(1.0f)) ;
    }
    TEST_CASE("(math::pow_f) positive base and positive exp") {
        CHECK_EQ(pow_f(2.2f, 2), doctest::Approx(4.84f)) ;
    }
    TEST_CASE("(math::pow_f) negative base and positive exp") {
        CHECK_EQ(pow_f(-2.0f, 3), doctest::Approx(-8.0f)) ;
    }
    TEST_CASE("(math::pow_f) positive base and negative exp") {
        CHECK_EQ(pow_f(2.0f, -2), doctest::Approx(0.25f)) ;
    }
    TEST_CASE("(math::pow_f) negative base and negative exp") {
        CHECK_EQ(pow_f(-2.0f, -3), doctest::Approx(-0.125f)) ;
    }

    // pow_d
    TEST_CASE("(math::pow_d) exp is zero") {
        CHECK_EQ(pow_d(2.0, 0), doctest::Approx(1.0)) ;
    }
    TEST_CASE("(math::pow_d) positive base and positive exp") {
        CHECK_EQ(pow_d(2.2, 2), doctest::Approx(4.84)) ;
    }
    TEST_CASE("(math::pow_d) negative base and positive exp") {
        CHECK_EQ(pow_d(-2.0, 3), doctest::Approx(-8.0)) ;
    }
    TEST_CASE("(math::pow_d) positive base and negative exp") {
        CHECK_EQ(pow_d(2.0, -2), doctest::Approx(0.25)) ;
    }
    TEST_CASE("(math::pow_d) negative base and negative exp") {
        CHECK_EQ(pow_d(-2.0, -3), doctest::Approx(-0.125)) ;
    }
}
