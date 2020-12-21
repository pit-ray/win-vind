#ifndef _INTERVAL_TIMER_HPP
#define _INTERVAL_TIMER_HPP

#include <memory>

// IntervalTimer provides a function that returns true after a certain amount of time.
class IntervalTimer
{
private:
    struct Impl ;
    std::unique_ptr<Impl> pimpl ;

public:
    explicit IntervalTimer(const int delta_us=30) ;

    virtual ~IntervalTimer() noexcept ;

    IntervalTimer(IntervalTimer&&) ;
    IntervalTimer& operator=(IntervalTimer&&) ;

    void set_delta(const int delta_us) noexcept ;
    void reset() noexcept ;
    bool is_passed() const ;

    IntervalTimer(const IntervalTimer&)             = delete ;
    IntervalTimer& operator=(const IntervalTimer&)  = delete ;
} ;

#endif
