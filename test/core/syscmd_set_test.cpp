#include <doctest.h>

#include "g_params.cpp"
#include "syscmd/set.cpp"

using namespace vind ;
using namespace vind::syscmd ;

TEST_CASE("(syscmd::do_set) Clear all parameters") {
    gparams::clear() ;

    SUBCASE("(syscmd::do_set) Empty argument occurs exception.") {
        CHECK_THROWS(do_set("")) ;
    }

    SUBCASE("(syscmd::do_set) It has only spaces.") {
        CHECK_THROWS(do_set("      ")) ;
    }

    SUBCASE("(syscmd::do_set) It does not have the option name.") {
        CHECK_THROWS(do_set("  =  ")) ;
    }

    SUBCASE("(syscmd::do_set) It does not have the option name with a string value.") {
        CHECK_THROWS(do_set("  =  aaa")) ;
    }

    SUBCASE("(syscmd::do_set) It does not have the option name with a numeric value.") {
        CHECK_THROWS(do_set("  =  23")) ;
    }

    SUBCASE("(syscmd::do_set) It does not have the option value.") {
        CHECK_THROWS(do_set(" ABC =  ")) ;
    }

    SUBCASE("(syscmd::do_set) The option name is one character.") {
        CHECK_NOTHROW(do_set("A")) ;
        CHECK(gparams::get_b("a")) ;
    }

    SUBCASE("(syscmd::do_set) The option name is `no`.") {
        CHECK_NOTHROW(do_set("no")) ;
        CHECK(gparams::get_b("no")) ;
    }

    SUBCASE("(syscmd::do_set) Enable option") {
        CHECK_NOTHROW(do_set("ABC")) ;
        CHECK(gparams::get_b("abc")) ;
    }

    SUBCASE("(syscmd::do_set) Disable option.") {
        CHECK_NOTHROW(do_set("noABC")) ;
        CHECK_FALSE(gparams::get_b("abc")) ;
    }

    SUBCASE("(syscmd::do_set) Set numeric option.") {
        CHECK_NOTHROW(do_set("ABC = 25")) ;
        CHECK_EQ(gparams::get_i("abc"), 25) ;
    }

    SUBCASE("(syscmd::do_set) Set invalid option name.") {
        CHECK_THROWS(do_set("A BC = 25")) ;
    }

    SUBCASE("(syscmd::do_set) Set string option.") {
        CHECK_NOTHROW(do_set("ABC = tthbd")) ;
        CHECK_EQ(gparams::get_s("abc"), "tthbd") ;
    }

    SUBCASE("(syscmd::do_set) Set string option including number") {
        CHECK_NOTHROW(do_set("ABC = 235h23")) ;
        CHECK_EQ(gparams::get_s("abc"), "235h23") ;
    }

    SUBCASE("(syscmd::do_set) Set string option including space") {
        CHECK_NOTHROW(do_set("ABC = 235 h23  ")) ;
        CHECK_EQ(gparams::get_s("abc"), "235 h23") ;
    }
}
