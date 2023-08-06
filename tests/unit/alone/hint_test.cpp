#include <doctest/doctest.h>

#include "core/hintassign.hpp"


TEST_SUITE("core/hintassign") {
    using namespace vind::core;

    TEST_CASE("Hint Assignment (<= 26)") {
        std::size_t count = 26 ;
        std::vector<Hint> outputs ;
        std::vector<std::string> labels ;
        assign_identifier_hints(count, outputs, labels) ;

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
            CHECK_EQ(labels.size(), 26) ;
            CHECK_EQ(labels[0].size(), 1) ;
            CHECK_EQ(labels[9].size(), 1) ;
            CHECK_EQ(labels[25].size(), 1) ;

            CHECK_EQ(labels[0], "A") ;
            CHECK_EQ(labels[9], "E") ;
            CHECK_EQ(labels[25], "J") ;
        }
    }

    TEST_CASE("Hint Assignment (<= 676)") {
        std::size_t count = 676 ;
        std::vector<Hint> outputs ;
        std::vector<std::string> labels ;
        assign_identifier_hints(count, outputs, labels) ;

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
            CHECK_EQ(labels.size(), 676) ;

            CHECK_EQ(labels[0].size(), 2) ;
            CHECK_EQ(labels[36].size(), 2) ;
            CHECK_EQ(labels[675].size(), 2) ;

            CHECK_EQ(labels[0], "AA") ;
            CHECK_EQ(labels[36], "SR") ;
            CHECK_EQ(labels[675], "JJ") ;
        }
    }

    TEST_CASE("Hint Assignment (<= 17576)") {
        std::size_t count = 17576 ;
        std::vector<Hint> outputs ;
        std::vector<std::string> labels ;
        assign_identifier_hints(count, outputs, labels) ;

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
            CHECK_EQ(labels.size(), 17576) ;

            CHECK_EQ(labels[0].size(), 3) ;
            CHECK_EQ(labels[711].size(), 3) ;
            CHECK_EQ(labels[17575].size(), 3) ;

            CHECK_EQ(labels[0], "AAA") ;
            CHECK_EQ(labels[721], "SSC") ;
            CHECK_EQ(labels[17575], "JJJ") ;
        }
    }

    TEST_CASE("Arbitary hints") {
        std::size_t count = 16 ;
        std::vector<Hint> outputs ;
        std::vector<std::string> labels ;
        std::string keys = "as" ;
        assign_identifier_hints(count, outputs, labels, keys) ;

        std::vector<Hint> expected_keys = {
            {KEYCODE_A, KEYCODE_A, KEYCODE_A, KEYCODE_A},
            {KEYCODE_A, KEYCODE_A, KEYCODE_A, KEYCODE_S},
            {KEYCODE_A, KEYCODE_A, KEYCODE_S, KEYCODE_A},
            {KEYCODE_A, KEYCODE_A, KEYCODE_S, KEYCODE_S},

            {KEYCODE_A, KEYCODE_S, KEYCODE_A, KEYCODE_A},
            {KEYCODE_A, KEYCODE_S, KEYCODE_A, KEYCODE_S},
            {KEYCODE_A, KEYCODE_S, KEYCODE_S, KEYCODE_A},
            {KEYCODE_A, KEYCODE_S, KEYCODE_S, KEYCODE_S},

            {KEYCODE_S, KEYCODE_A, KEYCODE_A, KEYCODE_A},
            {KEYCODE_S, KEYCODE_A, KEYCODE_A, KEYCODE_S},
            {KEYCODE_S, KEYCODE_A, KEYCODE_S, KEYCODE_A},
            {KEYCODE_S, KEYCODE_A, KEYCODE_S, KEYCODE_S},

            {KEYCODE_S, KEYCODE_S, KEYCODE_A, KEYCODE_A},
            {KEYCODE_S, KEYCODE_S, KEYCODE_A, KEYCODE_S},
            {KEYCODE_S, KEYCODE_S, KEYCODE_S, KEYCODE_A},
            {KEYCODE_S, KEYCODE_S, KEYCODE_S, KEYCODE_S},
        } ;

        std::vector<std::string> expected_labels = {
            "AAAA",
            "AAAS",
            "AASA",
            "AASS",

            "ASAA",
            "ASAS",
            "ASSA",
            "ASSS",

            "SAAA",
            "SAAS",
            "SASA",
            "SASS",

            "SSAA",
            "SSAS",
            "SSSA",
            "SSSS"
        } ;

        for(int i = 0 ; i < outputs.size() ; i ++) {
            for(int j = 0 ; j < outputs[i].size() ; j ++) {
                CHECK_EQ(outputs[i][j], expected_keys[i][j]) ;
            }
            CHECK_EQ(labels[i], expected_labels[i]) ;
        }
    }
}
