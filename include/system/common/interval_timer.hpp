#ifndef _INTERVAL_TIMER_HPP
#define _INTERVAL_TIMER_HPP

#include <memory>

namespace IntervalTimerDefaults
{
    static constexpr int REPEAT_KEYSTROKE = 1'500'00 ;
}

class IntervalTimer
{
private:
    struct Impl ;
    std::unique_ptr<Impl> pimpl ;

public:
    explicit IntervalTimer(const int delta_us=30) ;

    virtual ~IntervalTimer() noexcept ;

    IntervalTimer(IntervalTimer&&) noexcept ;
    IntervalTimer& operator=(IntervalTimer&&) noexcept ;

    void set_delta(const int delta_us) noexcept ;
    void reset() noexcept ;
    bool is_passed() const noexcept ;

    IntervalTimer(const IntervalTimer&) = delete ;
    IntervalTimer& operator=(const IntervalTimer&) = delete ;
} ;

#endif
