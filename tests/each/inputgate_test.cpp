#include <doctest.h>

#include "core/inputgate.cpp"

using namespace vind ;
using namespace vind::core ;

namespace vind
{
    namespace bind
    {
        BindedFunc::SPtr ref_global_funcs_bynames(const std::string&) {
            return nullptr ;
        }
    }
}


TEST_CASE("inputgate::solve_recursive_key2keyset_mapping") {
    Key2KeysetMap table{} ;

    SUBCASE("One recursive mapping") {
        // A -> {C, D}
        table[KEYCODE_A] = {KEYCODE_C, KEYCODE_D} ; 

        // D -> {G, E}
        table[KEYCODE_D] = {KEYCODE_E, KEYCODE_G} ; 

        solve_recursive_key2keyset_mapping(table) ;

        // A -> {C, E, G}
        KeySet expect_a = {KEYCODE_C, KEYCODE_E, KEYCODE_G} ;
        CHECK_EQ(table[KEYCODE_A], expect_a) ;

        // D -> {G, E}
        KeySet expect_d = {KEYCODE_E, KEYCODE_G} ;
        CHECK_EQ(table[KEYCODE_D], expect_d) ;
    }

    SUBCASE("Three recursive mapping") {
        // A -> {X, P}
        table[KEYCODE_A] = {KEYCODE_X, KEYCODE_P} ; 

        // X -> {H, J, Z}
        table[KEYCODE_X] = {KEYCODE_H, KEYCODE_J, KEYCODE_Z} ;

        // Z -> {G, E, I}
        table[KEYCODE_Z] = {KEYCODE_E, KEYCODE_G, KEYCODE_I} ;

        solve_recursive_key2keyset_mapping(table) ;

        // A -> {H, J, E, G, I, P}
        KeySet expect_a = {
            KEYCODE_E, KEYCODE_G, KEYCODE_H, KEYCODE_I, KEYCODE_J, KEYCODE_P
        } ;
        CHECK_EQ(table[KEYCODE_A], expect_a) ;

        // X -> {H, J, E, G, I}
        KeySet expect_x = {
            KEYCODE_E, KEYCODE_G, KEYCODE_H, KEYCODE_I, KEYCODE_J
        } ;
        CHECK_EQ(table[KEYCODE_X], expect_x) ;

        KeySet expect_z = {KEYCODE_E, KEYCODE_G, KEYCODE_I} ;
        CHECK_EQ(table[KEYCODE_Z], expect_z) ;
    }

    SUBCASE("Detection of mapping to itself") {
        // A -> {X, P}
        table[KEYCODE_A] = {KEYCODE_X, KEYCODE_P} ; 

        // P -> J
        table[KEYCODE_P] = {KEYCODE_J} ;

        // J -> {A, B}
        table[KEYCODE_J] = {KEYCODE_A, KEYCODE_B} ;

        solve_recursive_key2keyset_mapping(table) ;

        CHECK(table[KEYCODE_A].empty()) ;

        KeySet expect = {KEYCODE_A, KEYCODE_B} ;
        CHECK_EQ(table[KEYCODE_P], expect) ;
        CHECK_EQ(table[KEYCODE_J], expect) ;
    }
}


TEST_CASE("inputgate::replace_command_with_key2keyset") {
    Command cmd {
        {KEYCODE_SHIFT, KEYCODE_A},
        {KEYCODE_CTRL, KEYCODE_X}
    } ;

    Key2KeysetMap table ;

    table[KEYCODE_X] = {KEYCODE_J, KEYCODE_G} ;


    auto result = replace_command_with_key2keyset(cmd, table) ;

    Command expect {
        {KEYCODE_SHIFT, KEYCODE_A},
        {KEYCODE_CTRL, KEYCODE_G, KEYCODE_J}
    } ;
    CHECK_EQ(result, expect) ;
}


TEST_CASE("inputgate::solve_recursive_cmd2cmd_mapping") {
    // define cmd2cmd mappings
    Map map1("g<s-h>h", "ty<c-j>", true) ;
    Map map2("t", "z<c-h><c-j>", true) ;
    Map map3("<c-j>", "hhg", true) ;

    std::unordered_map<std::size_t, Map> cmd2cmd {
        {map1.in_hash(), map1},
        {map2.in_hash(), map2},
        {map3.in_hash(), map3}
    } ;

    // define key2keyset mappings
    Key2KeysetMap key2keyset ;
    key2keyset[KEYCODE_H] = {KEYCODE_SHIFT, KEYCODE_Z} ;

    auto result = solve_recursive_cmd2cmd_mapping(cmd2cmd, key2keyset) ;

    // g<s-h>h -> z<c-s-z><s-z><s-z>gy<s-z><s-z>g
    Command expect1 {
        {KEYCODE_Z},
        {KEYCODE_LSHIFT, KEYCODE_LCTRL, KEYCODE_Z},
        {KEYCODE_LSHIFT, KEYCODE_Z},
        {KEYCODE_LSHIFT, KEYCODE_Z},
        {KEYCODE_G},
        {KEYCODE_Y},
        {KEYCODE_LSHIFT, KEYCODE_Z},
        {KEYCODE_LSHIFT, KEYCODE_Z},
        {KEYCODE_G}
    } ;
    CHECK_EQ(result[map1.in_hash()], expect1) ;

    // t -> z<c-s-z><s-z><s-z>g
    Command expect2 {
        {KEYCODE_Z},
        {KEYCODE_LSHIFT, KEYCODE_LCTRL, KEYCODE_Z},
        {KEYCODE_LSHIFT, KEYCODE_Z},
        {KEYCODE_LSHIFT, KEYCODE_Z},
        {KEYCODE_G}
    } ;
    CHECK_EQ(result[map2.in_hash()], expect2) ;

    // <c-j> -> <s-z><s-z>g
    Command expect3 {
        {KEYCODE_LSHIFT, KEYCODE_Z},
        {KEYCODE_LSHIFT, KEYCODE_Z},
        {KEYCODE_G}
    } ;
    CHECK_EQ(result[map3.in_hash()], expect3) ;
}
