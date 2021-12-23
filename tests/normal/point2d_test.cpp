#include <doctest.h>

#include "util/point_2d.hpp"

#include "util/rect.hpp"

using namespace vind::util ;

TEST_SUITE("util/point_2d") {
    TEST_CASE("constructor and equal test") {
        Point2D p1(static_cast<LONG>(20), static_cast<LONG>(50)) ;

        Point2D p2(static_cast<int>(20), static_cast<int>(50)) ;

        POINT base{20, 50} ;
        Point2D p3(base) ;

        Point2D p4(std::move(base)) ;

        CHECK(p1 == p2) ;
        CHECK(p1 == p3) ;
        CHECK(p1 == p4) ;
        CHECK(p2 == p3) ;
        CHECK(p3 == p4) ;
    }

    TEST_CASE("not equal") {
        Point2D p1(40, 50) ;
        Point2D p2(40, 52) ;

        CHECK(p1 != p2) ;
        CHECK_FALSE(p1 == p2) ;
    }

    TEST_CASE("getter test") {
        Point2D p(40, 50) ;
        POINT tp{40, 50} ;

        const auto cp = p ;

        CHECK_EQ(p.x(), 40) ;
        CHECK_EQ(p.y(), 50) ;

        CHECK_EQ(p.data().x, tp.x) ;
        CHECK_EQ(p.data().y, tp.y) ;

        CHECK_EQ(cp.data().x, tp.x) ;
        CHECK_EQ(cp.data().y, tp.y) ;

        POINT pp = p ;
        CHECK_EQ(pp.x, tp.x) ;
        CHECK_EQ(pp.y, tp.y) ;
    }

    TEST_CASE("compare test") {
        Point2D p1(40, 50) ;
        Point2D p2(30, 60) ;
        Point2D p3(20, 50) ;

        CHECK(p2 > p1) ;
        CHECK(p1 > p3) ;
        CHECK(p2 >= p1) ;
        CHECK(p1 >= p3) ;

        CHECK(p1 < p2) ;
        CHECK(p3 < p1) ;
        CHECK(p1 <= p2) ;
        CHECK(p3 <= p1) ;

        CHECK(p1 >= p1) ;
        CHECK(p2 <= p2) ;
    }
}
