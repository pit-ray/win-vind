#include <doctest.h>

#include "core/cmdparser.hpp"
#include "core/keycodedef.hpp"

#include <iostream>


TEST_SUITE("core/bindings_parser") {
    using namespace vind::core ;

    TEST_CASE("parse_ascii_command") {
        auto ks1 = parse_ascii_command('A') ;
        KeySet expect1 = {KEYCODE_SHIFT, KEYCODE_A} ;
        CHECK_EQ(ks1, expect1) ;

        auto ks2 = parse_ascii_command('x') ;
        KeySet expect2 = {KEYCODE_X} ;
        CHECK_EQ(ks2, expect2) ;

        CHECK_THROWS(parse_ascii_command(0)) ;
    }

    TEST_CASE("parse_combined_command") {
        auto ks1 = parse_combined_command("s-c-h-j") ;
        KeySet expect1 = {KEYCODE_SHIFT, KEYCODE_C, KEYCODE_H, KEYCODE_J} ;
        CHECK_EQ(ks1, expect1) ;

        auto ks2 = parse_combined_command("shift-ctrl-X-b-a") ;
        KeySet expect2 = {KEYCODE_CTRL, KEYCODE_SHIFT, KEYCODE_A, KEYCODE_B, KEYCODE_X} ;
        CHECK_EQ(ks2, expect2) ;

        auto ks3 = parse_combined_command("ctrl-p-N-alt") ;
        KeySet expect3 = {KEYCODE_CTRL, KEYCODE_ALT, KEYCODE_SHIFT, KEYCODE_N, KEYCODE_P} ;
        CHECK_EQ(ks3, expect3) ;

        auto ks4 = parse_combined_command("a-a") ;
        KeySet expect4 = {KEYCODE_ALT, KEYCODE_A} ;
        CHECK_EQ(ks4, expect4) ;

        CHECK_THROWS(parse_combined_command("j-b")) ;
        CHECK_THROWS(parse_combined_command("shift-fjakls")) ;
    }

    TEST_CASE("parse_command") {
        auto cmd1 = parse_command("<s-h>jH<alt-b><m-m>") ;
        Command expect1 {
            {KEYCODE_SHIFT, KEYCODE_H},
            {KEYCODE_J},
            {KEYCODE_SHIFT, KEYCODE_H},
            {KEYCODE_ALT, KEYCODE_B},
            {KEYCODE_ALT, KEYCODE_M}
        } ;
        CHECK_EQ(cmd1, expect1) ;

        auto cmd2 = parse_command("<Esc-Left>") ;
        Command expect2 {
            {KEYCODE_ESC, KEYCODE_LEFT}
        } ;
        CHECK_EQ(cmd2, expect2) ;

        auto cmd3 = parse_command("!<any>") ;

        KeySet ks3{} ;
        KeyCode k3('!') ;

        if(auto shift = get_shift_keycode('!')) {
            ks3.emplace_back(shift) ;
        }
        ks3.push_back(k3) ;

        Command expect3 {
            ks3,
            {KEYCODE_OPTIONAL}
        } ;

        CHECK_EQ(cmd3, expect3) ;

        CHECK_THROWS(parse_command("b<shift-j><a-jmh")) ;
    }
}
