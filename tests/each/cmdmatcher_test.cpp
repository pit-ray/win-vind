#include <doctest/doctest.h>

#include "core/cmdmatcher.hpp"

#include <iostream>


namespace vind
{
    namespace bind
    {
        const std::string& BindedFunc::name() const noexcept {
            static std::string tmp = "<func>" ;
            return tmp ;
        }

        SystemCall BindedFunc::process(std::uint16_t, const std::string&) {
            return SystemCall::NOTHING ;
        }

        BindedFunc::SPtr ref_global_func_byname(const std::string& name) {
            return nullptr ;
        }
    }
}


TEST_SUITE("core/cmdmatcher") {
    using namespace vind::core ;

    TEST_CASE("basic keycode") {
        std::vector<CmdUnitSet> cmd {
            {KEYCODE_CTRL, KEYCODE_B},
            {KEYCODE_SHIFT, KEYCODE_F},
            {KEYCODE_LCTRL, KEYCODE_F}
        } ;

        std::vector<CmdUnit::SPtr> ptrcmd{} ;
        for(const auto& cmdunit : cmd) {
            ptrcmd.push_back(std::make_shared<CmdUnit>(cmdunit)) ;
        }

        CmdMatcher matcher{ptrcmd} ;

        SUBCASE("case1: reject") {
            CmdUnit in1{KEYCODE_CTRL, KEYCODE_B} ;
            int res = matcher.update_state(in1) ;
            CHECK_EQ(res, 2) ;
            CHECK(matcher.is_matching()) ;

            CmdUnit in2{KEYCODE_F, KEYCODE_SHIFT, KEYCODE_LSHIFT} ;
            res = matcher.update_state(in2) ;
            CHECK_EQ(res, 3) ;
            CHECK(matcher.is_matching()) ;

            CmdUnit in3{KEYCODE_F, KEYCODE_SHIFT} ;
            res = matcher.update_state(in3) ;
            CHECK_EQ(res, 0) ;
            CHECK(matcher.is_rejected()) ;
        }

        SUBCASE("case2: backward") {
            CmdUnit in1{KEYCODE_CTRL, KEYCODE_B} ;
            int res = matcher.update_state(in1) ;
            CHECK_EQ(res, 2) ;
            CHECK(matcher.is_matching()) ;

            CmdUnit in2{KEYCODE_J} ;
            res = matcher.update_state(in2) ;
            CHECK_EQ(res, 0) ;
            CHECK(matcher.is_rejected()) ;

            matcher.backward_state(1) ;
            CHECK(matcher.is_matching()) ;

            CmdUnit in3{KEYCODE_F, KEYCODE_RSHIFT} ;
            res = matcher.update_state(in3) ;
            CHECK_EQ(res, 2) ;
            CHECK(matcher.is_matching()) ;

            matcher.reset_state() ;
            CHECK(matcher.is_matching()) ;

            res = matcher.update_state(in1) ;
            CHECK_EQ(res, 2) ;
            CHECK(matcher.is_matching()) ;
        }

        SUBCASE("case3: accept") {
            CmdUnit in1{KEYCODE_CTRL, KEYCODE_B} ;
            int res = matcher.update_state(in1) ;
            CHECK_EQ(res, 2) ;
            CHECK(matcher.is_matching()) ;

            CmdUnit in2{KEYCODE_F, KEYCODE_SHIFT, KEYCODE_LSHIFT} ;
            res = matcher.update_state(in2) ;
            CHECK_EQ(res, 3) ;
            CHECK(matcher.is_matching()) ;

            CmdUnit in3{KEYCODE_CTRL, KEYCODE_F} ;
            res = matcher.update_state(in3) ;
            CHECK_EQ(res, 2) ;
            CHECK(matcher.is_accepted()) ;

            CmdUnit in4{KEYCODE_D} ;
            res = matcher.update_state(in4) ;
            CHECK_EQ(res, 0) ;
            CHECK(matcher.is_rejected()) ;
        }
    }

    TEST_CASE("<any>") {
        std::vector<CmdUnitSet> cmd {
            {KEYCODE_CTRL, KEYCODE_B},
            {KEYCODE_F},
            {KEYCODE_OPTIONAL}
        } ;

        std::vector<CmdUnit::SPtr> ptrcmd{} ;
        for(const auto& cmdunit : cmd) {
            ptrcmd.push_back(std::make_shared<CmdUnit>(cmdunit)) ;
        }

        CmdMatcher matcher{ptrcmd} ;

        SUBCASE("case1") {
            CmdUnit in1{KEYCODE_CTRL, KEYCODE_B} ;
            int res = matcher.update_state(in1) ;
            CHECK_EQ(res, 2) ;
            CHECK(matcher.is_matching()) ;

            CmdUnit in2{KEYCODE_F} ;
            res = matcher.update_state(in2) ;
            CHECK_EQ(res, 1) ;
            CHECK(matcher.is_matching()) ;

            CmdUnit in3{KEYCODE_F, KEYCODE_SHIFT, KEYCODE_U} ;
            res = matcher.update_state(in3) ;
            CHECK_EQ(res, 3) ;
            CHECK(matcher.is_accepted()) ;

            CmdUnit in4{KEYCODE_F} ;
            res = matcher.update_state(in4) ;
            CHECK_EQ(res, 1) ;
            CHECK(matcher.is_accepted()) ;
        }
    }

    TEST_CASE("<num>") {
        std::vector<CmdUnitSet> cmd {
            {KEYCODE_LCTRL, KEYCODE_B},
            {KEYCODE_OPTNUMBER},
            {KEYCODE_F},
        } ;

        std::vector<CmdUnit::SPtr> ptrcmd{} ;
        for(const auto& cmdunit : cmd) {
            ptrcmd.push_back(std::make_shared<CmdUnit>(cmdunit)) ;
        }

        CmdMatcher matcher{ptrcmd} ;

        SUBCASE("case1") {
            CmdUnit in1{KEYCODE_CTRL, KEYCODE_B} ;
            int res = matcher.update_state(in1) ;
            CHECK_EQ(res, 2) ;
            CHECK(matcher.is_matching()) ;

            CmdUnit in2{KEYCODE_1, KEYCODE_2} ;
            res = matcher.update_state(in2) ;
            CHECK_EQ(res, 2) ;
            CHECK(matcher.is_matching()) ;

            CmdUnit in3{KEYCODE_2} ;
            res = matcher.update_state(in3) ;
            CHECK_EQ(res, 1) ;
            CHECK(matcher.is_matching()) ;

            CmdUnit in4{KEYCODE_F} ;
            res = matcher.update_state(in4) ;
            CHECK_EQ(res, 1) ;
            CHECK(matcher.is_accepted()) ;

            matcher.backward_state(1) ;

            CmdUnit in5{KEYCODE_9, KEYCODE_8} ;
            res = matcher.update_state(in5) ;
            CHECK_EQ(res, 2) ;
            CHECK(matcher.is_matching()) ;

            CmdUnit in6{KEYCODE_J} ;
            res = matcher.update_state(in6) ;
            CHECK_EQ(res, 0) ;
            CHECK(matcher.is_rejected()) ;
        }
    }

    TEST_CASE("typing w/ ctrl") {
        std::vector<CmdUnitSet> cmd {
            {KEYCODE_CTRL, KEYCODE_B},
        } ;

        std::vector<CmdUnit::SPtr> ptrcmd{} ;
        for(const auto& cmdunit : cmd) {
            ptrcmd.push_back(std::make_shared<CmdUnit>(cmdunit)) ;
        }

        CmdMatcher matcher{ptrcmd} ;

        CmdUnit in1{KEYCODE_CTRL} ;
        int res = matcher.update_state(in1) ;
        CHECK_EQ(res, 0) ;
        CHECK(matcher.is_rejected()) ;

        matcher.backward_state(1) ;

        CmdUnit in2{KEYCODE_CTRL, KEYCODE_B} ;
        res = matcher.update_state(in2) ;
        CHECK_EQ(res, 2) ;
        CHECK(matcher.is_accepted()) ;
    }

    TEST_CASE("typing w/ shift") {
        std::vector<CmdUnitSet> cmd {
            {KEYCODE_SHIFT, KEYCODE_F},
            {KEYCODE_SHIFT, KEYCODE_F},
        } ;

        std::vector<CmdUnit::SPtr> ptrcmd{} ;
        for(const auto& cmdunit : cmd) {
            ptrcmd.push_back(std::make_shared<CmdUnit>(cmdunit)) ;
        }

        CmdMatcher matcher{ptrcmd} ;

        CmdUnit in1{KEYCODE_SHIFT} ;
        int res = matcher.update_state(in1) ;
        CHECK_EQ(res, 0) ;
        CHECK(matcher.is_rejected()) ;

        matcher.backward_state(1) ;

        CmdUnit in2{KEYCODE_SHIFT, KEYCODE_F} ;
        res = matcher.update_state(in2) ;
        CHECK_EQ(res, 2) ;
        CHECK(matcher.is_matching()) ;

        CmdUnit in3{KEYCODE_SHIFT} ;
        res = matcher.update_state(in3) ;
        CHECK_EQ(res, 0) ;
        CHECK(matcher.is_rejected()) ;

        matcher.backward_state(1) ;

        CmdUnit in4{KEYCODE_SHIFT, KEYCODE_F} ;
        res = matcher.update_state(in4) ;
        CHECK_EQ(res, 2) ;
        CHECK(matcher.is_accepted()) ;

    }
}
