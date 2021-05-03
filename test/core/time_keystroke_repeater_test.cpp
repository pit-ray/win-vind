#include <doctest.h>

#include <chrono>
#include <iostream>

#include "time/keystroke_repeater.cpp"

using namespace vind ;

TEST_CASE("(KeyStrokeRepeater::global) Increases an inputted velocity with a constant acceleration base") {
    float v = 0.0001f ;
    for(float tms = 0.0f ; tms < 1000.0f ; tms ++) {
        auto after = const_accelerate(v, tms) ;
        CHECK_GE(after, v) ; // increased from previous velocity (after >= v)
        v = after ; //update
    }
}

using namespace std::chrono ;
constexpr auto WAIT_TIME_CHRONO = 230ms ;
constexpr auto WAIT_TIME_MS = static_cast<int>(duration_cast<milliseconds>(WAIT_TIME_CHRONO).count()) ;

TEST_CASE("(KeyStrokeRepeater) Method Test: ") {
    KeyStrokeRepeater ksr(WAIT_TIME_MS) ;

    SUBCASE("Test the setter of waiting time") {
        CHECK_EQ(ksr.get_wait_time_ms(), WAIT_TIME_MS) ;
    }

    SUBCASE("Test the Getter of waiting time") {
        CHECK_NOTHROW(ksr.set_wait_time(100)) ;
        CHECK_EQ(ksr.get_wait_time_ms(), 100) ;
    }

    SUBCASE("Copy Constructer Test") {
        KeyStrokeRepeater clone(ksr) ;
        CHECK_EQ(clone.get_wait_time_ms(), WAIT_TIME_MS) ;
    }

    SUBCASE("Copy Operator Test") {
        KeyStrokeRepeater clone ;
        CHECK_NOTHROW(clone = ksr) ;
        CHECK_EQ(clone.get_wait_time_ms(), WAIT_TIME_MS) ;
    }

    SUBCASE("Move Constructer Test") {
        KeyStrokeRepeater clone(std::move(ksr)) ;
        CHECK_EQ(clone.get_wait_time_ms(), WAIT_TIME_MS) ;
    }

    SUBCASE("Move Operator Test") {
        KeyStrokeRepeater clone ;
        CHECK_NOTHROW(clone = std::move(ksr)) ;
        CHECK_EQ(clone.get_wait_time_ms(), WAIT_TIME_MS) ;
    }

    SUBCASE("It stops for a period of time at first") {
        using namespace std::chrono ;

        auto t = system_clock::now() ;
        int true_num = 0 ;
        while((system_clock::now() - t) < WAIT_TIME_CHRONO / 2) {
            if(ksr.is_pressed()) true_num ++ ;
        }
        CHECK_EQ(true_num, 0) ;

        SUBCASE("It emulates the repeating of keystroke") {
            t = system_clock::now() ;
            true_num = 0 ;
            while((system_clock::now() - t) < WAIT_TIME_CHRONO * 3) {
                if(ksr.is_pressed()) true_num ++ ; //sometimes true
            }
            CHECK_GT(true_num, 2) ; //pushed repeat

            SUBCASE("Reset state of KeyStrokeRepeater, so its behavior is the same as first.") {
                true_num = 0 ;
                t = system_clock::now() ;
                ksr.reset() ;
                while((system_clock::now() - t) < WAIT_TIME_CHRONO / 2) {
                    if(ksr.is_pressed()) true_num ++ ;
                }
                CHECK_EQ(true_num, 0) ;
            }
        }
    }
}
