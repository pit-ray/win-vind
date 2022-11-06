#include <doctest/doctest.h>

#include "util/rect.hpp"

using namespace vind ;
using namespace vind::util ;

TEST_SUITE("util/rect") {
    TEST_CASE("(util::copy) I/O") {
        RECT rect1{100, 150, 200, 20} ;
        RECT rect2{0, 0, 0, 0} ;

        CHECK_NOTHROW(util::copy(rect2, rect1)) ;

        CHECK_EQ(rect2.left, 100) ;
        CHECK_EQ(rect2.top, 150) ;
        CHECK_EQ(rect2.right, 200) ;
        CHECK_EQ(rect2.bottom, 20) ;
    }

    TEST_CASE("(util::width, util::height) I/O") {
        LONG width = 128, height = 256 ;
        RECT rect{20, 45, 20 + width, 45 + height} ;

        CHECK_EQ(util::width(rect), width) ;
        CHECK_EQ(util::height(rect), height) ;
    }

    TEST_CASE("(util::center_x, util::center_y) I/O") {
        LONG half_width = 200, half_height = 400 ;
        LONG cx = 20, cy = 50 ;
        RECT rect = {
            cx - half_width,
            cy - half_height,
            cx + half_width,
            cy + half_height
        } ;
        CHECK_EQ(util::center_x(rect), cx) ;
        CHECK_EQ(util::center_y(rect), cy) ;
    }

    TEST_CASE("(util::l1_distance) L1 I/O") {
        LONG dx = 12, dy = 34 ;
        LONG hw = 234, hh = 514 ;

        LONG cx1 = 50, cy1 = 246 ;
        RECT r1{cx1 - hw, cy1 - hh, cx1 + hw, cy1 + hh} ;

        auto cx2 = cx1 + dx, cy2 = cy1 + dy ;
        RECT r2{cx2 - hw, cy2 - hh, cx2 + hw, cy2 + hh} ;

        CHECK_EQ(util::l1_distance(r1, r2), dx + dy) ;
    }

    TEST_CASE("(util::l2_distance, util::l2_distance_nosq) L2 I/O") {
        LONG dx = 3, dy = 4 ;
        LONG hw = 232, hh = 518 ;

        LONG cx1 = 52, cy1 = 246 ;
        RECT r1{cx1 - hw, cy1 - hh, cx1 + hw, cy1 + hh} ;

        auto cx2 = cx1 + dx, cy2 = cy1 + dy ;
        RECT r2{cx2 - hw, cy2 - hh, cx2 + hw, cy2 + hh} ;

        auto result_nosq = dx * dx + dy * dy ;
        auto result = std::sqrt(result_nosq) ;

        CHECK_EQ(util::l2_distance_nosq(r1, r2), result_nosq) ;
        CHECK_EQ(util::l2_distance_nosq(cx1, cy1, cx2, cy2), result_nosq) ;

        CHECK_EQ(util::l2_distance(r1, r2), result) ;
        CHECK_EQ(util::l2_distance(cx1, cy1, cx2, cy2), result) ;
    }

    TEST_CASE("(util::is_ouf_of_range) check out of") {
        RECT r1{20, 60, 300, 600} ;
        RECT r2{302, 50, 502, 700} ;
        CHECK(util::is_out_of_range(r1, r2)) ;
    }

    TEST_CASE("(util::is_fully_in_range) check in") {
        RECT r1{21, 61, 199, 129} ;
        RECT r2{20, 60, 200, 130} ;
        CHECK(util::is_fully_in_range(r1, r2)) ;
    }

    TEST_CASE("(util::is_intersect) check intersect") {
        RECT r1{20, 60, 200, 130} ;
        RECT r2{21, 50, 105, 150} ;
        CHECK(util::is_intersect(r1, r2)) ;
    }

    TEST_CASE("(util::is_equal) check equel") {
        RECT r{20, 60, 200, 130} ;
        CHECK(util::is_equal(r, r)) ;
    }

    TEST_CASE("(util::is_intersect) check intersect") {
        RECT r1{21, 61, 199, 129} ;
        RECT r2{20, 60, 200, 130} ;
        CHECK(util::is_bigger_than(r2, r1)) ;
    }
}
