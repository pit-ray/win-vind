#include <doctest.h>

#include "util/box_2d.cpp"

using namespace vind ;
using namespace vind::util ;


TEST_CASE("(Box2D) Constructor test") {
    LONG left = 200, top = 550, right = 400, bottom = 650 ;

    Box2D b0(left, top, right, bottom) ;
    Box2D b1(left, top, right, bottom) ;

    Point2D center(300, 600) ;
    LONG width = 200, height = 100 ;
    Box2D b2(center, width, height) ;

    RECT rect{left, top, right, bottom} ;
    Box2D b3(rect) ;

    RECT temp ;
    util::copy(temp, rect) ;
    Box2D b4(std::move(temp)) ;

    Box2D b5(10, 20, 234, 234) ;

    const Box2D b6(left, top, right, bottom) ;

    SUBCASE("Equal") {
        CHECK(b0 == b1) ;
        CHECK(b0 != b5) ;
    }

    SUBCASE("Basic") {
        CHECK(b1 == b2) ;
        CHECK(b1 == b3) ;
        CHECK(b1 == b4) ;
        CHECK(b2 == b3) ;
        CHECK(b2 == b4) ;
        CHECK(b3 == b4) ;
    }

    SUBCASE("Copy") {
        Box2D c(b1) ;
        CHECK(b1 == c) ;

        c = b5 ;
        CHECK(c == b5) ;
    }

    SUBCASE("Move") {
        Box2D b5_copy(b5) ;
        Box2D c(std::move(b5_copy)) ;
        CHECK(b5 == c) ;

        Box2D b0_copy(b0) ;
        c = std::move(b0_copy) ;
        CHECK(c == b1) ;
    }

    SUBCASE("Getter") {
        CHECK_EQ(b0.left(), left) ;
        CHECK_EQ(b0.top(), top) ;
        CHECK_EQ(b0.right(), right) ;
        CHECK_EQ(b0.bottom(), bottom) ;

        CHECK_EQ(b6.left(), left) ;
        CHECK_EQ(b6.top(), top) ;
        CHECK_EQ(b6.right(), right) ;
        CHECK_EQ(b6.bottom(), bottom) ;

        CHECK(b2.center() == center) ;
        CHECK_EQ(b2.center_x(), center.x()) ;
        CHECK_EQ(b2.center_y(), center.y()) ;

        CHECK_EQ(b2.width(), width) ;
        CHECK_EQ(b2.height(), height) ;
        CHECK_EQ(b2.area(), width * height) ;

        CHECK(util::is_equal(b3, rect)) ;
        CHECK(util::is_equal(b3.data(), rect)) ;
    }
}

TEST_CASE("(Box2D) operator value-based comparison") {
    Box2D b0(0, 20, 30, 40) ;
    Box2D b1(10, 20, 30, 40) ;
    Box2D b2(10, 30, 30, 40) ;
    Box2D b3(10, 30, 40, 40) ;
    Box2D b4(10, 30, 40, 50) ;

    CHECK(b0 < b1) ;
    CHECK(b0 < b2) ;
    CHECK(b0 < b3) ;
    CHECK(b0 < b4) ;
    CHECK(b1 < b2) ;
    CHECK(b1 < b3) ;
    CHECK(b1 < b4) ;
    CHECK(b2 < b3) ;
    CHECK(b2 < b4) ;
    CHECK(b3 < b4) ;

    CHECK(b0 <= b0) ;
    CHECK(b0 <= b2) ;
    CHECK(b0 <= b3) ;
    CHECK(b0 <= b4) ;
    CHECK(b1 <= b2) ;
    CHECK(b1 <= b3) ;
    CHECK(b1 <= b4) ;
    CHECK(b2 <= b3) ;
    CHECK(b2 <= b4) ;
    CHECK(b3 <= b4) ;

    CHECK(b1 < b4) ;
    CHECK(b2 < b4) ;
    CHECK(b3 < b4) ;
    CHECK(b2 < b3) ;
    CHECK(b1 < b3) ;
    CHECK(b1 < b2) ;
    CHECK(b0 < b4) ;
    CHECK(b0 < b3) ;
    CHECK(b0 < b2) ;
    CHECK(b0 < b1) ;

    CHECK(b1 <= b1) ;
    CHECK(b1 <= b4) ;
    CHECK(b2 <= b4) ;
    CHECK(b3 <= b4) ;
    CHECK(b2 <= b3) ;
    CHECK(b1 <= b3) ;
    CHECK(b1 <= b2) ;
    CHECK(b0 <= b4) ;
    CHECK(b0 <= b3) ;
    CHECK(b0 <= b2) ;
    CHECK(b0 <= b1) ;
}

TEST_CASE("(Box2D) size-based comparison is_same()") {
    Box2D b1(Point2D(20, 40), 400, 500) ;
    Box2D b2(Point2D(40, 50), 400, 500) ;
    CHECK(b1.is_same(b2)) ;
    CHECK(b2.is_same(b1)) ;
}

TEST_CASE("(Box2D) size-based comparison is_not_same()") {
    Box2D b1(Point2D(20, 40), 400, 600) ;
    Box2D b2(Point2D(40, 50), 300, 600) ;
    Box2D b3(Point2D(40, 50), 400, 500) ;
    CHECK(b1.is_not_same(b2)) ;
    CHECK(b1.is_not_same(b3)) ;
    CHECK(b2.is_not_same(b3)) ;
}

TEST_CASE("(Box2D) size-based comparison") {
    Box2D b1(Point2D(20, 40), 400, 500) ;
    Box2D b2(Point2D(40, 50), 500, 600) ;
    Box2D b3(Point2D(40, 50), 300, 700) ;
    Box2D b4(Point2D(40, 50), 200, 700) ;

    CHECK(b2.is_bigger_than(b1)) ;
    CHECK(b3.is_bigger_than(b1)) ;

    CHECK(b1.is_smaller_than(b2)) ;
    CHECK_FALSE(b3.is_smaller_than(b1)) ;

    CHECK(b2.is_bigger_equal(b1)) ;
    CHECK(b3.is_bigger_equal(b4)) ;
    CHECK(b3.is_bigger_equal(b1)) ;

    CHECK(b1.is_smaller_equal(b2)) ;
    CHECK(b4.is_smaller_equal(b3)) ;
    CHECK_FALSE(b3.is_smaller_equal(b1)) ;
}
