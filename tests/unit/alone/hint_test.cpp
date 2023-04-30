#include <doctest/doctest.h>

#include "util/hint.hpp"


TEST_SUITE("util/hint") {
    using namespace vind::util ;

    TEST_CASE("Hint Assignment (<= 26)") {
        std::size_t count = 26 ;
        std::vector<Hint> outputs ;
        assign_identifier_hints(count, outputs) ;

        SUBCASE("Keycode") {
            CHECK_EQ(outputs.size(), 26) ;
            CHECK_EQ(outputs[0].size(), 1) ;
            CHECK_EQ(outputs[9].size(), 1) ;
            CHECK_EQ(outputs[25].size(), 1) ;

            CHECK_EQ(outputs[0][0].to_code(), KEYCODE_A) ;
            CHECK_EQ(outputs[9][0].to_code(), KEYCODE_E) ;
            CHECK_EQ(outputs[25][0].to_code(), KEYCODE_J) ;
        }

        SUBCASE("String") {
            std::vector<std::string> strs ;
            convert_hints_to_strings(outputs, strs) ;

            CHECK_EQ(strs.size(), 26) ;
            CHECK_EQ(strs[0].size(), 1) ;
            CHECK_EQ(strs[9].size(), 1) ;
            CHECK_EQ(strs[25].size(), 1) ;

            CHECK_EQ(strs[0], "A") ;
            CHECK_EQ(strs[9], "E") ;
            CHECK_EQ(strs[25], "J") ;
        }
    }

    TEST_CASE("Hint Assignment (<= 676)") {
        std::size_t count = 676 ;
        std::vector<Hint> outputs ;
        assign_identifier_hints(count, outputs) ;

        SUBCASE("Keycode") {
            CHECK_EQ(outputs.size(), 676) ;

            CHECK_EQ(outputs[0].size(), 2) ;
            CHECK_EQ(outputs[36].size(), 2) ;
            CHECK_EQ(outputs[675].size(), 2) ;

            CHECK_EQ(outputs[0][0].to_code(), KEYCODE_A) ;
            CHECK_EQ(outputs[0][1].to_code(), KEYCODE_A) ;

            CHECK_EQ(outputs[36][0].to_code(), KEYCODE_S) ;
            CHECK_EQ(outputs[36][1].to_code(), KEYCODE_R) ;

            CHECK_EQ(outputs[675][0].to_code(), KEYCODE_J) ;
            CHECK_EQ(outputs[675][1].to_code(), KEYCODE_J) ;
        }

        SUBCASE("String") {
            std::vector<std::string> strs ;
            convert_hints_to_strings(outputs, strs) ;

            CHECK_EQ(strs.size(), 676) ;

            CHECK_EQ(strs[0].size(), 2) ;
            CHECK_EQ(strs[36].size(), 2) ;
            CHECK_EQ(strs[675].size(), 2) ;

            CHECK_EQ(strs[0], "AA") ;
            CHECK_EQ(strs[36], "SR") ;
            CHECK_EQ(strs[675], "JJ") ;
        }
    }

    TEST_CASE("Hint Assignment (<= 17576)") {
        std::size_t count = 17576 ;
        std::vector<Hint> outputs ;
        assign_identifier_hints(count, outputs) ;

        SUBCASE("Keycode") {
            CHECK_EQ(outputs.size(), 17576) ;

            CHECK_EQ(outputs[0].size(), 3) ;
            CHECK_EQ(outputs[711].size(), 3) ;
            CHECK_EQ(outputs[17575].size(), 3) ;

            CHECK_EQ(outputs[0][0].to_code(), KEYCODE_A) ;
            CHECK_EQ(outputs[0][1].to_code(), KEYCODE_A) ;
            CHECK_EQ(outputs[0][2].to_code(), KEYCODE_A) ;

            CHECK_EQ(outputs[721][0].to_code(), KEYCODE_S) ;
            CHECK_EQ(outputs[721][1].to_code(), KEYCODE_S) ;
            CHECK_EQ(outputs[721][2].to_code(), KEYCODE_C) ;

            CHECK_EQ(outputs[17575][0].to_code(), KEYCODE_J) ;
            CHECK_EQ(outputs[17575][1].to_code(), KEYCODE_J) ;
            CHECK_EQ(outputs[17575][2].to_code(), KEYCODE_J) ;
        }

        SUBCASE("String") {
            std::vector<std::string> strs ;
            convert_hints_to_strings(outputs, strs) ;

            CHECK_EQ(strs.size(), 17576) ;

            CHECK_EQ(strs[0].size(), 3) ;
            CHECK_EQ(strs[711].size(), 3) ;
            CHECK_EQ(strs[17575].size(), 3) ;

            CHECK_EQ(strs[0], "AAA") ;
            CHECK_EQ(strs[721], "SSC") ;
            CHECK_EQ(strs[17575], "JJJ") ;
        }
    }
}
