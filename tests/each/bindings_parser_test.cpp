#include <doctest.h>

#include "core/bindings_parser.hpp"

#include "core/keycode_def.hpp"

#include <iostream>


TEST_SUITE("core/bindings_parser") {
    using namespace vind::core ;

    TEST_CASE("parse_pure_one_character_command") {
        auto ks1 = parse_pure_one_character_command('A') ;
        KeySet expect1 = {KEYCODE_SHIFT, KEYCODE_A} ;
        CHECK_EQ(ks1, expect1) ;

        auto ks2 = parse_pure_one_character_command('x') ;
        KeySet expect2 = {KEYCODE_X} ;
        CHECK_EQ(ks2, expect2) ;
    }

}
