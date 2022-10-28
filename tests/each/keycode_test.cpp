#include <doctest/doctest.h>

#include "core/keycode.hpp"

#include <algorithm>
#include <cstring>
#include <sstream>
#include <vector>

TEST_SUITE("core/keycode") {
    using namespace vind ;
    using namespace vind::core ;

    TEST_CASE("KeyCode::empty, KeyCode::operator bool, KeyCode::operator!") {
        KeyCode a{} ;
        KeyCode b{KEYCODE_2} ;

        CHECK(a.empty()) ;
        CHECK(!a) ;

        CHECK_FALSE(b.empty()) ;
        CHECK(static_cast<bool>(b)) ;
    }

    TEST_CASE("KeyCode::is_*") {
        KeyCode k1('a') ;
        CHECK_FALSE(k1.is_major_system()) ;
        CHECK_FALSE(k1.is_unreal()) ;
        CHECK_FALSE(k1.is_toggle()) ;
        CHECK_FALSE(k1.is_shift()) ;

        KeyCode k2('B') ;
        CHECK_FALSE(k2.is_major_system()) ;
        CHECK_FALSE(k2.is_unreal()) ;
        CHECK_FALSE(k2.is_toggle()) ;
        CHECK_FALSE(k2.is_shift()) ;

        KeyCode k3('2') ;
        CHECK_FALSE(k3.is_major_system()) ;
        CHECK_FALSE(k3.is_unreal()) ;
        CHECK_FALSE(k3.is_toggle()) ;
        CHECK_FALSE(k3.is_shift()) ;

        KeyCode k4(KEYCODE_B) ;
        CHECK_FALSE(k4.is_major_system()) ;
        CHECK_FALSE(k4.is_unreal()) ;
        CHECK_FALSE(k4.is_toggle()) ;
        CHECK_FALSE(k4.is_shift()) ;

        KeyCode k5(KEYCODE_7) ;
        CHECK_FALSE(k5.is_major_system()) ;
        CHECK_FALSE(k5.is_unreal()) ;
        CHECK_FALSE(k5.is_toggle()) ;
        CHECK_FALSE(k5.is_shift()) ;

        KeyCode k6(KEYCODE_SHIFT) ;
        CHECK(k6.is_major_system()) ;
        CHECK(k6.is_unreal()) ;
        CHECK_FALSE(k6.is_toggle()) ;
        CHECK(k6.is_shift()) ;

        KeyCode k7(KEYCODE_LCTRL) ;
        CHECK(k7.is_major_system()) ;
        CHECK_FALSE(k7.is_unreal()) ;
        CHECK_FALSE(k7.is_toggle()) ;
        CHECK_FALSE(k7.is_shift()) ;

        KeyCode k8(KEYCODE_CAPSLOCK) ;
        CHECK(k8.is_major_system()) ;
        CHECK_FALSE(k8.is_unreal()) ;
        CHECK(k8.is_toggle()) ;
        CHECK_FALSE(k8.is_shift()) ;

        KeyCode k9('"') ;
        CHECK_FALSE(k9.is_major_system()) ;
        CHECK_FALSE(k9.is_unreal()) ;
        CHECK_FALSE(k9.is_toggle()) ;
        CHECK_FALSE(k9.is_shift()) ;

        KeyCode k10('#') ;
        CHECK_FALSE(k10.is_major_system()) ;
        CHECK_FALSE(k10.is_unreal()) ;
        CHECK_FALSE(k10.is_toggle()) ;
        CHECK_FALSE(k10.is_shift()) ;

        KeyCode k11("ralt") ;
        CHECK(k11.is_major_system()) ;
        CHECK_FALSE(k11.is_unreal()) ;
        CHECK_FALSE(k11.is_toggle()) ;
        CHECK(k11.is_shift()) ;

        KeyCode k12("space") ;  // magic ascii
        CHECK_FALSE(k12.is_major_system()) ;
        CHECK_FALSE(k12.is_unreal()) ;
        CHECK_FALSE(k12.is_toggle()) ;
        CHECK_FALSE(k12.is_shift()) ;

        KeyCode k13("num") ;
        CHECK(k13.is_major_system()) ;
        CHECK(k13.is_unreal()) ;
        CHECK_FALSE(k13.is_toggle()) ;
        CHECK_FALSE(k13.is_shift()) ;

        KeyCode k14("any") ;
        CHECK(k14.is_major_system()) ;
        CHECK(k14.is_unreal()) ;
        CHECK_FALSE(k14.is_toggle()) ;
        CHECK_FALSE(k14.is_shift()) ;
    }

    TEST_CASE("KeyCode::to_code") {
        KeyCode k1(KEYCODE_0) ;
        CHECK_EQ(k1.to_code(), KEYCODE_0) ;

        KeyCode k2(KEYCODE_SHIFT) ;
        CHECK_EQ(k2.to_code(), KEYCODE_SHIFT) ;

        KeyCode k3(KEYCODE_LSHIFT) ;
        CHECK_EQ(k3.to_code(), KEYCODE_LSHIFT) ;

        KeyCode k4('A') ;
        CHECK_EQ(k4.to_code(), KEYCODE_A) ;

        KeyCode k5('a') ;
        CHECK_EQ(k5.to_code(), KEYCODE_A) ;
    }

    TEST_CASE("KeyCode::to_representative, KeyCode::to_physical") {
        KeyCode k1(KEYCODE_LSHIFT) ;
        KeyCode expect1(KEYCODE_SHIFT) ;
        CHECK_EQ(k1.to_representative(), expect1) ;
        CHECK_EQ(k1.to_physical(), k1) ;

        KeyCode k2(KEYCODE_RCTRL) ;
        KeyCode expect2(KEYCODE_CTRL) ;
        CHECK_EQ(k2.to_representative(), expect2) ;
        CHECK_EQ(k2.to_physical(), k2) ;

        KeyCode k3(KEYCODE_SHIFT) ;
        KeyCode expect3(KEYCODE_LSHIFT) ;
        CHECK(k3.to_representative().empty()) ;
        CHECK_EQ(k3.to_physical(), expect3) ;
    }

    TEST_CASE("KeyCode::name") {
        KeyCode k1("A") ;
        CHECK_EQ(k1.name(), "a") ;

        KeyCode k2(KEYCODE_ESC) ;
        CHECK_EQ(k2.name(), "esc") ;

        KeyCode k3(KEYCODE_SHIFT) ;
        CHECK_EQ(k3.name(), "shift") ;

        KeyCode k4(KEYCODE_LSHIFT) ;
        CHECK_EQ(k4.name(), "lshift") ;
    }

    TEST_CASE("KeyCode::operator unsigned char, int") {
        KeyCode k1(KEYCODE_0) ;
        CHECK_EQ(static_cast<unsigned char>(k1), KEYCODE_0) ;
        CHECK_EQ(static_cast<int>(k1), static_cast<int>(KEYCODE_0)) ;

        KeyCode k2("A") ;
        CHECK_EQ(static_cast<unsigned char>(k2), KEYCODE_A) ;
        CHECK_EQ(static_cast<int>(k2), static_cast<int>(KEYCODE_A)) ;

        KeyCode k3("shift") ;
        CHECK_EQ(static_cast<unsigned char>(k3), KEYCODE_SHIFT) ;
        CHECK_EQ(static_cast<int>(k3), static_cast<int>(KEYCODE_SHIFT)) ;
    }

    TEST_CASE("KeyCode::operator std::string") {
        KeyCode k1("tab") ;
        std::string s1 = k1 ;
        CHECK_EQ(s1, "tab") ;

        KeyCode k2(KEYCODE_0) ;
        std::string s2 = k2 ;
        CHECK_EQ(s2, "0") ;

        KeyCode k3('A') ;
        std::string s3 = k3 ;
        CHECK_EQ(s3, "a") ;

        KeyCode k4(KEYCODE_LSHIFT) ;
        std::string s4 = k4 ;
        CHECK_EQ(s4, "lshift") ;
    }

    TEST_CASE("KeyCode::operator==, KeyCode::operator!=") {
        KeyCode k1("a") ;
        KeyCode k1_("a") ;
        KeyCode k2("A") ;
        KeyCode k2_('A') ;
        KeyCode k3("SHIFT") ;
        KeyCode k3_("ShIFT") ;
        KeyCode k4("CapsLock") ;
        KeyCode k4_("capsLock") ;

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
        CHECK_THROWS_AS(KeyCode("hhogehogejflak;"), std::logic_error) ;

        KeyCode k1("A", true) ;
        KeyCode k2("a", true) ;
        KeyCode k3("A", false) ;
        KeyCode k4("a", false) ;

        CHECK_EQ(k1.name(), "a") ;
        CHECK_EQ(k2.name(), "a") ;
        CHECK_EQ(k3.name(), "alt") ;
        CHECK_EQ(k4.name(), "alt") ;
    }

    TEST_CASE("operator<<(Stream, KeyCode)") {
        std::vector<KeyCode> v ;
        v.emplace_back("H") ;
        v.emplace_back("shift") ;
        v.emplace_back("alt") ;
        v.emplace_back("c") ;

        std::stringstream ss ;
        for(const auto& k : v) {
            if(!k.is_major_system()) {
                ss << k ;
            }
            else {
                ss << "<" << k << ">" ;
            }
        }

        CHECK_EQ(ss.str(), "h<shift><alt>c") ;
    }

    TEST_CASE("operator<<(Stream, KeySet)") {
        KeySet ks {
            KeyCode("shift"),
            KeyCode("h"),
            KeyCode("b"),
            KeyCode("alt"),
            KeyCode("e")
        } ;

        std::stringstream ss ;
        ss << ks ;
        CHECK_EQ(ss.str(), "<shift-h-b-alt-e>") ;
    }

    TEST_CASE("operator<<(Stream, Command)") {
        Command cmd {
            {KeyCode("shift"), KeyCode("k"), KeyCode("j")},
            {KeyCode("ctrl"), KeyCode("h")}
        } ;

        std::stringstream ss ;
        ss << cmd ;
        CHECK_EQ(ss.str(), "<shift-k-j><ctrl-h>") ;
    }

    TEST_CASE("sortablility") {
        std::vector<KeyCode> keys {
            KEYCODE_A,
            KEYCODE_LSHIFT
        } ;

        std::vector<KeyCode> expect {
            KEYCODE_LSHIFT,
            KEYCODE_A
        } ;

        std::sort(keys.begin(), keys.end()) ;
        CHECK_EQ(keys, expect) ;

        std::sort(keys.begin(), keys.end()) ;
        CHECK_EQ(keys, expect) ;

        std::sort(keys.begin(), keys.end()) ;
        CHECK_EQ(keys, expect) ;
    }

    TEST_CASE("keycode_to_number") {
        for(int i = 0 ; i < 10 ; i ++) {
            KeyCode k(i + KEYCODE_0) ;
            CHECK_EQ(keycode_to_number(k), i) ;
        }
    }

    TEST_CASE("get_shift_keycode") {
        auto s1 = get_shift_keycode('A') ;
        CHECK_EQ(s1, KEYCODE_SHIFT) ;
    }

    TEST_CASE("char_to_keycode") {
        auto k1 = char_to_keycode('A') ;
        CHECK_EQ(k1, KEYCODE_A) ;

        auto k2 = char_to_keycode('a') ;
        CHECK_EQ(k2, KEYCODE_A) ;
    }

    TEST_CASE("is_need_deadkey_for_input") {
        // TODO: Depends on the keyboard layout of the runtime
        // environment, so a mock is required for complete testing.

        for(char i = 'A' ; i < 'Z' ; i ++) {
            CHECK_FALSE(is_need_deadkey_for_input(i)) ;
        }

        for(char i = 'a' ; i < 'z' ; i ++) {
            CHECK_FALSE(is_need_deadkey_for_input(i)) ;
        }
    }

    TEST_CASE("keycode_to_unicode") {
        KeyCode shift(KEYCODE_SHIFT) ;

        KeyCode k1(KEYCODE_A) ;
        CHECK_EQ(keycode_to_unicode(k1), "a") ;

        KeySet ss{shift} ;
        CHECK_EQ(keycode_to_unicode(k1, ss), "A") ;

        CHECK(keycode_to_unicode(KEYCODE_ESC).empty()) ;
    }
}
