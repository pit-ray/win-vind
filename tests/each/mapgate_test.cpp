#include <doctest.h>

#include "core/mapgate.cpp"

using namespace vind ;
using namespace vind::core ;

namespace vind
{
    namespace bind
    {
        const auto& all_global_binded_funcs() {
            static std::vector<BindedFunc::SPtr> tmp {
            } ;
            return tmp ;
        }
    }
}


TEST_CASE("mapgate::solve_recursive_key2keyset_mapping") {
    Key2KeysetMap table{} ;
    load_input_combination() ;

    SUBCASE("One recursive mapping") {
        // A -> {C, D}
        KeyUnorderedSet target_a = {KEYCODE_C, KEYCODE_D} ; 
        table[KEYCODE_A] = target_a ;

        // D -> {G, E}
        KeyUnorderedSet target_d = {KEYCODE_E, KEYCODE_G} ;
        table[KEYCODE_D] = target_d ;

        solve_recursive_key2keyset_mapping(table) ;

        // A -> {C, E, G}
        KeyUnorderedSet expect_a = {KEYCODE_C, KEYCODE_E, KEYCODE_G} ;
        CHECK_EQ(table[KEYCODE_A], expect_a) ;

        // D -> {G, E}
        CHECK_EQ(table[KEYCODE_D], target_d) ;
    }

    SUBCASE("Three recursive mapping") {
        // A -> {X, P}
        KeyUnorderedSet target_a = {KEYCODE_X, KEYCODE_P} ; 
        table[KEYCODE_A] = target_a ;

        // X -> {H, J, Z}
        KeyUnorderedSet target_x = {KEYCODE_H, KEYCODE_J, KEYCODE_Z} ;
        table[KEYCODE_X] = target_x ;

        // Z -> {G, E, I}
        KeyUnorderedSet target_z = {KEYCODE_E, KEYCODE_G, KEYCODE_I} ;
        table[KEYCODE_Z] = target_z ;

        solve_recursive_key2keyset_mapping(table) ;

        // A -> {H, J, E, G, I, P}
        KeyUnorderedSet expect_a = {
            KEYCODE_H, KEYCODE_J, KEYCODE_E, KEYCODE_G, KEYCODE_I, KEYCODE_P
        } ;
        CHECK_EQ(table[KEYCODE_A], expect_a) ;

        // X -> {H, J, E, G, I}
        KeyUnorderedSet expect_x = {
            KEYCODE_H, KEYCODE_J, KEYCODE_E, KEYCODE_G, KEYCODE_I
        } ;
        CHECK_EQ(table[KEYCODE_X], expect_x) ;

        CHECK_EQ(table[KEYCODE_Z], target_z) ;
    }

    SUBCASE("Detection of mapping to itself") {
        // A -> {X, P}
        KeyUnorderedSet target_a = {KEYCODE_X, KEYCODE_P} ; 
        table[KEYCODE_A] = target_a ;

        // P -> J
        KeyUnorderedSet target_p = {KEYCODE_J} ;
        table[KEYCODE_P] = target_p ;

        // J -> {A, B}
        KeyUnorderedSet target_j = {KEYCODE_A, KEYCODE_B} ;
        table[KEYCODE_J] = target_j ;

        solve_recursive_key2keyset_mapping(table) ;

        CHECK(table[KEYCODE_A].empty()) ;
        CHECK_EQ(table[KEYCODE_P], target_j) ;
        CHECK_EQ(table[KEYCODE_J], target_j) ;
    }
}


TEST_CASE("mapgate::replace_command_with_key2keyset") {
}


TEST_CASE("mapgate::solve_recursive_cmd2cmd_mapping") {
}
