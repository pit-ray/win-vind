#include <doctest.h>

#include "core/keycodecvt.hpp"

#include "core/maptable.cpp"

#include <algorithm>


TEST_CASE("Map") {
    using namespace vind ;
    using namespace vind::core ;

    load_input_combination() ;

    Map a("a<s-h>j", "jblk", true) ;

    CHECK(a.is_map()) ;
    CHECK_FALSE(a.is_noremap()) ;

    Command expect_trigger {
        {KEYCODE_A},
        {KEYCODE_SHIFT, KEYCODE_H},
        {KEYCODE_J}
    } ;
    CHECK_EQ(a.trigger_command(), expect_trigger) ;
    CHECK_EQ(a.trigger_command_string(), "a<s-h>j") ;

    Command expect_target {
        {KEYCODE_J},
        {KEYCODE_B},
        {KEYCODE_L},
        {KEYCODE_K}
    } ;
    CHECK_EQ(a.target_command(), expect_target) ;
    CHECK_EQ(a.target_command_string(), "jblk") ;

    CHECK_FALSE(a.empty()) ;

    Map b("a<s-h>j", "jblk", true) ;
    Map c("j", "I", false) ;

    CHECK(a == b) ;
    CHECK(a != c) ;
    CHECK(b != c) ;
}

TEST_CASE("MapTable") {
    using namespace vind::core ;

    load_input_combination() ;

    auto& maptable = MapTable::get_instance() ;

    // Store as default
    Map a("a<s-h>j", "jblk", true) ;
    maptable.add_map("a<s-h>j", "jblk", Mode::GUI_NORMAL) ;

    Map b("hogehoge", "hogege", false) ;
    maptable.add_noremap("hogehoge", "hogege", Mode::GUI_NORMAL) ;

    Map c("unko", "unkokko", false) ;
    maptable.add(c, Mode::GUI_NORMAL) ;

    maptable.save_asdef() ;

    // Check if these maps were stored.
    CHECK_EQ(maptable.get("a<s-h>j", Mode::GUI_NORMAL), a) ;
    CHECK_EQ(maptable.get("hogehoge", Mode::GUI_NORMAL), b) ;
    CHECK_EQ(maptable.get("unko", Mode::GUI_NORMAL), c) ;

    auto noremaps = maptable.get_noremaps(Mode::GUI_NORMAL) ;
    CHECK_EQ(noremaps.size(), 2) ;
    CHECK(std::find(noremaps.begin(), noremaps.end(), b) != noremaps.end()) ;
    CHECK(std::find(noremaps.begin(), noremaps.end(), c) != noremaps.end()) ;

    auto maps = maptable.get_remaps(Mode::GUI_NORMAL) ;
    CHECK_EQ(maps.size(), 1) ;
    CHECK_EQ(maps[0], a) ;

    auto allmaps = maptable.get_allmaps(Mode::GUI_NORMAL) ;
    CHECK_EQ(allmaps.size(), 3) ;
    CHECK(std::find(allmaps.begin(), allmaps.end(), a) != allmaps.end()) ;
    CHECK(std::find(allmaps.begin(), allmaps.end(), b) != allmaps.end()) ;
    CHECK(std::find(allmaps.begin(), allmaps.end(), c) != allmaps.end()) ;


    // Add more maps.
    Map d("foo", "bar", false) ;
    Map e("unko", "anko", true) ;

    maptable.add(d, Mode::GUI_NORMAL) ;
    maptable.add(e, Mode::GUI_NORMAL) ;

    noremaps = maptable.get_noremaps(Mode::GUI_NORMAL) ;
    CHECK_EQ(noremaps.size(), 2) ;
    CHECK(std::find(noremaps.begin(), noremaps.end(), b) != noremaps.end()) ;
    CHECK(std::find(noremaps.begin(), noremaps.end(), d) != noremaps.end()) ;

    maps = maptable.get_remaps(Mode::GUI_NORMAL) ;
    CHECK_EQ(maps.size(), 2) ;
    CHECK(std::find(maps.begin(), maps.end(), a) != noremaps.end()) ;
    CHECK(std::find(maps.begin(), maps.end(), e) != noremaps.end()) ;

    allmaps = maptable.get_allmaps(Mode::GUI_NORMAL) ;
    CHECK_EQ(allmaps.size(), 4) ;
    CHECK(std::find(allmaps.begin(), allmaps.end(), b) != allmaps.end()) ;
    CHECK(std::find(allmaps.begin(), allmaps.end(), d) != allmaps.end()) ;
    CHECK(std::find(allmaps.begin(), allmaps.end(), a) != allmaps.end()) ;
    CHECK(std::find(allmaps.begin(), allmaps.end(), e) != allmaps.end()) ;


    // Reset maps to default.
    maptable.reset_todef() ;

    noremaps = maptable.get_noremaps(Mode::GUI_NORMAL) ;
    CHECK_EQ(noremaps.size(), 2) ;
    CHECK(std::find(noremaps.begin(), noremaps.end(), b) != noremaps.end()) ;
    CHECK(std::find(noremaps.begin(), noremaps.end(), c) != noremaps.end()) ;

    maps = maptable.get_remaps(Mode::GUI_NORMAL) ;
    CHECK_EQ(maps.size(), 1) ;
    CHECK_EQ(maps[0], a) ;

    allmaps = maptable.get_allmaps(Mode::GUI_NORMAL) ;
    CHECK_EQ(allmaps.size(), 3) ;
    CHECK(std::find(allmaps.begin(), allmaps.end(), a) != allmaps.end()) ;
    CHECK(std::find(allmaps.begin(), allmaps.end(), b) != allmaps.end()) ;
    CHECK(std::find(allmaps.begin(), allmaps.end(), c) != allmaps.end()) ;


    // Remove a map.
    maptable.remove("unko", Mode::GUI_NORMAL) ;
    CHECK_THROWS(maptable.get("unko", Mode::GUI_NORMAL)) ;

    noremaps = maptable.get_noremaps(Mode::GUI_NORMAL) ;
    CHECK_EQ(noremaps.size(), 1) ;
    CHECK_EQ(noremaps[0], b) ;

    maps = maptable.get_remaps(Mode::GUI_NORMAL) ;
    CHECK_EQ(maps.size(), 1) ;
    CHECK_EQ(maps[0], a) ;

    allmaps = maptable.get_allmaps(Mode::GUI_NORMAL) ;
    CHECK_EQ(allmaps.size(), 2) ;
    CHECK(std::find(allmaps.begin(), allmaps.end(), a) != allmaps.end()) ;
    CHECK(std::find(allmaps.begin(), allmaps.end(), b) != allmaps.end()) ;


    // Remove all maps.
    maptable.clear(Mode::GUI_NORMAL) ;

    noremaps = maptable.get_noremaps(Mode::GUI_NORMAL) ;
    CHECK(noremaps.empty()) ;

    maps = maptable.get_remaps(Mode::GUI_NORMAL) ;
    CHECK(maps.empty()) ;

    allmaps = maptable.get_allmaps(Mode::GUI_NORMAL) ;
    CHECK(allmaps.empty()) ;


    // Restore all maps to default one.
    maptable.reset_todef() ;

    noremaps = maptable.get_noremaps(Mode::GUI_NORMAL) ;
    CHECK_EQ(noremaps.size(), 2) ;
    CHECK(std::find(noremaps.begin(), noremaps.end(), b) != noremaps.end()) ;
    CHECK(std::find(noremaps.begin(), noremaps.end(), c) != noremaps.end()) ;

    maps = maptable.get_remaps(Mode::GUI_NORMAL) ;
    CHECK_EQ(maps.size(), 1) ;
    CHECK_EQ(maps[0], a) ;

    allmaps = maptable.get_allmaps(Mode::GUI_NORMAL) ;
    CHECK_EQ(allmaps.size(), 3) ;
    CHECK(std::find(allmaps.begin(), allmaps.end(), a) != allmaps.end()) ;
    CHECK(std::find(allmaps.begin(), allmaps.end(), b) != allmaps.end()) ;
    CHECK(std::find(allmaps.begin(), allmaps.end(), c) != allmaps.end()) ;
}
