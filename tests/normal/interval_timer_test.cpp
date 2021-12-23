#include <doctest.h>

#include "util/interval_timer.cpp"

#include <chrono>

using namespace vind::util ;

TEST_SUITE("util/interval_timer") {
    TEST_CASE("Method Test: ") {
        IntervalTimer timer(500) ;

        SUBCASE("Delta Getter Test") {
            CHECK_EQ(timer.get_delta_us(), 500) ;
        }

        SUBCASE("Delta Setter Test") {
            timer.set_delta(123) ;
            CHECK_EQ(timer.get_delta_us(), 123) ;
        }

        SUBCASE("Copy Constructer Test") {
            IntervalTimer clone(timer) ;
            CHECK_EQ(clone.get_delta_us(), 500) ;
        }

        SUBCASE("Copy Operator Test") {
            IntervalTimer clone(100) ;
            clone = timer ;
            CHECK_EQ(clone.get_delta_us(), 500) ;
        }

        SUBCASE("Move Constructer Test") {
            IntervalTimer clone(std::move(timer)) ;
            CHECK_EQ(clone.get_delta_us(), 500) ;
        }
        SUBCASE("Move Operator Test") {
            IntervalTimer clone(100) ;
            clone = std::move(timer) ;
            CHECK_EQ(clone.get_delta_us(), 500) ;
        }

        SUBCASE("Does the timer make pass at first?") {
            CHECK(timer.is_passed()) ; //pass once
            CHECK_FALSE(timer.is_passed()) ; //block right now

        }
        SUBCASE("Does the timer make pass after elapsing the delta time?") {
            timer.is_passed() ; //pass at first

            using namespace std::chrono ;
            auto t = system_clock::now() ;

            while((system_clock::now() - t) < 1ms) {} //wait one milisecond

            CHECK(timer.is_passed()) ; //pass once
            CHECK_FALSE(timer.is_passed()) ; //block right now
        }

        SUBCASE("Timer Reset Test") {
            timer.is_passed() ; //pass at first
            CHECK_FALSE(timer.is_passed()) ;
            timer.reset() ;
            CHECK(timer.is_passed()) ; //reseted, so it's the first call
        }
    }
}
