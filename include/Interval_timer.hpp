#ifndef _INTERVAL_TIMER_HPP
#define _INTERVAL_TIMER_HPP

#include <chrono>
#include <mutex>

class IntervalTimer
{
private:
    std::chrono::microseconds delta_time ;
    mutable std::chrono::system_clock::time_point start_time ;
    mutable bool first_call ;
    mutable std::mutex mtx ;

public:
    template <typename T>
    explicit IntervalTimer(const T& delta) noexcept
    : delta_time(static_cast<std::chrono::microseconds>(delta)),
      start_time(std::chrono::system_clock::now()),
      first_call(true),
      mtx()
    {}

    explicit IntervalTimer() noexcept
    : IntervalTimer(30) {}

    ~IntervalTimer() = default ;

    IntervalTimer(IntervalTimer&&) = default ;
    IntervalTimer& operator=(IntervalTimer&&) = default ;

    IntervalTimer(const IntervalTimer&) = default ;
    IntervalTimer& operator=(const IntervalTimer&) = default ;


    void set_delta(std::chrono::microseconds delta) noexcept {
        delta_time = delta ;
    }

    void reset() noexcept {
        start_time = std::chrono::system_clock::now() ;
        first_call = true ;
    }

    const auto is_passed() const noexcept {
        std::lock_guard<std::mutex> lock{mtx} ;

        using namespace std::chrono ;
        if(first_call) {
            first_call = false ;
            return true ;
        }

        const auto dt = system_clock::now() - start_time ;
        if(dt < delta_time) return false ;

        start_time = system_clock::now() ;
        return true ;
    }
} ;

#endif