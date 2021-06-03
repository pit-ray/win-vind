#include <doctest.h>

#include "mode.cpp"

using namespace vind ;
using namespace vind::mode ;

TEST_CASE("(mode::set_global_mode, mode::get_global_mode) Check set-get") {
    CHECK_NOTHROW(set_global_mode(Mode::GUI_NORMAL)) ;
    CHECK_EQ(get_global_mode(), Mode::GUI_NORMAL) ;

    auto int_mode = static_cast<int>(Mode::GUI_VISUAL) ;
    CHECK_NOTHROW(set_global_mode(int_mode)) ;
    CHECK_EQ(get_global_mode(), int_mode) ;
}

TEST_CASE("(mode::get_global_index) Check i/o") {
    CHECK_NOTHROW(set_global_mode(Mode::GUI_NORMAL)) ;
    CHECK_EQ(get_global_index(), Mode::GUI_NORMAL & Mode::MASK_INDEX) ;
}

TEST_CASE("(mode::convert_to_prefix) Check i/o") {
    CHECK_EQ(to_prefix(Mode::INSERT), "i") ;
}

TEST_CASE("(mode::get_index_from_prefix) Check i/o") {
    CHECK_EQ(prefix_to_mode("i"), Mode::INSERT) ;
}

TEST_CASE("(mode::to_name) Check i/o") {
    CHECK_EQ(to_name(Mode::INSERT), "Insert") ;
    CHECK_EQ(to_name(static_cast<int>(Mode::INSERT)), "Insert") ;
}
