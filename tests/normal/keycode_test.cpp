#include <doctest.h>

#include "core/keycode.cpp"

#include <cstring>
#include <sstream>
#include <vector>

using namespace vind ;

TEST_SUITE("core/keycode") {
    TEST_CASE("KeyCode::empty, KeyCode::operator bool, KeyCode::operator!") {
        core::KeyCode a{} ;
        core::KeyCode b{KEYCODE_2} ;

        CHECK(a.empty()) ;
        CHECK(!a) ;

        CHECK_FALSE(b.empty()) ;
        CHECK(static_cast<bool>(b)) ;
    }

    TEST_CASE("KeyCode::is_*") {
        core::KeyCode k1('a') ;
        CHECK(k1.is_ascii()) ;
        CHECK_FALSE(k1.is_unreal()) ;
        CHECK_FALSE(k1.is_number()) ;
        CHECK_FALSE(k1.is_toggle()) ;

        core::KeyCode k2('B') ;
        CHECK(k2.is_ascii()) ;
        CHECK_FALSE(k2.is_unreal()) ;
        CHECK_FALSE(k2.is_number()) ;
        CHECK_FALSE(k2.is_toggle()) ;

        core::KeyCode k3('2') ;
        CHECK(k3.is_ascii()) ;
        CHECK_FALSE(k3.is_unreal()) ;
        CHECK(k3.is_number()) ;
        CHECK_FALSE(k3.is_toggle()) ;

        core::KeyCode k4(KEYCODE_B) ;
        CHECK(k4.is_ascii()) ;
        CHECK_FALSE(k4.is_unreal()) ;
        CHECK_FALSE(k4.is_number()) ;
        CHECK_FALSE(k4.is_toggle()) ;

        core::KeyCode k5(KEYCODE_7) ;
        CHECK(k5.is_ascii()) ;
        CHECK_FALSE(k5.is_unreal()) ;
        CHECK(k5.is_number()) ;
        CHECK_FALSE(k5.is_toggle()) ;

        core::KeyCode k6(KEYCODE_SHIFT) ;
        CHECK_FALSE(k6.is_ascii()) ;
        CHECK(k6.is_unreal()) ;
        CHECK_FALSE(k6.is_number()) ;
        CHECK_FALSE(k6.is_toggle()) ;

        core::KeyCode k7(KEYCODE_LCTRL) ;
        CHECK_FALSE(k7.is_ascii()) ;
        CHECK_FALSE(k7.is_unreal()) ;
        CHECK_FALSE(k7.is_number()) ;
        CHECK_FALSE(k7.is_toggle()) ;

        core::KeyCode k8(KEYCODE_CAPSLOCK) ;
        CHECK_FALSE(k8.is_ascii()) ;
        CHECK_FALSE(k8.is_unreal()) ;
        CHECK_FALSE(k8.is_number()) ;
        CHECK(k8.is_toggle()) ;

        core::KeyCode k9('"') ;
        CHECK(k9.is_ascii()) ;
        CHECK_FALSE(k9.is_unreal()) ;
        CHECK_FALSE(k9.is_number()) ;
        CHECK_FALSE(k9.is_toggle()) ;

        core::KeyCode k10('#') ;
        CHECK(k10.is_ascii()) ;
        CHECK_FALSE(k10.is_unreal()) ;
        CHECK_FALSE(k10.is_number()) ;
        CHECK_FALSE(k10.is_toggle()) ;

        core::KeyCode k11("alt") ;
        CHECK_FALSE(k11.is_ascii()) ;
        CHECK(k11.is_unreal()) ;
        CHECK_FALSE(k11.is_number()) ;
        CHECK_FALSE(k11.is_toggle()) ;

        core::KeyCode k12("space") ;  // magic ascii
        CHECK(k12.is_ascii()) ;
        CHECK_FALSE(k12.is_unreal()) ;
        CHECK_FALSE(k12.is_number()) ;
        CHECK_FALSE(k12.is_toggle()) ;

        core::KeyCode k13("num") ;
        CHECK_FALSE(k13.is_ascii()) ;
        CHECK(k13.is_unreal()) ;
        CHECK_FALSE(k13.is_number()) ;
        CHECK_FALSE(k13.is_toggle()) ;

        core::KeyCode k14("any") ;
        CHECK_FALSE(k14.is_ascii()) ;
        CHECK(k14.is_unreal()) ;
        CHECK_FALSE(k14.is_number()) ;
        CHECK_FALSE(k14.is_toggle()) ;
    }

    TEST_CASE("KeyCode::to_ascii, KeyCode::to_shifted_ascii") {
        char c1 = 'A' ;
        core::KeyCode k1(c1) ;
        CHECK_EQ(k1.to_ascii(), 'a') ;
        CHECK_EQ(k1.to_shifted_ascii(), 'A') ;

        char c2 = 'b' ;
        core::KeyCode k2(c2) ;
        CHECK_EQ(k2.to_ascii(), 'b') ;
        CHECK_EQ(k2.to_shifted_ascii(), 'B') ;

        char c3 = '=' ;
        core::KeyCode k3(c3) ;

        char c4 = '\\' ;
        core::KeyCode k4(c4) ;

        char c5 = '5' ;
        core::KeyCode k5(c5) ;

        core::KeyCode k6(KEYCODE_SPACE) ;
        CHECK_EQ(k6.to_ascii(), ' ') ;
        CHECK_EQ(k6.to_shifted_ascii(), 0) ;

        core::KeyCode k7(KEYCODE_ESC) ;
        CHECK_EQ(k7.to_ascii(), 0) ;
        CHECK_EQ(k7.to_shifted_ascii(), 0) ;

        core::KeyCode k8("space") ;
        CHECK_EQ(k8.to_ascii(), ' ') ;
        CHECK_EQ(k8.to_shifted_ascii(), 0) ;
    }

    TEST_CASE("KeyCode::to_number") {
        for(int i = 0 ; i < 10 ; i ++) {
            core::KeyCode k(i + KEYCODE_0) ;

            CHECK(k.is_number()) ;
            CHECK_EQ(k.to_number(), i) ;
        }
    }

    TEST_CASE("KeyCode::to_code") {
        core::KeyCode k1(KEYCODE_0) ;
        CHECK_EQ(k1.to_code(), KEYCODE_0) ;

        core::KeyCode k2(KEYCODE_SHIFT) ;
        CHECK_EQ(k2.to_code(), KEYCODE_SHIFT) ;

        core::KeyCode k3(KEYCODE_LSHIFT) ;
        CHECK_EQ(k3.to_code(), KEYCODE_LSHIFT) ;

        core::KeyCode k4('A') ;
        CHECK_EQ(k4.to_code(), KEYCODE_A) ;

        core::KeyCode k5('a') ;
        CHECK_EQ(k5.to_code(), KEYCODE_A) ;
    }

    TEST_CASE("KeyCode::to_representative, KeyCode::to_physical") {
        core::KeyCode k1(KEYCODE_LSHIFT) ;
        core::KeyCode expect1(KEYCODE_SHIFT) ;
        CHECK_EQ(k1.to_representative(), expect1) ;
        CHECK_EQ(k1.to_physical(), k1) ;

        core::KeyCode k2(KEYCODE_RCTRL) ;
        core::KeyCode expect2(KEYCODE_CTRL) ;
        CHECK_EQ(k2.to_representative(), expect2) ;
        CHECK_EQ(k2.to_physical(), k2) ;

        core::KeyCode k3(KEYCODE_SHIFT) ;
        core::KeyCode expect3(KEYCODE_LSHIFT) ;
        CHECK(k3.to_representative().empty()) ;
        CHECK_EQ(k3.to_physical(), expect3) ;
    }

    TEST_CASE("KeyCode::name") {
        core::KeyCode k1("A") ;
        CHECK_EQ(k1.name(), "a") ;

        core::KeyCode k2(KEYCODE_ESC) ;
        CHECK_EQ(k2.name(), "esc") ;

        core::KeyCode k3(KEYCODE_SHIFT) ;
        CHECK_EQ(k3.name(), "shift") ;

        core::KeyCode k4(KEYCODE_LSHIFT) ;
        CHECK_EQ(k4.name(), "lshift") ;
    }

    TEST_CASE("KeyCode::operator char") {
        core::KeyCode k1('a') ;
        CHECK_EQ(static_cast<char>(k1), 'a') ;

        core::KeyCode k2(' ') ;
        CHECK_EQ(static_cast<char>(k2), ' ') ;

        core::KeyCode k3(KEYCODE_ALT) ;
        CHECK_EQ(static_cast<char>(k3), 0) ;
    }

    TEST_CASE("KeyCode::operator unsigned char, int") {
        core::KeyCode k1(KEYCODE_0) ;
        CHECK_EQ(static_cast<unsigned char>(k1), KEYCODE_0) ;
        CHECK_EQ(static_cast<int>(k1), static_cast<int>(KEYCODE_0)) ;

        core::KeyCode k2("A") ;
        CHECK_EQ(static_cast<unsigned char>(k2), KEYCODE_A) ;
        CHECK_EQ(static_cast<int>(k2), static_cast<int>(KEYCODE_A)) ;

        core::KeyCode k3("shift") ;
        CHECK_EQ(static_cast<unsigned char>(k3), KEYCODE_SHIFT) ;
        CHECK_EQ(static_cast<int>(k3), static_cast<int>(KEYCODE_SHIFT)) ;
    }

    TEST_CASE("KeyCode::operator std::string, const char*") {
        auto check_std_string = [](auto& key, auto&& str) {
            std::string sbuf = key ;
            CHECK_EQ(sbuf, str) ;
        } ;

        core::KeyCode k1("tab") ;
        check_std_string(k1, "tab") ;
        CHECK_EQ(std::strcmp(static_cast<const char*>(k1), "tab"), 0) ;

        core::KeyCode k2(KEYCODE_0) ;
        check_std_string(k2, "0") ;
        CHECK_EQ(std::strcmp(static_cast<const char*>(k2), "0"), 0) ;

        core::KeyCode k3('A') ;
        check_std_string(k3, "a") ;
        CHECK_EQ(std::strcmp(static_cast<const char*>(k3), "a"), 0) ;

        core::KeyCode k4(KEYCODE_LSHIFT) ;
        check_std_string(k4, "lshift") ;
        CHECK_EQ(std::strcmp(static_cast<const char*>(k4), "lshift"), 0) ;
    }

    TEST_CASE("KeyCode::operator==, KeyCode::operator!=") {
        core::KeyCode k1("a") ;
        core::KeyCode k1_("a") ;
        core::KeyCode k2("A") ;
        core::KeyCode k2_('A') ;
        core::KeyCode k3("SHIFT") ;
        core::KeyCode k3_("ShIFT") ;
        core::KeyCode k4("CapsLock") ;
        core::KeyCode k4_("capsLock") ;

        CHECK(k1 == k1) ;
        CHECK(k1 == k1_) ;
        CHECK(k2 == k2) ;
        CHECK(k2 == k2_) ;
        CHECK(k3 == k3) ;
        CHECK(k3 == k3_) ;
        CHECK(k4 == k4) ;
        CHECK(k4 == k4_) ;

        CHECK(k1 == k2) ;
        CHECK(k1 != k3) ;
        CHECK(k1 != k4) ;

        CHECK(k2 != k3) ;
        CHECK(k2 != k4) ;

        CHECK(k3 != k4) ;
    }

    TEST_CASE("KeyCode::KeyCode") {
        CHECK_THROWS_AS(core::KeyCode("hhogehogejflak;"), std::out_of_range) ;

        core::KeyCode k1("A", true) ;
        core::KeyCode k2("a", true) ;
        core::KeyCode k3("A", false) ;
        core::KeyCode k4("a", false) ;

        CHECK_EQ(k1.name(), "a") ;
        CHECK_EQ(k2.name(), "a") ;
        CHECK_EQ(k3.name(), "alt") ;
        CHECK_EQ(k4.name(), "alt") ;
    }

    TEST_CASE("KeyCode::is_shifted") {
        CHECK_FALSE(core::KeyCode::is_shifted('a')) ;
        CHECK(core::KeyCode::is_shifted('A')) ;
    }

    TEST_CASE("operator<<(Stream, KeyCode)") {
        std::vector<core::KeyCode> v ;
        v.emplace_back("H") ;
        v.emplace_back("shift") ;
        v.emplace_back("alt") ;
        v.emplace_back("c") ;

        std::stringstream ss ;
        for(const auto& k : v) {
            if(k.is_ascii()) {
                ss << k ;
            }
            else {
                ss << "<" << k << ">" ;
            }
        }

        CHECK_EQ(ss.str(), "h<shift><alt>c") ;
    }
}
