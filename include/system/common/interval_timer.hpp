#ifndef _INTERVAL_TIMER_HPP
#define _INTERVAL_TIMER_HPP

#include <memory>

class IntervalTimer
{
private:
    struct Impl ;
    std::unique_ptr<Impl> pimpl ;

public:
    explicit IntervalTimer(const int delta_us=30) ;

    ~IntervalTimer() ;

    IntervalTimer(IntervalTimer&&) noexcept ;
    IntervalTimer& operator=(IntervalTimer&&) noexcept ;

    void set_delta(const int delta_us) noexcept ;
    void reset() noexcept ;
    bool is_passed() const noexcept ;

    IntervalTimer(const IntervalTimer&) = delete ;
    IntervalTimer& operator=(const IntervalTimer&) = delete ;
} ;

#endif